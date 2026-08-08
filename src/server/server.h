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
#include "fwd.h"

enum class AuthType
{
    Authorized      = 0,
    NonAuthorized   = 1,
};

/// @todo Не нужно так наверн. Подумать.
struct SessionData
{
    AuthType auth;
    SessionPtr session;
    ClientId id;
};


class Server
{
public:

    Server( short port, std::shared_ptr< MessageBroker > broker );

    void Start();

    void Stop();

    void RemoveSession( SessionId id );

private:
    SessionId nextSessionId_{ 1 };
    boost::asio::io_context ioContext_; 
    boost::asio::ip::tcp::acceptor acceptor_;
    std::unordered_map< SessionId, SessionData > sessions_;
    std::shared_ptr< MessageBroker > broker_;

    void DoAccept();
    
};

using ServerPtr = std::unique_ptr< Server >;