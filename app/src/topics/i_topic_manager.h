#pragma once

/// @file i_topic_manager.h
///
/// Интерфейс топик-менеджера. Объявление.
///

#include "fwd.h"
#include "rc.h"
#include <unordered_set>


/// @brief Абстрактный класс топик-менеджера.
class ITopicManager
{
public:

    /// @brief Коструктор.
    virtual ~ITopicManager() = default;

    /// @brief Опубликовать сообщение в тему.
    /// @param[in] topic Название темы.
    /// @param[in] message Сообщение.
    /// @param[out] clients Клиенты, принадлежащие конкретному топику.
    /// @return Код возврата. 0 - Упех.
    virtual TopicCodes Publish( const Topic& topic, const std::string& message, std::unordered_set< ClientId >& clients ) = 0;

    /// @brief Подписаться на тему.
    /// @param[in] id ID клиента.
    /// @param[in] topic Название темы.
    /// @return Код возврата. 0 - Упех.
    virtual TopicCodes Subscribe( ClientId id, const Topic& topic ) = 0;

    /// @brief Создать тему.
    /// @param[in] id ID клиента.
    /// @param[in] topic Название темы.
    /// @return Код возврата. 0 - Упех.
    virtual TopicCodes Create( ClientId id, const Topic& topic) = 0;

    /// @brief Отписаться от темы.
    /// @param[in] id ID клиента.
    /// @param[in] topic Название темы.
    /// @return Код возврата. 0 - Упех.
    virtual TopicCodes Unsubscribe( ClientId id, const Topic& topic ) = 0;
};