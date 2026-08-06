/// @file client_manager.cpp
///
/// Менеджер клиентов. Реализация.
///


#include "clients_manager.h"
#include "../db/i_client_repository.h"
#include "../logs/log.h"
#include <cstddef>
#include <cstdint>
#include <memory>
#include <random>
#include <vector>


ClientManager::ClientManager( IClientRepositoryPtr db )
    : db_{ std::move( db ) }
{
    LoadClients();
}

ClientsCodes ClientManager::CreateClient( const std::string& name )
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


/// @todo В момент удаления клиента. База данных может записывать по аптайму.
ClientsCodes ClientManager::RemoveClient( ClientId id )
{
    auto it = clients_.find( id );
    if ( it == clients_.end() )
    {
        ERROR_ALL( "Failed to remove client: " << id );
        return ClientsCodes::ClientNotFound;
    }
    if ( it->second.sesId )
    {
        it->second.Disconnect();
    }
    db_->DeleteClient( it->second.GetId() );
    clients_.erase( it );
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

ClientsCodes ClientManager::ConnectClient( const std::string& name, SessionId id, SessionWeakPtr session )
{
    for( auto& [ id, client ] : clients_ )
    {
        if ( client.GetName() == name )
        {
            client.sesId = id;
            client.session = session;
            return ClientsCodes::Ok;
        }
    }
    return ClientsCodes::ClientNotFound;
}

void ClientManager::DisconnectClient( ClientId id )
{
    auto it = clients_.find( id );
    if ( it == clients_.end() )
    {
        ERROR_ALL( "Failed to disconnect client: " << id << ", not found" );
        return;
    }
    it->second.sesId = std::nullopt;
}
