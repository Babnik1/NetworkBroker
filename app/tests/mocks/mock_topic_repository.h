#pragma once
/// @file mock_topic_repository.cpp
///
/// Мок JSON репозитория топиков.
///


#include "db/i_topic_repository.h"
#include <string>


/// Мокнутый класс JSON репозитория топиков.
class MockTopicRepository : public ITopicRepository
{
public:

    /// @copydoc ITopicRepository::SaveTopic
    bool SaveTopic( const Topic& topic, const ClientId id ) override
    {
        if ( !saveResult_ )
        {
            return false;
        }

        topics_[ topic ].insert( id );
        return true;
    }

    /// @copydoc ITopicRepository::LoadTopics
    std::unordered_map< Topic, std::unordered_set< ClientId > >
    LoadTopics() override
    {
        return topics_;
    }

    /// @copydoc ITopicRepository::CreateTopic
    bool CreateTopic( const Topic& topic, const ClientId id ) override
    {
        if ( !createResult_ )
        {
            return false;
        }

        topics_[ topic ].insert( id );
        return true;
    }

    /// @copydoc ITopicRepository::RemoveFromTopic
    bool RemoveFromTopic( const Topic& topic, const ClientId id ) override
    {
        if ( !removeResult_ )
        {
            return false;
        }

        auto it = topics_.find( topic );

        if ( it == topics_.end() )
        {
            return false;
        }

        it->second.erase( id );
        return true;
    }

    /// @brief Установка кода возврата.
    /// @param[in] result Код возврата.
    void SetSaveResult( bool result )
    {
        saveResult_ = result;
    }

    /// @brief Установка кода возврата.
    /// @param[in] result Код возврата.
    void SetCreateResult( bool result )
    {
        createResult_ = result;
    }

    /// @brief Установка кода возврата.
    /// @param[in] result Код возврата.
    void SetRemoveResult( bool result )
    {
        removeResult_ = result;
    }

    /// @brief Добавлдение топика.
    /// @param[in] topic Топик.
    /// @param[in] clients Клиенты.
    void AddTopic( const Topic& topic, const std::unordered_set< ClientId >& clients )
    {
        topics_[ topic ] = clients;
    }

private:
    /// @brief Тестовые данные.
    std::unordered_map< Topic, std::unordered_set< ClientId > > topics_;

    bool saveResult_ = true;
    bool createResult_ = true;
    bool removeResult_ = true;
};