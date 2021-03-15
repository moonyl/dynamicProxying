//
// Created by moony on 3/9/21.
//

#pragma once
#include <boost/asio.hpp>
#include <boost/signals2.hpp>

class ProxySession
{
    boost::asio::ip::tcp::socket _toServer;
    boost::asio::ip::tcp::socket _toClient;
    boost::asio::ip::tcp::resolver _resolver;
    enum {
        MaxLengthFromClient = 512,
        MaxLengthFromServer = 1024 * 20
    };
    std::array<char, MaxLengthFromClient> reqData;
    std::array<char, MaxLengthFromServer> respData;

    boost::signals2::signal<void()> _disconnectSignal;
    std::vector<boost::signals2::connection> _connections;
    bool _state = true;
    boost::asio::io_context &_ioContext;
    boost::asio::deadline_timer _sessionRemoveTimer;

public:
    ProxySession(boost::asio::io_context &ioContext);
    ~ProxySession();
    void connectProxy();
    void addDisconnectListener(std::function<void()> handler);
    void removeAllListener();

    operator boost::asio::ip::tcp::socket& ();

private:
    void connectServer(boost::asio::ip::tcp::resolver::results_type::iterator endpointsIter, size_t amountOfBytes = 0);
    void pipeSockets();
    void writeToServer(size_t ammountOfBytes);
    void writeToClient(size_t ammountOfBytes);
    void doRequestTask();
    void doResponseTask();
    void resolveServer(const std::string_view &hostname, const std::string_view &port, size_t bytesNotYetSent);

    void handleCommonError(const boost::system::error_code &errCode);
};


