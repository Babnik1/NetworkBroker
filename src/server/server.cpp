/// @file server.h
///
/// Класс TCP сервера. Реализация.
///


#include "server.h"
#include <boost/asio/io_context.hpp>
#include <boost/system/detail/error_code.hpp>
#include <cstdint>
#include <memory>

#include "broker/message_broker.h"


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
    SessionId id = nextSessionId_++;
    auto socket = std::make_shared< boost::asio::ip::tcp::socket >( ioContext_ );
    acceptor_.async_accept( *socket, [ this, socket, id ]( const boost::system::error_code& error )
        {
            if ( !error )
            {
                INFO_ALL( "Client " << socket->remote_endpoint().address() << " connected succesfully" );
                auto session = std::make_shared< Session >( id, std::move( *socket ), broker_ );
                sessions_.emplace(
                    session->GetId(),
                    SessionData{ AuthType::NonAuthorized, 
                              session, 
                                   0 } );
                session->SetSelf( session );
                session->Start();
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

void Server::RemoveSession( SessionId id )
{
    sessions_.erase( id );
}



