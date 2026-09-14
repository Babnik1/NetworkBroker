#pragma once
/// @file mock_client_manager.cpp
///
/// Мок клиент-менеджера.
///


#include <string>
#include "clients/i_client_manager.h"

/// @brief Мокнутый класс клиент-менеджера.
class MockClientManager : public IClientManager
{
public:

    /// @copydoc IClientManager::CreateClient
    ClientsCodes CreateClient( const std::string& name ) override
    {
        lastCreatedName = name;
        return createResult;
    }

    /// @copydoc IClientManager::ConnectClient
    ClientsCodes ConnectClient( const std::string& name, SessionId id, SessionWeakPtr session ) override
    {
        lastConnectedName = name;
        lastSessionId = id;
        return connectResult;
    }

    /// @copydoc IClientManager::DisconnectClient
    void DisconnectClient( SessionId id ) override
    {
        lastDisconnectedSessionId = id;
    }

    /// @copydoc IClientManager::GetCLientId
    ClientsCodes GetClientId( SessionId id, ClientId& clientId ) override
    {
        lastGetClientIdSessionId = id;

        if ( getClientIdResult == ClientsCodes::Ok )
        {
            clientId = clientIdToReturn;
        }

        return getClientIdResult;
    }

    /// @copydoc IClientManager::SendTopicMessage
    void SendTopicMessage( const std::string& message, std::unordered_set< ClientId >& clients ) override
    {
        lastMessage = message;
        lastClients = clients;
        sendMessageCalled = true;
    }

    ClientsCodes UnloginClient( ClientId clientId )
    {
        return ClientsCodes::Ok;
    }

    /// @brief Тестовые данные.
    ClientsCodes createResult = ClientsCodes::Ok;
    ClientsCodes connectResult = ClientsCodes::Ok;
    ClientsCodes getClientIdResult = ClientsCodes::Ok;

    ClientId clientIdToReturn = 123;

    std::string lastCreatedName;
    std::string lastConnectedName;

    SessionId lastSessionId = 0;
    SessionId lastDisconnectedSessionId = 0;
    SessionId lastGetClientIdSessionId = 0;

    std::string lastMessage;
    std::unordered_set< ClientId > lastClients;

    bool sendMessageCalled = false;
};