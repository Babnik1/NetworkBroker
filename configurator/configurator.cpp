#include "configurator.h"

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>


Configurator::Configurator( std::string config )
    : configFile_{ config }
{}

ReturnCodes Configurator::ReadConfig()
{
    Configs configs;
    std::ifstream file( configFile_ );
    if ( !file.is_open() )
    {
        return ReturnCodes::ErrFileNotFound;
    }
    try
    {
        nlohmann::json data = nlohmann::json::parse( file );
        configs.nameFileLog = data.at( "log_file_name" );
        configs.port = data.at( "server_port" ).get< short >();
    }
    catch ( const nlohmann::json::parse_error& e ) 
    {
        std::cout << "Configs parse error: " << e.what() << "\n";
        return ReturnCodes::ErrReadFile;
    }
    catch ( const nlohmann::json::out_of_range& e )
    {
        std::cout << "Configs invalid key error: " << e.what() << "\n";
        return ReturnCodes::ErrInvalidKey;
    }
    catch ( const nlohmann::json::type_error& e )
    {
        std::cout << "Configs invalid argument error: " << e.what() << "\n";
        return ReturnCodes::ErrInvalidArg;
    }
    catch ( const nlohmann::json::exception& e )
    {
        std::cout << "Configs unknown error: " << e.what() << "\n";
        return ReturnCodes::UnknownError;
    }
    SetConfigs( configs );
    return ReturnCodes::Ok;
}

void Configurator::SetConfigs( const Configs configs )
{
    configs_ = configs;
}

Configs Configurator::GetConfigs() const 
{
    return configs_;
}
