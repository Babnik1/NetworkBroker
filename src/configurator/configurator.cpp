/// @file configurator.cpp
///
/// Загрузчик конфигурации. Реализация.
///


#include "configurator.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>


Configurator::Configurator( std::string config )
    : configFile_{ config }
{}

ConfigCodes Configurator::ReadConfig()
{
    Configs configs;
    std::ifstream file( configFile_ );
    if ( !file.is_open() )
    {
        return ConfigCodes::ErrFileNotFound;
    }
    try
    {
        nlohmann::json data = nlohmann::json::parse( file );
        configs.nameFileLog = data.value( "log_file_name", "app.log" );
        configs.port = data.value( "server_port", 2000 );
        configs.topicDb = data.value( "topics_database", "topic.json" );
        configs.clientDb = data.value( "clients_database", "client.json" );
        configs.loglevel = data.value( "log_level", "info" );
    }
    catch ( const nlohmann::json::parse_error& e ) 
    {
        std::cout << "Configs parse error: " << e.what() << "\n";
        return ConfigCodes::ErrReadFile;
    }
    catch ( const nlohmann::json::out_of_range& e )
    {
        std::cout << "Configs invalid key error: " << e.what() << "\n";
        return ConfigCodes::ErrInvalidKey;
    }
    catch ( const nlohmann::json::type_error& e )
    {
        std::cout << "Configs invalid argument error: " << e.what() << "\n";
        return ConfigCodes::ErrInvalidArg;
    }
    catch ( const nlohmann::json::exception& e )
    {
        std::cout << "Configs unknown error: " << e.what() << "\n";
        return ConfigCodes::UnknownError;
    }
    SetConfigs( configs );\
    return ConfigCodes::Ok;
}

void Configurator::SetConfigs( const Configs configs )
{
    configs_ = configs;
}

Configs Configurator::GetConfigs() const 
{
    return configs_;
}

