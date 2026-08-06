#pragma once
/// @file client.h
///
/// Клиент.
///


#include "../server/session.h"
#include <memory>
#include <optional>
#include <unordered_set>

using ClientId = uint64_t;  /// ID клиента.

/// @brief Стуктура клиента.
struct Client
{
    /// @brief Конструктор.
    /// @param[in] id ID клиента.
    /// @param[in] name Имя клиента.
    Client( ClientId id, std::string name );

    ClientId GetId() const;

    std::string GetName();

    void Disconnect();

    ClientId id;
    std::unordered_set< Topic > subscriptions;
    std::optional< SessionId > sesId;
    SessionWeakPtr session; 
};


