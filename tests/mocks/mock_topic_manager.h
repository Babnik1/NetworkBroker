#pragma once
/// @file mock_topic_manager.cpp
///
/// Мок топик-менеджера.
///


#include <string>
#include "topics/i_topic_manager.h"


/// @brief Мокнутый класс менеджера топиков.
class MockTopicManager : public ITopicManager
{
public:

    /// @copydoc ITopicManager::Publish
    TopicCodes Publish( const Topic& topic, const std::string& message, std::unordered_set< ClientId >& clients ) override
    {
        lastPublishTopic = topic;
        lastPublishMessage = message;

        clients = publishClients;

        publishCalled = true;

        return publishResult;
    }

    /// @copydoc ITopicManager::Subscribe
    TopicCodes Subscribe( ClientId id, const Topic& topic ) override
    {
        lastSubscribeClientId = id;
        lastSubscribeTopic = topic;

        subscribeCalled = true;

        return subscribeResult;
    }

    /// @copydoc ITopicManager::Create
    TopicCodes Create( ClientId id, const Topic& topic ) override
    {
        lastCreateClientId = id;
        lastCreateTopic = topic;

        createCalled = true;

        return createResult;
    }

    /// @copydoc ITopicManager::Unsubscribe
    TopicCodes Unsubscribe( ClientId id, const Topic& topic ) override
    {
        lastUnsubscribeClientId = id;
        lastUnsubscribeTopic = topic;

        unsubscribeCalled = true;

        return unsubscribeResult;
    }

    /// @brief Тестовые данные.
    TopicCodes publishResult = TopicCodes::Ok;
    TopicCodes subscribeResult = TopicCodes::Ok;
    TopicCodes createResult = TopicCodes::Ok;
    TopicCodes unsubscribeResult = TopicCodes::Ok;

    std::unordered_set< ClientId > publishClients;

    Topic lastPublishTopic;
    std::string lastPublishMessage;

    ClientId lastSubscribeClientId = 0;
    Topic lastSubscribeTopic;

    ClientId lastCreateClientId = 0;
    Topic lastCreateTopic;

    ClientId lastUnsubscribeClientId = 0;
    Topic lastUnsubscribeTopic;

    bool publishCalled = false;
    bool subscribeCalled = false;
    bool createCalled = false;
    bool unsubscribeCalled = false;
};
