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
#include "../fwd.h"
#include "../db/i_topic_repository.h"


/// @brief Класс топик-менеджера.
class TopicManager
{
public:

    /// @brief Конструктор.
    /// @param[in] db Путь к файлу БД.
    TopicManager( ITopicRepositoryPtr db );

    /// @brief Опубликовать сообщение в тему.
    /// @param[in] topic Название темы.
    /// @param[in] message Сообщение.
    /// @param[out] clients Клиенты, принадлежащие конкретному топику.
    /// @return Код возврата. 0 - Упех.
    TopicCodes Publish( const Topic& topic, const std::string& message, std::unordered_set< ClientId >& clients );

    /// @brief Подписаться на тему.
    /// @param[in] id ID клиента.
    /// @param[in] topic Название темы.
    /// @return Код возврата. 0 - Упех.
    TopicCodes Subscribe( const ClientId id, const Topic& topic );

    /// @brief Создать тему.
    /// @param[in] id ID клиента.
    /// @param[in] topic Название темы.
    /// @return Код возврата. 0 - Упех.
    TopicCodes Create( const ClientId id, const Topic& topic );

private:
    ITopicRepositoryPtr db_;                                                /// Указатель на БД.
    std::unordered_map< Topic, std::unordered_set< ClientId > > topics_;    /// Мапа топиков и ее клиентов.

    /// @brief Загрузка топиков из БД.
    void LoadTopics();

};

using TopicManagerPtr = std::shared_ptr< TopicManager >;