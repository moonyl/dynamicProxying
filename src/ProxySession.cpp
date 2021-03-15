//
// Created by moony on 3/9/21.
//

#include "ProxySession.h"
#include <iostream>
#include <skyr/url.hpp>
#include "ProxyHostFinder.h"
#include "utils/urls.h"
#include "redis/RedisClient.h"

using namespace boost::asio;
using boost::asio::ip::tcp;
using namespace std;

ProxySession::ProxySession(io_context &ioContext) :
    _toServer{ioContext}, _toClient{ioContext}, _resolver{ioContext},
    _ioContext{ioContext}, _sessionRemoveTimer{ioContext} {}

void ProxySession::resolveServer(const string_view& hostname, const string_view& port, size_t bytesNotYetSent) {
    _resolver.async_resolve(hostname, port,
                            [&, bytesNotYetSent](const boost::system::error_code &errCode, tcp::resolver::results_type results) {
                                //std::cout << "resolved, " << url.host() << std::endl;
                                if (!errCode) {
                                    connectServer(results, bytesNotYetSent);
                                } else {
                                    std::cerr << errCode.message() << std::endl;
                                    return;
                                }
                            });
}



void ProxySession::connectProxy() {
    try {

    _toClient.async_read_some(boost::asio::buffer(reqData.data(), reqData.size()),
                              [&](const boost::system::error_code &errCode, size_t ammountOfBytes) {
                                  if (!errCode) {
                                      string_view line(reqData.data(), ammountOfBytes);
                                      std::cout << "line: " << line << std::endl;
                                      auto pathName = pathnameFromHttpRequest(line);
                                      if (pathName.empty()) {
                                          pathName = pathnameFromRtspRequest(line);
                                      }
//                                      auto url = createUrlFromConnectReq(line);

                                      // TODO: redis를 통해서 url.pathname() 를 키로 하여 내부 proxy 서버의 주소와 포트를 획득해야 한다.
                                      ProxyHostFinder finder{pathName, RedisClient::instance()};
                                      resolveServer(finder.hostname(), finder.port(), ammountOfBytes);

                                  } else {
                                      // TODO : remain log
                                      std::cerr << errCode.message() << std::endl;
                                  }
                              });

    } catch (std::runtime_error& err)   {
        std::cerr << err.what() << std::endl;
    }
}

void ProxySession::connectServer(tcp::resolver::results_type::iterator endpointsIter, size_t ammountOfBytes) {
    _toServer.async_connect(endpointsIter->endpoint(), [&, ammountOfBytes](const boost::system::error_code &errCode) {
        if (!errCode) {
            if (ammountOfBytes) {
                writeToServer(ammountOfBytes);
            }

            pipeSockets();
        } else {
            handleCommonError(errCode);
        }
    });
}

void ProxySession::pipeSockets() {
    doRequestTask();
    doResponseTask();
}


void ProxySession::handleCommonError(const boost::system::error_code &errCode) {
    // TODO : remain log
    std::cerr << errCode.message() << std::endl;
    //throw std::runtime_error{errCode.message()};
    //_disconnectSignal();
    _toClient.close();
    _toServer.close();

    if (_state) {
        _state = false;
        _sessionRemoveTimer.expires_from_now(boost::posix_time::seconds(2));
        _sessionRemoveTimer.async_wait([&](const boost::system::error_code& errCode)    {
            _disconnectSignal();
            removeAllListener();
            std::cout << __func__ << std::endl;
        });
    }
}

void ProxySession::writeToServer(size_t ammountOfBytes) {
    _toServer.async_write_some(boost::asio::buffer(reqData.data(), ammountOfBytes),
                               [&](const boost::system::error_code &errCode, size_t ammountOfBytes) {
        if (errCode)    {
            handleCommonError(errCode);
        }
    });
}

void ProxySession::writeToClient(size_t ammountOfBytes) {
    _toClient.async_write_some(boost::asio::buffer(respData.data(), ammountOfBytes),
                               [&](const boost::system::error_code &errCode, size_t ammountOfBytes) {
        if (errCode)    {
            handleCommonError(errCode);
        }
    });
}

void ProxySession::doRequestTask() {
    if (_state == true) {
        _toClient.async_read_some(boost::asio::buffer(reqData.data(), reqData.size()),
                                  [&](const boost::system::error_code &errCode, size_t ammountOfBytes) {
                                      if (!errCode) {
                                          writeToServer(ammountOfBytes);
                                          doRequestTask();
                                      } else {
                                          handleCommonError(errCode);
                                      }
                                  });
    } else {
        std::cerr << "maybe disconnected by user, " << __func__ << std::endl;
    }
}

void ProxySession::doResponseTask() {
    if (_state == true) {
        _toServer.async_read_some(boost::asio::buffer(respData.data(), respData.size()),
                                  [&](const boost::system::error_code &errCode, size_t ammountOfBytes) {
                                      if (!errCode) {
                                          writeToClient(ammountOfBytes);
                                          doResponseTask();
                                      } else {
                                          handleCommonError(errCode);
                                      }
                                  });
    } else {
        std::cerr << "maybe disconnected by user, " << __func__ << std::endl;
    }

}

void ProxySession::addDisconnectListener(std::function<void()> handler) {
    _connections.push_back(_disconnectSignal.connect(handler));
}

void ProxySession::removeAllListener() {
    for (auto const & connect : _connections)   {
        connect.disconnect();
    }
}

ProxySession::~ProxySession() {
    std::cout << __func__ << std::endl;
}

ProxySession::operator tcp::socket &() {
    return _toClient;
}
