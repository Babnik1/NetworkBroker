#pragma once
/// @file client_manager.h
///
/// Менеджер клиентов. Объявление.
///


#include <memory>
#include <unordered_map>
#include "client.h"
#include "fwd.h"
#include "rc.h"


class ClientManager
{
public:
    ClientManager( IClientRepositoryPtr db );

    ClientsCodes CreateClient( const std::string& name );

    ClientsCodes ConnectClient( const std::string& name, SessionId id, SessionWeakPtr session );

    void DisconnectClient( SessionId id );

    ClientsCodes RemoveClient( const ClientId id );

    /// @brief Получение ID клиента.
    /// @param[in] id
    /// @param[out] clientId ID клиента.
    /// @return Код возврата. Ok - Успех.
    ClientsCodes GetClientId( SessionId id, ClientId& clientId );

    void SendTopicMessage( const std::string& message, std::unordered_set< ClientId >& clients );

private:
    std::unordered_map< ClientId, Client > clients_;
    IClientRepositoryPtr db_;

    void LoadClients();
};
