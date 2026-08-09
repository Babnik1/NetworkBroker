#pragma once

/// @file client.h
///
/// Клиент. Объявление.
///


#include "server/session.h"
#include "server/session.h"
#include <cstdint>
#include <memory>
#include <optional>
#include <unordered_set>


using ClientId = uint64_t;    /// ID клиента.
inline uint64_t invalidClientId = 0; /// Невалидный ID клиента.


/// @brief Стуктура клиента.
class Client
{
public:

    /// @brief Конструктор.
    /// @param[in] id ID клиента.
    /// @param[in] name Имя клиента.
    Client( ClientId id, std::string name );

    /// @brief Получение ID клиента.
    /// @return ID клиента.
    ClientId GetId() const;

    /// @brief Получение имени клиента.
    /// @return Имя клиента.
    std::string GetName() const;

    /// @brief Получение ID сессии клиента.
    /// @return ID сессии клиента.
    SessionId GetSessionId() const;

    /// @brief Уставнока сессии клиенту.
    /// @param[in] session Слабая ссылка на сессию.
    void SetSession( SessionWeakPtr session );

    /// @brief Сообщение клиенту, что он деавторизован.
    void Disconnect();

    /// @brief Сигнал отправки сообщения клиенту.
    /// @param[in] message Сообщение.
    void SendTopicMessage( const std::string message );

private:

    ClientId id_;               /// ID Клиента.
    std::string name_;          /// Имя клиента.
    SessionWeakPtr session_;    /// Слабая ссылка на сессию.
};


