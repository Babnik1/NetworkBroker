#pragma once

/// @file topic_manager.h
///
/// Менеджер топиков. Объявление.
///


#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include "rc.h"
#include "fwd.h"
#include "db/i_topic_repository.h"
#include "topics/i_topic_manager.h"


/// @brief Класс топик-менеджера.
class TopicManager : public ITopicManager
{
public:

    /// @brief Конструктор.
    /// @param[in] db Путь к файлу БД.
    TopicManager( ITopicRepositoryPtr db );

    /// @copydoc ITopicManager::Publish
    TopicCodes Publish( const Topic& topic, const std::string& message, std::unordered_set< ClientId >& clients );

    /// @copydoc ITopicManager::Subscribe
    TopicCodes Subscribe( const ClientId id, const Topic& topic );

    /// @copydoc ITopicManager::Create
    TopicCodes Create( const ClientId id, const Topic& topic );

    /// @copydoc ITopicManager::Unsubscribe
    TopicCodes Unsubscribe( const ClientId id, const Topic& topic );

private:
    ITopicRepositoryPtr db_;                                                /// Указатель на БД.
    std::unordered_map< Topic, std::unordered_set< ClientId > > topics_;    /// Мапа топиков и ее клиентов.

    /// @brief Загрузка топиков из БД.
    void LoadTopics();

};

using TopicManagerPtr = std::shared_ptr< TopicManager >;