/// @file server.h
///
/// Класс TCP сервера. Реализация.
///


#include "server.h"
#include <boost/asio/io_context.hpp>
#include <boost/system/detail/error_code.hpp>
#include <memory>


Server::Server( short port )
    : ioContext_{}
    , acceptor_{ ioContext_, boost::asio::ip::tcp::endpoint( boost::asio::ip::tcp::v4(), port ) }
{
    // sessionsManager_ = std::make_unique< SessionsManager >();
}

void Server::Start()
{
    INFO_ALL( "Server was started..." );
    DoAccept();
    ioContext_.run();
}

void Server::Stop()
{
    INFO_ALL( "Stopping server..." );
    ioContext_.stop();
}

void Server::DoAccept()
{
    auto socket = std::make_shared< boost::asio::ip::tcp::socket >( ioContext_ );
    acceptor_.async_accept( *socket, [ this, socket ]( const boost::system::error_code& error )
        {
            if ( !error )
            {
                INFO_ALL( "Client " << socket << " connected succesfully" );
                /// Тут будем отправлять запрос хочет он зарегаться или залогиниться.
                /// Модуль ауфа будет это все обрабатывать.
                
                auth_->CheckClient();

            }
            else
            {
                ERROR_LOG( "Connection error. Client: " << socket );
            }

            if ( !acceptor_.is_open() )
            {
                WARNING_LOG( "Server was stopped" );
                return;
            }
            DoAccept();
        } );                                    
}



ReturnCodes AttachSession()
{

}

ReturnCodes DetachSession()
{

}



