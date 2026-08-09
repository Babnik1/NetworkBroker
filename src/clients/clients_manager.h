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


/// @brief Класс клиент-менеджера.
class ClientManager
{
public:

    /// @brief Конструктор.
    /// @param[in] db Указатель на базу данных.
    ClientManager( IClientRepositoryPtr db );

    /// @brief Создание клиента.
    /// @param[in] name Имя клиента.
    /// @return Код возврата. Ok - Успех.
    ClientsCodes CreateClient( const std::string& name );

    /// @brief Подсключение клиента.
    /// @param[in] name Имя клиента.
    /// @param[in] id ID сессии.
    /// @param[in] session Слабый указатель на сессию.
    /// @return Код возврата. Ok - Успех.
    ClientsCodes ConnectClient( const std::string& name, SessionId id, SessionWeakPtr session );

    /// @brief Отключение клиента.
    /// @param[in] id ID сессии.
    void DisconnectClient( SessionId id );

    /// @brief Удаление клиента.
    /// @details Не используется. Сделано на будущее.
    /// @param[in] id ID Клиента.
    /// @return Код возврата. Ok - Успех.
    ClientsCodes RemoveClient( const ClientId id );

    /// @brief Получение ID клиента.
    /// @param[in] id ID сессии.
    /// @param[out] clientId ID клиента.
    /// @return Код возврата. Ok - Успех.
    ClientsCodes GetClientId( SessionId id, ClientId& clientId );

    /// @brief Отправка сообщений клиентам.
    /// @param[in] message Сообщение.
    /// @param[in] clients Список клиентов.
    void SendTopicMessage( const std::string& message, std::unordered_set< ClientId >& clients );

private:

    std::unordered_map< ClientId, Client > clients_;    /// Список клиентов и их ID.
    IClientRepositoryPtr db_;                           /// Указатель на базу данных.

    /// @brief Загрузка клиентов из БД.
    void LoadClients();
};
