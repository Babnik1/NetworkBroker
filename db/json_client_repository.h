#pragma once
/// @file json_client_repository.h
///
/// База данных в JSON. Объявление.
///


#include "i_client_repository.h"
#include <memory>
#include <string>


/// @brief Класс базы данных, хранящейся в JSON.
class JsonRepository : public IClientRepository
{
public:
    /// @brief Конструктор.
    /// @param[in] dbFile Путь к базе данных.
    JsonRepository( const std::string dbFile );

    /// @copydoc IClientRepository::LoadClients
    virtual std::vector< Client > LoadClients() override;

    /// @copydoc IClientRepository::SaveClient
    virtual bool SaveClient( const Client& client ) override;

    /// @copydoc IClientRepository::DeleteClient
    virtual bool DeleteClient( ClientId id ) override;

private:
    std::string dbFile_; /// Путь к базе данных.
};