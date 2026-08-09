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

/// @todo Добавить возможность выставления дефолта , если не прочитали.
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
        configs.nameFileLog = data.at( "log_file_name" );
        configs.port = data.at( "server_port" ).get< short >();
        configs.topicDb = data.at( "topics_database" );
        configs.clientDb = data.at( "clients_database" );
        configs.loglevel = data.at( "log_level" );
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

