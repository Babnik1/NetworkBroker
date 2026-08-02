#include "clients_manager.h"
#include "../logs/log.h"
#include <cstddef>
#include <cstdint>
#include <memory>
#include <random>
#include <vector>

ClientManager::ClientManager( IClientRepositoryPtr db )
    : db_{ db }
{
    LoadClients();
}

void ClientManager::CreateClient( std::string name )
{
    std::random_device rd;
    std::mt19937 gen( rd() );
    std::uniform_int_distribution< int > distrib( 10000, 100000 );

    bool complete = false;
    int clientId = -1;
    do 
    {
        clientId = distrib( gen );
        auto it = clients_.find( clientId );
        if ( it == clients_.end() )
        {
            complete = true;
        }
    }
    while ( !complete );

    Client client{ static_cast< uint64_t >( clientId ), name };

    clients_.emplace( clientId, client );
    db_->SaveClient( client ); 
}

void ClientManager::RemoveClient( ClientId id )
{
    auto it = clients_.find( id );
    if ( it == clients_.end() )
    {
        ERROR_ALL( "Failed to remove client: " << id );
        return;
    }
    it->second->Stop();
    clients_.erase( it );
    db_->DeleteClient( client );
}

void ClientManager::LoadClients()
{
    clients_.clear();
    std::vector< Client > clients = db_->LoadClients();
    for ( const auto& client : clients )
    {
        clients_.emplace( client.GetId(), client );
    }
}

ClientId ClientManager::ConnectClient( std::string name, SessionPtr session )
{
    for( auto& [ id, client ] : clients_ )
    {
        if ( client.GetName() == name )
        {
            client.session = session;
            return client.GetId();
        }
    }
    return -1; // not found
}

void ClientManager::DisconnectClient( ClientId id )
{
    auto it = clients_.find( id );
    if ( it == clients_.end() )
    {
        ERROR_ALL( "Failed to disconnect client: " << id << ", not found" );
        return;
    }
    it->second.session = nullptr;
}