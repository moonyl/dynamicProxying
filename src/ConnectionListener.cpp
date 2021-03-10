//
// Created by moony on 3/9/21.
//

#include "ConnectionListener.h"
using namespace std;
using namespace boost::asio;
using boost::asio::ip::tcp;


ConnectionListener::ConnectionListener(io_context &ioContext, unsigned short port) :
        tcpEndPoint{tcp::v4(), port},
        tcpAcceptor{ioContext, tcpEndPoint} {}

void ConnectionListener::listen() {
    tcpAcceptor.listen();
}


void ConnectionListener::accept(tcp::socket& clientSocket, function<void(const boost::system::error_code&)>&& onAccept) {
    tcpAcceptor.async_accept(clientSocket, std::move(onAccept));
}
