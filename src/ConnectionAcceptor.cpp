//
// Created by moony on 3/9/21.
//

#include "ConnectionAcceptor.h"
#include <memory>
#include "ConnectionListener.h"
#include <iostream>
#include "ProxySession.h"
using namespace std;
using namespace boost::asio;
using boost::asio::ip::tcp;


ConnectionAcceptor::ConnectionAcceptor(io_context &ioContext, ConnectionListener &listener) :
    _listener{listener}, _ioContext{ioContext} {}


void ConnectionAcceptor::start() {
    auto session = make_shared<ProxySession>(_ioContext);
    auto sessionRemoveTimer = make_shared<deadline_timer>(_ioContext);
    session->addDisconnectListener([&, session, sessionRemoveTimer](){
        std::cout << "disconnected" << std::endl;

        _sessions.erase(session);
        //deadline_timer t(_ioContext);
//        sessionRemoveTimer->expires_from_now(boost::posix_time::seconds(3));
//        sessionRemoveTimer->async_wait([session](const boost::system::error_code& errCode){
//            std::cout << "it should be delete" << std::endl;
//            session->removeAllListener();
//        });
    });
//    std::cout << "after creation: " << session.get() << std::endl;
    _sessions.insert(session);

    _listener.accept(*session, [&, session](const boost::system::error_code& errCode){
        if (errCode)   {
            std::cerr << "error" << std::endl;
            return;
        }
//        std::cout << "accepted" << std::endl;
//        std::cout << "after accepted: " << session.get() << std::endl;
        session->connectProxy();
        start();
    });
}
