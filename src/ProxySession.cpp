//
// Created by moony on 3/9/21.
//

#include "ProxySession.h"
#include <iostream>
#include <skyr/url.hpp>

using namespace boost::asio;
using boost::asio::ip::tcp;

ProxySession::ProxySession(io_context &ioContext) : _toServer(ioContext), _toClient(ioContext), _resolver(ioContext) {}

void ProxySession::connectProxy() {
    _toClient.async_read_some(boost::asio::buffer(reqData.data(), reqData.size()),
                              [&](const boost::system::error_code &errCode, size_t ammountOfBytes) {
                                  if (!errCode) {

                                      string_view line(reqData.data(), ammountOfBytes);
                                      std::cout << "line: " << line << std::endl;
                                      auto pos = line.find("rtsp://");
                                      auto end = line.find(" RTSP/");
                                      auto urlStr = line.substr(pos, end - pos);
                                      skyr::url url{urlStr};
                                      std::cout << "port: " << url.port() << ", hostname: " << url.hostname() << std::endl;

                                      std::cout << url.hostname() << ", " << url.port() << ", " << url.pathname() << std::endl;
                                      _resolver.async_resolve(url.hostname(), "8554",
                                                              [&, url, ammountOfBytes](const boost::system::error_code &errCode, tcp::resolver::results_type results) {
                                                                  std::cout << "resolved, " << url.host() << std::endl;
                                                                  if (!errCode) {
                                                                      connectServer(results, ammountOfBytes);
                                                                  } else {
                                                                      std::cerr << errCode.message() << std::endl;
                                                                      return;
                                                                  }
                                                              });

                                  } else {
                                      std::cerr << errCode.message() << std::endl;

                                  }

                              });

//    string_view urlStr{"rtsp://172.25.223.203:8554/192.168.15.107:554/onvif/profile3/media.smp"};
//    skyr::url url{urlStr};
//    std::cout << __func__ << std::endl;
//    std::cout << url.hostname() << ", " << url.port() << ", " << url.pathname() << std::endl;
//    _resolver.async_resolve(url.hostname(), url.port(),
//                          [&, url](const boost::system::error_code &errCode, tcp::resolver::results_type results) {
//                              std::cout << "resolved, " << url.host() << std::endl;
//                              if (!errCode) {
//                                  connectServer(results);
//                              } else {
//                                  std::cerr << errCode.message() << std::endl;
//                                  return;
//                              }
//                          });

}

void ProxySession::connectServer(tcp::resolver::results_type::iterator endpointsIter, size_t ammountOfBytes) {
    _toServer.async_connect(endpointsIter->endpoint(), [&, ammountOfBytes](const boost::system::error_code &errCode) {
        if (!errCode) {
            if (ammountOfBytes) {
                writeToServer(ammountOfBytes);
            }

            pipeSockets();
            std::cout << "connected " << std::endl;
        } else {
            std::cerr << errCode.message() << std::endl;
            return;
        }
    });
}

void ProxySession::pipeSockets() {
    doRequestTask();
    doResponseTask();


}

void ProxySession::writeToServer(size_t ammountOfBytes) {
    _toServer.async_write_some(boost::asio::buffer(reqData.data(), ammountOfBytes),
                               [&](const boost::system::error_code &errCode, size_t ammountOfBytes) {
                                   if (!errCode) {
                                       std::cout << "written to server: " << ammountOfBytes << std::endl;
                                   } else {
                                       std::cerr << errCode.message() << std::endl;
                                       return;
                                   }
                               });
}

void ProxySession::writeToClient(size_t ammountOfBytes) {
    _toClient.async_write_some(boost::asio::buffer(respData.data(), ammountOfBytes),
                               [&](const boost::system::error_code &errCode, size_t ammountOfBytes) {
                                   if (!errCode) {
                                       std::cout << "written to client: " << ammountOfBytes << std::endl;
                                   } else {
                                       std::cerr << errCode.message() << std::endl;
                                       return;
                                   }
                               });
}

void ProxySession::doRequestTask() {
    _toClient.async_read_some(boost::asio::buffer(reqData.data(), reqData.size()),
                              [&](const boost::system::error_code &errCode, size_t ammountOfBytes) {
                                  if (!errCode) {
                                      writeToServer(ammountOfBytes);
                                      doRequestTask();
                                  } else {
                                      std::cerr << errCode.message() << std::endl;
                                      return;
                                  }
                              });
}

void ProxySession::doResponseTask() {
    _toServer.async_read_some(boost::asio::buffer(respData.data(), respData.size()),
                              [&](const boost::system::error_code &errCode, size_t ammountOfBytes) {
                                  if (!errCode) {
                                      writeToClient(ammountOfBytes);
                                      doResponseTask();
                                  } else {
                                      std::cerr << errCode.message() << std::endl;
                                      return;
                                  }
                              });
}

ProxySession::operator tcp::socket &() {
    std::cout << "handle: " << _toClient.native_handle() << std::endl;
    return _toClient;
}
