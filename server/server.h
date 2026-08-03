#pragma once 

/// @file server.h
///
/// Класс TCP сервера. Объявление.
///

#include <boost/asio.hpp>
#include <memory>
#include <unordered_map>
#include <vector>
#include "../logs/log.h"
#include "session.h"
#include "../fwd.h"


class Server
{
public:

    Server( short port, std::shared_ptr< MessageBroker > broker );

    void Start();

    void Stop();

private:
    boost::asio::io_context ioContext_; 
    boost::asio::ip::tcp::acceptor acceptor_;
    std::unordered_map< ClientId, SessionPtr > sessions_;
    std::shared_ptr< MessageBroker > broker_;

    void DoAccept();
    
};

using ServerPtr = std::unique_ptr< Server >;