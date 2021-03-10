//
// Created by moony on 3/9/21.
//

#pragma once

#include <set>
#include <memory>
#include <boost/asio.hpp>

class ConnectionListener;
class ProxySession;
class ConnectionAcceptor
{
    std::set<std::shared_ptr<ProxySession>> _sessions;
    ConnectionListener &_listener;
    boost::asio::io_context &_ioContext;

public:
    ConnectionAcceptor(boost::asio::io_context &ioContext, ConnectionListener &listener);

    void start();
};


