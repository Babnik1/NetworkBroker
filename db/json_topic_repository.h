#pragma once
/// @file json_topic_repository.h
///
/// База данных топиков в JSON. Объявление.
///


#include "i_topic_repository.h"
#include <string>


/// @brief Класс базы данных топиков.
class JsonTopicRepository : public ITopicRepository
{
public:

    /// @brief Конструткор.
    /// @param[in] dbFile Путь к файлу БД.
    JsonTopicRepository( const std::string dbFile );

    /// @copydoc ITopicRepository::SaveTopic
    virtual bool SaveTopic( Topic& topic, ClientId id );

    /// @copydoc ITopicRepository::LoadTopics
    virtual std::unordered_map< Topic, std::unordered_set< ClientId > > LoadTopics();

    /// @copydoc ITopicRepository::CreateTopic
    virtual bool CreateTopic( Topic& topic, ClientId id );

private:

    std::string dbFile_;    /// Путь к файлу БД.
};