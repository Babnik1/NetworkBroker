#include "src/logs/log_manager.h"
#include "src/logs/log.h"
#include "src/server/server.h"
#include "src/configurator/configurator.h"
#include "src/db/json_client_repository.h"
#include "src/db/json_topic_repository.h"
#include "src/clients/clients_manager.h"
#include "src/topics/topic_manager.h"
#include "src/broker/message_broker.h"
#include <memory>
#include <string>
#include <iostream>

const std::string configFile = "config.json";

int main()
{
    std::cout << "Application is running... ";
    ConfiguratorPtr configurator = std::make_unique< Configurator >( configFile );
    ConfigCodes rc = configurator->ReadConfig();
    if ( rc != ConfigCodes::Ok )
    {
        return 1;
    }
    std::cout << "Configurator was started succesfully";
    LogManager::Init( configurator->GetConfigs().nameFileLog, configurator->GetConfigs().loglevel );
    INFO_LOG( "Log manager was started succesfully" );
    INFO_SHELL( "Console log session was started" );
    INFO_LOG( "File log session was started" );

    auto clientsRepository = std::make_unique< JsonRepository >( configurator->GetConfigs().clientDb ); 
    auto clientManager = std::make_shared< ClientManager >( std::move( clientsRepository ) );

    auto topicsRepository = std::make_unique< JsonTopicRepository >( configurator->GetConfigs().topicDb ); 
    auto topicManager = std::make_shared< TopicManager >( std::move( topicsRepository ) );

    auto broker = std::make_shared< MessageBroker >( clientManager, topicManager );

    ServerPtr server = std::make_unique< Server >( configurator->GetConfigs().port, broker );
    server->Start();
    return 0;
}