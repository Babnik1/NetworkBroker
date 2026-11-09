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
#include "i_client_manager.h"
#include "passwd/i_password_verifier.h"


/// @brief Класс клиент-менеджера.
class ClientManager : public IClientManager
{
public:

    /// @brief Конструктор.
    /// @param[in] db Указатель на базу данных.
    ClientManager( IClientRepositoryPtr db );

    /// @copydoc IClientManager::CreateClient
    ClientsCodes CreateClient( const std::string& name, const std::string& passwd );

    /// @copydoc IClientManager::ConnectClient
    ClientsCodes ConnectClient( const std::string& name, SessionId id, const std::string& passwd, SessionWeakPtr session );

    /// @copydoc IClientManager::DisconnectClient
    void DisconnectClient( SessionId id );

    /// @copydoc IClientManager::GetClientId
    ClientsCodes GetClientId( SessionId id, ClientId& clientId );

    /// @copydoc IClientManager::SendTopicMessage
    void SendTopicMessage( const std::string& message, std::unordered_set< ClientId >& clients );

    /// @copydoc IClientManager::UnloginClient
    ClientsCodes UnloginClient( const ClientId clientId );

    /// @brief Удаление клиента.
    /// @details Не используется. Сделано на будущее.
    /// @param[in] id ID Клиента.
    /// @return Код возврата. Ok - Успех.
    ClientsCodes RemoveClient( const ClientId id );

private:

    std::unordered_map< ClientId, Client > clients_;    /// Список клиентов и их ID.
    IClientRepositoryPtr db_;                           /// Указатель на базу данных.
    IPasswordVerifierPtr passVer_;                      /// Указатель на интерфейс обработчика паролей.

    /// @brief Загрузка клиентов из БД.
    void LoadClients();
};
