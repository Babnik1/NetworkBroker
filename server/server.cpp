/// @file server.h
///
/// Класс TCP сервера. Реализация.
///

#include "server.h"
#include <boost/asio/io_context.hpp>
#include <boost/system/detail/error_code.hpp>
#include <memory>


Server::Server( short port, std::shared_ptr< MessageBroker > broker )
    : ioContext_{}
    , acceptor_{ ioContext_, boost::asio::ip::tcp::endpoint( boost::asio::ip::tcp::v4(), port ) }
    , broker_{ broker }
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
                INFO_ALL( "Client " << socket->remote_endpoint().address() << " connected succesfully" );
                auto session = std::make_shared< Session >( std::move( *socket ) );

                if ( broker_->GetClientManager()->ConnectClient( session ) )
                {
                    sessions_.emplace( session->GetId(), session );
                }
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



