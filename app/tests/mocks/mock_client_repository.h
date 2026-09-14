#pragma once
/// @file mock_client_repository.cpp
///
/// Мок JSON репозитория клиентов.
///

#include "db/i_client_repository.h"


/// @brief Мокнутый класс JSON репозитория клиентов.
class MockClientRepository : public IClientRepository
{
public:

    /// @copydoc IClientRepository::LoadClients
    std::vector< Client > LoadClients() override
    {
        return clients_;
    }

    /// @copydoc IClientRepository::SaveClient
    bool SaveClient( const Client& client ) override
    {
        if ( !saveResult_ )
        {
            return false;
        }

        clients_.push_back( client );
        return true;
    }

    /// @copydoc IClientRepository::DeleteClient
    bool DeleteClient( ClientId id ) override
    {
        if ( !deleteResult_ )
        {
            return false;
        }

        auto it = std::remove_if(
            clients_.begin(),
            clients_.end(),
            [ id ]( const Client& client )
            {
                return client.GetId() == id;
            } );

        if ( it == clients_.end() )
        {
            return false;
        }

        clients_.erase( it, clients_.end() );
        return true;
    }

    /// @brief Установка возврата функции.
    /// @param[in] result Возврат.
    void SetSaveResult( bool result )
    {
        saveResult_ = result;
    }

    /// @brief Установка возврата функции.
    /// @param[in] result Возврат.
    void SetDeleteResult( bool result )
    {
        deleteResult_ = result;
    }

    /// @brief Добавление клиента.
    /// @param[in] client Клиент.
    void AddClient( const Client& client )
    {
        clients_.push_back( client );
    }

private:

    /// @brief Тестовые данные.
    std::vector< Client > clients_;
    bool saveResult_ = true;
    bool deleteResult_ = true;
};