#pragma once 

/// @file server.h
///
/// Класс TCP сервера. Объявление.
///

#include <boost/asio.hpp>
#include <memory>
#include "../logs/log.h"


class Server
{
public:

    Server( short port );

    void Start();

    void Stop();

private:
    boost::asio::io_context ioContext_; 
    boost::asio::ip::tcp::acceptor acceptor_;
    // SessionsManagerPtr sessionsManager_;

    void DoAccept();
    
};

using ServerPtr = std::unique_ptr< Server >;