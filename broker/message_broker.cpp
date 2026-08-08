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
#include <unordered_set>
#include <vector>
#include "../topics/topic_manager.h"
#include "../logs/log.h"


namespace  
{

/// @brief Действия.
enum class Actions
{
    PUBLISH     = 0,    /// Публикация сообщения в топик.
    SUBSCRIBE   = 1,    /// Подписка на топик.
    LOGIN       = 2,    /// Аутентификация.
    REGISTER    = 3,    /// Регистрация.
    CREATE      = 4,    /// Создание нового топика.
    UNSUBSCRIBE = 5,    /// Отписка от топика.
};

/// @brief Мапа коммант и действий.
static const std::unordered_map< std::string, Actions > commands =
{
    { "LOGIN",          Actions::LOGIN          },
    { "REGISTER",       Actions::REGISTER       },
    { "PUBLISH",        Actions::PUBLISH        },
    { "SUBSCRIBE",      Actions::SUBSCRIBE      },
    { "CREATE",         Actions::CREATE         },
    { "UNSUBSCRIBE",    Actions::UNSUBSCRIBE    }
};

/// @brief Код ошибки в сообщение клиенту.
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

        case BrokerCodes::Unauthorized:
            return "ERROR You are unauthorized\n";

        default:
            return "ERROR Internal error\n";
    }
}

} // anonimous namespace

MessageBroker::MessageBroker( ClientManagerPtr cliManager, TopicManagerPtr topManager )
    : cliManager_{ cliManager }
    , topManager_{ topManager }
{}

std::string MessageBroker::HandleCommand( SessionId id, const std::string& command, SessionWeakPtr session )
{
    std::string action;
    std::string argument;

    std::istringstream iss( command );

    if ( !( iss >> action ) )
    {
        return CodeToString( BrokerCodes::InvalidCommand );
        DEBUG_LOG( "Failed stream to action" );
    }
    

    std::getline( iss, argument );

   if ( !argument.empty() )
    {
        argument.erase( 0, 1 );
    }

    if ( argument.empty() )
    {
        DEBUG_LOG( "Argument is empty" );
        return CodeToString( BrokerCodes::InvalidCommand );
    }

    auto it = commands.find( action );

    if ( it == commands.end() )
    {
        DEBUG_LOG( "There is not command in map: " << action );
        return CodeToString( BrokerCodes::InvalidCommand );
    }
    BrokerCodes rc = BrokerCodes::InvalidCommand;

    switch ( it->second )
    {
        case Actions::LOGIN:
        {
            rc = Login( id, argument, session );
            break;
        }
        case Actions::REGISTER:
        {
            rc = Register( id, argument, session );
            break;
        }
        case Actions::SUBSCRIBE:
        {
            rc = Subscribe( id, argument );
            break;
        }
        case Actions::PUBLISH:
        {
            std::istringstream pub( argument );

            std::string topic;
            std::string message;

            pub >> topic;
            if ( topic.empty() )
            {
                DEBUG_LOG( "PUBLISH: Topic is empty" );
                return CodeToString( BrokerCodes::InvalidCommand );
            }

            std::getline( pub, message );

            if ( message.empty() )
            {
                DEBUG_LOG( "PUBLISH: Message is empty" );
                return CodeToString( BrokerCodes::InvalidCommand );
            }

            if ( !message.empty() )
            {
                message.erase( 0, 1 );
            }

            rc = Publish( id, topic, message );
            break;
        }
        case Actions::CREATE:
        {
            rc = Create( id, argument );
            break;
        }
        case Actions::UNSUBSCRIBE:
        {
            rc = Unsubscribe( id, argument );
        }
    }

    return CodeToString( rc );
}

BrokerCodes MessageBroker::Login( SessionId id, const std::string& name, SessionWeakPtr session )
{
    return static_cast< BrokerCodes >( cliManager_->ConnectClient( name , id, session ) );
}

BrokerCodes MessageBroker::Register( SessionId id, const std::string& name, SessionWeakPtr session )
{
    BrokerCodes rc = static_cast< BrokerCodes >( cliManager_->CreateClient( name ) );
    if ( rc != BrokerCodes::Ok )
    {
        return rc;
    }
    INFO_ALL( "Client " << name << " registered successfully" );
    rc =  static_cast< BrokerCodes >( cliManager_->ConnectClient( name , id, session ) );
    if ( rc != BrokerCodes::Ok )
    {
        return rc;
    }
    return BrokerCodes::Ok;
}

/// @todo Сделать так, чтобы самому себе не отправлялось.
/// Наверн надо передать айди сессии, которую исключаем и в сессии проверять есть ли он.
BrokerCodes MessageBroker::Publish( SessionId id, const Topic& topic, const std::string& message )
{
    ClientId clientId;
    BrokerCodes rc = static_cast< BrokerCodes >( cliManager_->GetClientId( id, clientId ) );
    if ( rc == BrokerCodes::ClientNotFound )
    {
        return BrokerCodes::Unauthorized;
    }

    std::unordered_set< ClientId > clients;
    rc = static_cast< BrokerCodes >( topManager_->Publish( topic, message, clients ) );
    if ( rc == BrokerCodes::TopicNotFound )
    {
        return rc;
    }

    cliManager_->SendTopicMessage( message, clients );
    return  BrokerCodes::Ok;
}

BrokerCodes MessageBroker::Subscribe( SessionId id, const Topic& topic )
{
    ClientId clientId;
    BrokerCodes rc = static_cast< BrokerCodes >( cliManager_->GetClientId( id, clientId ) );
    if ( rc == BrokerCodes::ClientNotFound )
    {
        return BrokerCodes::Unauthorized;
    }
    return static_cast< BrokerCodes >( topManager_->Subscribe( clientId, topic ) );
}

BrokerCodes MessageBroker::Create( SessionId id, const Topic& topic )
{
    ClientId clientId;
    BrokerCodes rc = static_cast< BrokerCodes >( cliManager_->GetClientId( id, clientId ) );
    if ( rc == BrokerCodes::ClientNotFound )
    {
        return BrokerCodes::Unauthorized;
    }
    return static_cast< BrokerCodes >( topManager_->Create( clientId, topic ) );
}

BrokerCodes MessageBroker::Unsubscribe( SessionId id, const Topic& topic )
{
    ClientId clientId;
    BrokerCodes rc = static_cast< BrokerCodes >( cliManager_->GetClientId( id, clientId ) );
    if ( rc == BrokerCodes::ClientNotFound )
    {
        return BrokerCodes::Unauthorized;
    }
    return static_cast< BrokerCodes >( topManager_->Unsubscribe( clientId, topic ) );
}

void MessageBroker::Disconnect( SessionId id )
{
    cliManager_->DisconnectClient( id );
}