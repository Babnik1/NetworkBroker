#include "logs/log_manager.h"
#include "logs/log.h"
#include "server/server.h"
#include "configurator/configurator.h"
#include <memory>
#include <string>

const std::string configFile = "config.json";

int main()
{
    ConfiguratorPtr configurator = std::make_unique< Configurator >( configFile );
    ReturnCodes rc = configurator->ReadConfig();
    if ( rc != ReturnCodes::Ok )
    {
        return 1;
    }
    LogManager::Init( configurator->GetConfigs().nameFileLog );
    std::string msg = "Hello";
    INFO_ALL( "Broker was started: " << msg );
    ServerPtr server = std::make_unique< Server >( configurator->GetConfigs().port );
    server->Start();
    server->Stop();
    return 0;
}