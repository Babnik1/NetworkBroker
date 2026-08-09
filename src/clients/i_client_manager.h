#pragma once 

/// @file i_client_manager.h
///
/// Интерфейс клиент-менеджера. Объявление.
///

#include "rc.h"
#include "fwd.h"
#include "unordered_set"


/// @brief Абстрактный класс клиент-менеджера.
class IClientManager
{
public:

    /// @brief Деструктор.
    virtual ~IClientManager() = default;

    /// @brief Создание клиента.
    /// @param[in] name Имя клиента.
    /// @return Код возврата. Ok - Успех.
    virtual ClientsCodes CreateClient( const std::string& name ) = 0;

    /// @brief Подсключение клиента.
    /// @param[in] name Имя клиента.
    /// @param[in] id ID сессии.
    /// @param[in] session Слабый указатель на сессию.
    /// @return Код возврата. Ok - Успех.
    virtual ClientsCodes ConnectClient( const std::string& name, SessionId id, SessionWeakPtr session ) = 0;

    /// @brief Получение ID клиента.
    /// @param[in] id ID сессии.
    /// @param[out] clientId ID клиента.
    /// @return Код возврата. Ok - Успех.
    virtual ClientsCodes GetClientId( SessionId id, ClientId& clientId ) = 0;

    /// @brief Удаление клиента.
    /// @details Не используется. Сделано на будущее.
    /// @param[in] id ID Клиента.
    /// @return Код возврата. Ok - Успех.
    virtual void DisconnectClient( SessionId id ) = 0;

    /// @brief Отправка сообщений клиентам.
    /// @param[in] message Сообщение.
    /// @param[in] clients Список клиентов.
    virtual void SendTopicMessage( const std::string& message, std::unordered_set< ClientId >& clients ) = 0;
};