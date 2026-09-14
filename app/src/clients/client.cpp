/// @file client.cpp
///
/// Клиент. Реализация.
///


#include "client.h"


Client::Client( ClientId id, std::string name )
    : id_{ id }
    , name_{ name }
    , session_{ }
{}

ClientId Client::GetId() const
{
    return id_;
}

std::string Client::GetName() const
{
    return name_;
}

void Client::SetSession( SessionWeakPtr session )
{
    session_ = session;
}

SessionId Client::GetSessionId() const
{
    if ( !session_.expired() )
    {
        return session_.lock()->GetId();
    }
    return invalidSessionId;
}

void Client::Disconnect()
{
    session_.reset();
}

void Client::SendTopicMessage( const std::string message )
{
    auto session = session_.lock();
    if( !session )
    {
        return;
    }

    session->Send( message + "\n" );
}

void Client::BufferTopicMessage( const std::string message )
{
    buffer_.push( message );
}

void Client::SendBuffer()
{
    SendTopicMessage( "You have new messages:");
    for ( ; !buffer_.empty(); buffer_.pop() )
    {
        SendTopicMessage( buffer_.front() );
    }
}