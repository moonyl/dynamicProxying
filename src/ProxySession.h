//
// Created by moony on 3/9/21.
//

#pragma once
#include <boost/asio.hpp>

class ProxySession
{
    boost::asio::ip::tcp::socket _toServer;
    boost::asio::ip::tcp::socket _toClient;
    boost::asio::ip::tcp::resolver _resolver;
    enum {
        MaxLengthFromClient = 1024,
        MaxLengthFromServer = 1024
    };
    std::array<char, MaxLengthFromClient> firstData;
    std::array<char, MaxLengthFromClient> reqData;
    std::array<char, MaxLengthFromServer> respData;

public:
    ProxySession(boost::asio::io_context &ioContext);
    void connectProxy();

    operator boost::asio::ip::tcp::socket& ();

private:
    void connectServer(boost::asio::ip::tcp::resolver::results_type::iterator endpointsIter, size_t amountOfBytes = 0);
    void pipeSockets();
    void writeToServer(size_t ammountOfBytes);
    void writeToClient(size_t ammountOfBytes);
    void doRequestTask();
    void doResponseTask();
};


