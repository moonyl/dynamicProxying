
#include <iostream>

#include <boost/asio.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>


#include "src/AppArgParser.h"
#include "src/ConnectionListener.h"
#include "src/ConnectionAcceptor.h"

using namespace boost::asio;
using namespace boost::asio::ip;

int main(int argc, char *argv[]) {
    io_context ioContext;

    using work_guard_type = boost::asio::executor_work_guard<boost::asio::io_context::executor_type>;
    work_guard_type work_guard(ioContext.get_executor());

    AppArgParser parser{argc, const_cast<const char**>(argv)};

    ConnectionListener listener{ioContext, parser.port()};
    listener.listen();

    //tcp::socket clientSocket{ioContext};
    ConnectionAcceptor acceptor{ioContext, listener};
    acceptor.start();

    ioContext.run();
    return 0;
}
