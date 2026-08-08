#pragma once
/// @file i_client_repisitory.h
///
/// Интерфейс базы данных клиентов.
///


#include <vector>
#include "../clients/client.h"

/// @brief Абстрактный класс базы данных клиентов.
class IClientRepository
{
public:
    /// @brief Конструктор.
    virtual ~IClientRepository() = default;

    /// @brief Загрузка клиентов из БД.
    /// @return Вектор клиентов.
    virtual std::vector< Client > LoadClients() = 0;

    /// @brief Сохранение клиента в БД.
    /// @details Умеет изменять клиентов и создавать новых.
    /// @param[in] client Клиент.
    /// @return false - неудача, true - успех.
    virtual bool SaveClient( const Client& client ) = 0;

    /// @brief Удаление клиента из БД.
    /// @param[in] id ID клиента.
    /// @return false - неудача, true - успех.
    virtual bool DeleteClient( ClientId id ) = 0;
};
