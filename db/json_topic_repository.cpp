/// @file json_topic_repository.cpp
///
/// База данных топиков в JSON. Реализация.
///



#include "json_topic_repository.h"
#include <algorithm>
#include <nlohmann/json.hpp>
#include <fstream>
#include <exception>
#include <string>
#include <unordered_set>
#include "../logs/log.h"

JsonTopicRepository::JsonTopicRepository( const std::string dbFile_ )
    : dbFile_{ dbFile_ }
{}

bool JsonTopicRepository::SaveTopic( Topic& topic, ClientId id )
{
    nlohmann::json jsonTopics;
    std::ifstream file( dbFile_ );
    if ( !file.is_open() )
    {
        ERROR_LOG( "Failed to open data base file for read: " << dbFile_ );
        return false;
    }
    try
    {
        file >> jsonTopics;
        file.close();
    }
    catch ( const nlohmann::json::exception& e )
    {
        ERROR_LOG( "Failed to parse database file: " << e.what() );
        return false;
    }

    if ( !jsonTopics.contains( topic ) )
    {
        ERROR_LOG( "Topic " << topic << " is not found." );
        return false;
    }
    auto& clients = jsonTopics[ topic ][ "clients" ];
    if ( std::find( clients.begin(), clients.end(), id ) != clients.end() )
    {
        ERROR_LOG( "Client " << id << " already subscribed on " << topic );
        return false;
    }
    clients.push_back( id );
    INFO_LOG( "Topic " << topic << " updated successfully, added: " << id );

    std::ofstream wFile( dbFile_ );
    if ( !wFile.is_open() )
    {
        ERROR_LOG( "Failed to open data base file for write: " << dbFile_ );
        return false;
    }

    wFile << jsonTopics.dump( 4 );
    wFile.close();
    return true;
}

bool JsonTopicRepository::CreateTopic( Topic& topic, ClientId id )
{
    nlohmann::json jsonTopics;
    std::ifstream file( dbFile_ );
    if ( file.is_open() )
    {
        try
        {
            file >> jsonTopics;
            file.close();
        }
        catch ( const nlohmann::json::exception& e )
        {
            ERROR_LOG( "Failed to parse database file: " << e.what() );
            return false;
        }
    }
    else
    {
        jsonTopics = nlohmann::json::object();
    }

    if ( jsonTopics.contains( topic ) )
    {
        ERROR_LOG( "Topic " << topic << " already exists." );
        return false;
    }
    
    jsonTopics[ topic ] = { { "clients", nlohmann::json::array() } };
    jsonTopics[ topic ][ "clients" ].push_back( id );
    
    INFO_LOG( "Created new topic: " << topic );

    std::ofstream wFile( dbFile_ );
    if ( !wFile.is_open() )
    {
        ERROR_LOG( "Failed to open database file for write: " << dbFile_ );
        return false;
    }

    wFile << jsonTopics.dump( 4 );
    wFile.close();
    return true;
}

std::unordered_map< Topic, std::unordered_set< ClientId > > JsonTopicRepository::LoadTopics()
{
    std::unordered_map< Topic, std::unordered_set< ClientId > > topics;
    nlohmann::json jsonTopics;
    std::ifstream file( dbFile_ );
    if ( !file.is_open() )
    {
        ERROR_LOG( "Failed to open data base file for read: " << dbFile_ );
        return {};
    }
    try
    {
        file >> jsonTopics;
        file.close();
        
        for ( const auto& [ key, value ] : jsonTopics.items() )
        {
            std::unordered_set< ClientId > clientSet;
            if ( value.contains( "clients" ) && value[ "clients" ].is_array() )
            {
                for ( const auto& client : value[ "clients" ] )
                {
                    clientSet.insert( client.get< ClientId >() );
                }
            }
            topics[ key ] = std::move( clientSet );
        }
    }
    catch ( const std::exception& e )
    {
        ERROR_LOG( "Failed to parse topics: " << e.what() );
        return {};
    }
    return topics;
}