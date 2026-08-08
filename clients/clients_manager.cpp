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
    return ClientsCodes::Ok;
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
    if ( it->second.GetId() )
    {
        it->second.Disconnect();
    }
    db_->DeleteClient( it->second.GetId() );
    clients_.erase( it );
    return ClientsCodes::Ok;
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
            if ( client.GetSessionId() != 0 )
            {
                ERROR_ALL( "Client " << name << " already connected" );
                return ClientsCodes::ClientAlreadyConnected;
            }
            client.SetSession( session );
            INFO_ALL( "Client ID: " << client.GetId() <<", name: " << name << " authorized successfully" );
            return ClientsCodes::Ok;
        }
    }
    WARNING_ALL( "Failed to authorize client: " << name << ". Client is not found." );
    return ClientsCodes::ClientNotFound;
}

void ClientManager::DisconnectClient( SessionId id )
{
    for ( auto& client : clients_ )
    {
        if ( client.second.GetSessionId() == id )
        {
            client.second.Disconnect();
            INFO_ALL( "Client ID: " << client.second.GetId() <<", name: " << client.second.GetName() << " disconnected successfully" );
            return;
        }
    }
    ERROR_ALL( "Failed to disconnect session: " << id );
}

ClientsCodes ClientManager::GetClientId( SessionId id, ClientId& clientId )
{
    for ( const auto& client : clients_ )
    {
        if ( client.second.GetSessionId() == id )
        {
            clientId = client.second.GetId();
            return ClientsCodes::Ok;
        }
    }
    return ClientsCodes::ClientNotFound;
}

void ClientManager::SendTopicMessage( const std::string& message, std::unordered_set< ClientId >& clients )
{
    for ( const auto& id : clients )
    {
        auto it = clients_.find( id );
        if ( it != clients_.end() )
        {
            if ( it->second.GetSessionId() != invalidSessionId )
            {
                DEBUG_LOG( "Message for client " << it->second.GetName() << ", ID: " << it->second.GetId() << " was sent" );
                it->second.SendTopicMessage( message );
            }
            else 
            {
                DEBUG_LOG( "Client " << it->second.GetName() << ", ID: " << it->second.GetId() << " is not authorize. Message skipped" );
            }
        }
    }
}
