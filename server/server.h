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


class Server
{
public:

    Server( short port );

    void Start();

    void Stop();

private:
    boost::asio::io_context ioContext_; 
    boost::asio::ip::tcp::acceptor acceptor_;
    std::unordered_map< ClientId, SessionPtr > sessions_;

    void DoAccept();
    
};

using ServerPtr = std::unique_ptr< Server >;