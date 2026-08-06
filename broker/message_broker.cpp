/// @file message_broker.cpp
///
/// Брокер. Реализация.
///


#include "message_broker.h"
#include "../clients//clients_manager.h"
#include "rc.h"
#include <cstddef>
#include <sys/types.h>
#include <unordered_map>
#include <vector>

enum class Actions
{
    PUBLISH     = 0,
    SUBSCRIBE   = 1,
    LOGIN       = 2,
    REGISTER    = 3,
};

std::vector< std::string > actions = {"PUBLISH", "SUBSCRIBE", "LOGIN", "REGISTER" };


std::string CodeToString( BrokerCodes rc )
{

}


MessageBroker::MessageBroker( ClientManagerPtr cliManager, TopicManagerPtr topManager )
    : cliManager_{ cliManager }
    , topManager_{ topManager }
{}


/// @todo Обработать, если LOGINpasha - склеено.
std::string MessageBroker::HandleCommand( SessionId id, const std::string& command )
{
    std::size_t position;
    BrokerCodes rc = BrokerCodes::InvalidCommand;

    for ( std::size_t i = 0; i < actions.size(); ++i )
    {
        std::size_t pos = command.find( actions[ i ] );
        if( pos != std::string::npos )
        {
            position = i;
        }
        else 
        {
            return CodeToString( rc );
        }
    }

    auto eraser = [ ]( std::string cmd ) -> std::string
    {
        std::size_t spacePos = cmd.find( ' ' );
        if ( spacePos != std::string::npos )
        {
            return cmd.erase( 0, spacePos + 1 );
        }
        return {};
    };


    switch ( static_cast< Actions >( position ) ) 
    {
        case Actions::PUBLISH:
        {
            
            rc = Publish( id, eraser( command ) );
            break;
        }
        case Actions::LOGIN:
        {
            rc = Login( id, eraser( command ) );
            break;
        }
        case Actions::REGISTER:
        {
            rc = Register( id, eraser( command ) );
            break;
        }
        case Actions::SUBSCRIBE:
        {
            rc = Subscribe( id, eraser( command ) );
            break;
        }
    }

    return CodeToString( rc );

}

BrokerCodes MessageBroker::Login( SessionId id, const std::string& name )
{
    if ( cliManager_->ConnectClient( name , id ) == -1 )
    {
        return BrokerCodes::ClientNotFound;
    }
    else 
    {
        return BrokerCodes::Ok;
    }
}

BrokerCodes MessageBroker::Register( SessionId id, const std::string& name )
{
    if ( cliManager_->CreateClient( name )
    {
        return 
    }
}

BrokerCodes MessageBroker::Publish( SessionId id, const std::string& message )
{

}

BrokerCodes MessageBroker::Subscribe( SessionId id, const std::string& message )
{

}