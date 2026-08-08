/// @file json_client_repository.cpp
///
/// База данных клиентов в JSON. Реализация.
///


#include "json_client_repository.h"
#include <cstdint>
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#include "../logs/log.h"


JsonRepository::JsonRepository( const std::string dbFile )
    : dbFile_{ dbFile }
{}

std::vector< Client > JsonRepository::LoadClients()
{
    std::vector< Client > clients;
    nlohmann::json jsonClients;
    std::ifstream file( dbFile_ );
    if ( !file.is_open() )
    {
        ERROR_LOG( "Failed to open data base file for read: " << dbFile_ );
        return {};
    }
    try
    {
        file >> jsonClients;
        file.close();
   
        for ( const auto& [ key, value ] : jsonClients.items() )
        {
            Client client
            {  
                std::stoull( key ), 
                value[ "name" ].get< std::string >() 
            };

            clients.push_back( client );
        }
    }
    catch ( const std::exception& e )
    {
        ERROR_LOG( "Failed to parse clients: " << e.what() );
        return {};
    }
    return clients;
}

bool JsonRepository::SaveClient( const Client& client )
{
    nlohmann::json jsonClients;
    std::ifstream file( dbFile_ );
    if ( !file.is_open() )
    {
        ERROR_LOG( "Failed to open data base file for read: " << dbFile_ );
        return false;
    }
    try
    {
        file >> jsonClients;
        file.close();
    }
    catch ( const nlohmann::json::exception& e )
    {
        ERROR_LOG( "Failed to parse database file: " << e.what() );
        return false;
    }
    std::string clientIdStr = std::to_string( client.GetId() );
    if ( jsonClients.contains( clientIdStr ) )
    {
        jsonClients[ clientIdStr ][ "name" ] = client.GetName();
        INFO_LOG( "Client " << client.GetName() << " updated successfully" );
    }
    else 
    {
        INFO_LOG( "Client " << client.GetName() << " is not found. Creating new..." );
        jsonClients[ clientIdStr ] = {
            { "name", client.GetName() }
        };
    }
    std::ofstream wFile( dbFile_ );
    if ( !wFile.is_open() )
    {
        ERROR_LOG( "Failed to open data base file for write: " << dbFile_ );
        return false;
    }

    wFile << jsonClients.dump( 4 );
    wFile.close();
    return true;
}

bool JsonRepository::DeleteClient( ClientId id )
{
    std::ifstream file( dbFile_ );
    if ( !file.is_open() )
    {
        ERROR_LOG( "Failed to open data base file: " << dbFile_ );
        return false;
    }

    try
    {
        nlohmann::json jsonClients;
        file >> jsonClients;
        file.close();
        
        std::string clientIdStr = std::to_string( id );

        if ( !jsonClients.contains( clientIdStr ) )
        {
            INFO_LOG( "Client " << id << " not found for deletion" );
            return false; 
        }

        jsonClients.erase( clientIdStr );
        INFO_LOG( "Client: " << id << " was deleted successfully" );

        std::ofstream wFile( dbFile_ );
        if ( !wFile.is_open() )
        {
            ERROR_LOG( "Failed to open data base file for write: " << dbFile_ );
            return false;
        }
        wFile << jsonClients.dump( 4 );
        wFile.close();
        return true;
    }
    catch ( const nlohmann::json::exception& e )
    {
        ERROR_LOG( "Failed to remove client: " << e.what() );
        return false;
    }
}
