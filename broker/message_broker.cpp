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

static const std::unordered_map< std::string, Actions > commands =
{
    { "LOGIN",      Actions::LOGIN      },
    { "REGISTER",   Actions::REGISTER   },
    { "PUBLISH",    Actions::PUBLISH    },
    { "SUBSCRIBE",  Actions::SUBSCRIBE  }
};


std::string CodeToString( BrokerCodes rc )
{
    switch ( rc )
    {
        case BrokerCodes::Ok:
            return "OK\n";

        case BrokerCodes::ClientNotFound:
            return "ERROR Client not found\n";

        case BrokerCodes::ClientAlreadyExists:
            return "ERROR Client already exists\n";

        case BrokerCodes::ClientAlreadyConnected:
            return "ERROR Client already connected\n";

        case BrokerCodes::InvalidCommand:
            return "ERROR Invalid command\n";

        case BrokerCodes::TopicNotFound:
            return "ERROR Topic not found\n";

        case BrokerCodes::AlreadySubscribed:
            return "ERROR Already subscribed\n";

        default:
            return "ERROR Internal error\n";
    }
}


MessageBroker::MessageBroker( ClientManagerPtr cliManager, TopicManagerPtr topManager )
    : cliManager_{ cliManager }
    , topManager_{ topManager }
{}


/// @todo Обработать, если LOGINpasha - склеено.
std::string MessageBroker::HandleCommand( SessionId id, const std::string& command, SessionWeakPtr session )
{
    std::string action;
    std::string argument;

    std::istringstream iss( command );

    if ( !( iss >> action ) )
    {
        return CodeToString( BrokerCodes::InvalidCommand );
    }
    

    std::getline( iss, argument );

   if ( !argument.empty() )
    {
        argument.erase( 0, 1 );
    }

    if ( argument.empty() )
    {
        return CodeToString( BrokerCodes::InvalidCommand );
    }

    auto it = commands.find( action );

    if ( it == commands.end() )
    {
        return CodeToString( BrokerCodes::InvalidCommand );
    }
    BrokerCodes rc = BrokerCodes::InvalidCommand;

    switch ( it->second )
    {
        case Actions::LOGIN:
            rc = Login( id, argument, session );
            break;

        case Actions::REGISTER:
            rc = Register( id, argument, session );
            break;

        case Actions::SUBSCRIBE:
            rc = Subscribe( id, argument );
            break;

        case Actions::PUBLISH:
            std::istringstream pub( argument );

            std::string topic;
            std::string message;

            pub >> topic;
            if ( topic.empty() )
            {
                return CodeToString( BrokerCodes::InvalidCommand );
            }

            std::getline( pub, message );

            if ( !message.empty() )
            {
                message.erase( 0, 1 );
            }

            if ( message.empty() )
            {
                return CodeToString( BrokerCodes::InvalidCommand );
            }

            if ( !message.empty() )
            {
                message.erase( 0, 1 );
            }

            rc = Publish( id, topic, message );
            break;
    }

    return CodeToString( rc );
}

BrokerCodes MessageBroker::Login( SessionId id, const std::string& name, SessionWeakPtr session )
{
    if ( cliManager_->ConnectClient( name , id, session ))
    {
        return BrokerCodes::ClientNotFound;
    }
    else 
    {
        return BrokerCodes::Ok;
    }
}

BrokerCodes MessageBroker::Register( SessionId id, const std::string& name, SessionWeakPtr session )
{
    BrokerCodes rc = static_cast< BrokerCodes >( cliManager_->CreateClient( name ) );
    if ( rc != BrokerCodes::Ok )
    {
        return rc;
    }
    rc =  static_cast< BrokerCodes >( cliManager_->ConnectClient( name , id, session ) );
    if ( rc != BrokerCodes::Ok )
    {
        return rc;
    }

    return BrokerCodes::Ok;
}

BrokerCodes MessageBroker::Publish( SessionId id, const std::string& topic, const std::string& message )
{
    ClientId clientId = cliManager_->GetClientId( id );
    if ( clientId == InvalidClientId )
    {
        return BrokerCodes::Unauthorized;
    }
    return topManager_->Publish( clientId, topic, message );
}

BrokerCodes MessageBroker::Subscribe( SessionId id, const std::string& topic )
{
    ClientId clientId = cliManager_->GetClientId( id );
    if ( clientId == InvalidClientId )
    {
        return BrokerCodes::Unauthorized;
    }
    return topManager_->Subscribe( clientId, topic );
}