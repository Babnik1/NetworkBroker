#pragma once
/// @file client_manager.h
///
/// Менеджер клиентов. Объявление.
///


#include <memory>
#include <unordered_map>
#include "client.h"
#include "../fwd.h"
#include "rc.h"


class ClientManager
{
public:
    ClientManager( IClientRepositoryPtr db );

    ClientsCodes CreateClient( const std::string& name );

    ClientsCodes ConnectClient( const std::string& name, SessionId id, SessionWeakPtr session );

    void DisconnectClient( ClientId id );

    ClientsCodes RemoveClient( const ClientId id );

    ClientId GetClientId( SessionId id );

private:
    std::unordered_map< ClientId, Client > clients_;
    IClientRepositoryPtr db_;

    void LoadClients();
};
