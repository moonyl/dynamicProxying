//
// Created by moony on 3/9/21.
//

#pragma once

#include <boost/asio.hpp>

class ConnectionListener
{
    boost::asio::ip::tcp::endpoint tcpEndPoint;
    boost::asio::ip::tcp::acceptor tcpAcceptor;

public:
    ConnectionListener(boost::asio::io_context& ioContext, unsigned short port);
    void listen();

    void accept(boost::asio::ip::tcp::socket &clientSocket,
                std::function<void(const boost::system::error_code &)> &&onAccept);
};

