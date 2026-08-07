#include "logs/log_manager.h"
#include "logs/log.h"
#include "server/server.h"
#include "configurator/configurator.h"
#include "db/json_client_repository.h"
#include "db/json_topic_repository.h"
#include "clients/clients_manager.h"
#include "topics/topic_manager.h"
#include "broker/message_broker.h"
#include <memory>
#include <string>

const std::string configFile = "config.json";

int main()
{
    INFO_ALL( "Application is running... " );

    ConfiguratorPtr configurator = std::make_unique< Configurator >( configFile );
    ConfigCodes rc = configurator->ReadConfig();
    if ( rc != ConfigCodes::Ok )
    {
        return 1;
    }
    LogManager::Init( configurator->GetConfigs().nameFileLog );
    INFO_LOG( "Configurator was started succesfully" );
    INFO_LOG( "Log manager was started succesfully" );
    INFO_SHELL( "Console log session was started" );
    INFO_LOG( "File log session was started" );

    auto clientsRepository = std::make_unique< JsonRepository >( "clientsbase.json" ); 
    auto clientManager = std::make_shared< ClientManager >( std::move( clientsRepository ) );

    auto topicsRepository = std::make_unique< JsonTopicRepository >( "topicsbase.json" ); 
    auto topicManager = std::make_shared< TopicManager >( std::move( topicsRepository ) );

    auto broker = std::make_shared< MessageBroker >( clientManager, topicManager );

    ServerPtr server = std::make_unique< Server >( configurator->GetConfigs().port, broker );
    server->Start();
    return 0;
}