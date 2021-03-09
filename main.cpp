
#include <iostream>

#include <boost/asio.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>

using namespace boost::asio;

int main() {
    io_context ioContext;

    using work_guard_type = boost::asio::executor_work_guard<boost::asio::io_context::executor_type>;
    work_guard_type work_guard(ioContext.get_executor());

    ioContext.run();
    return 0;
}
