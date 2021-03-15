
#include <iostream>

#include <boost/asio.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>


#include "src/AppArgParser.h"
#include "src/ConnectionListener.h"
#include "src/ConnectionAcceptor.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include "spdlog/sinks/rotating_file_sink.h"
#include <csignal>

using namespace boost::asio;
using namespace boost::asio::ip;

void sigHandler(int sig)
{
    std::cerr << "sig: " << sig << std::endl;
    exit(1);
}

int main(int argc, char *argv[]) {
    signal(SIGINT, sigHandler);
    signal(SIGTERM, sigHandler);

    std::shared_ptr<spdlog::logger> logger;
    try {
        spdlog::info("Hello, {}!", "World");
        logger = spdlog::basic_logger_mt("basic_logger", "logs/basic.txt");
        auto file_logger = spdlog::rotating_logger_mt("file_logger", "logs/mylogfile", 1048576 * 5, 3);
        logger->info("start logger\n");
        file_logger->error("error test");
    } catch(const spdlog::spdlog_ex &ex) {
        std::cout << "Log initialization failed: " << ex.what() << std::endl;
    }
    io_context ioContext;

    using work_guard_type = boost::asio::executor_work_guard<boost::asio::io_context::executor_type>;
    work_guard_type work_guard(ioContext.get_executor());

    AppArgParser parser{argc, const_cast<const char**>(argv)};

    ConnectionListener listener{ioContext, parser.port()};
//    listener.listen();

    //tcp::socket clientSocket{ioContext};
    ConnectionAcceptor acceptor{ioContext, listener};
    acceptor.start();

    logger->info("Vmx Distributor started for RTSP Proxying on {}\n", parser.port());
    std::cout << "Vmx Distributor started for RTSP Proxying on " << parser.port() << std::endl;

    logger->flush();
    try {
        ioContext.run();
    } catch(std::runtime_error& err)    {
        std::cerr << err.what() << std::endl;
    }

    return 0;
}
