#include <gtest/gtest.h>

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "broker/message_broker.h"
#include "clients/clients_manager.h"
#include "topics/topic_manager.h"
#include "db/i_client_repository.h"
#include "db/i_topic_repository.h"


class FakeClientRepository : public IClientRepository
{
public:
    std::vector<Client> LoadClients() override
    {
        return clients_;
    }

    bool SaveClient(const Client& client) override
    {
        for (auto& current : clients_)
        {
            if (current.GetId() == client.GetId())
            {
                current = client;
                return true;
            }
        }

        clients_.push_back(client);
        return true;
    }

    bool DeleteClient(ClientId id) override
    {
        for (auto it = clients_.begin(); it != clients_.end(); ++it)
        {
            if (it->GetId() == id)
            {
                clients_.erase(it);
                return true;
            }
        }

        return false;
    }

private:
    std::vector<Client> clients_;
};


class FakeTopicRepository : public ITopicRepository
{
public:
    bool SaveTopic(const Topic& topic, ClientId id) override
    {
        topics_[topic].insert(id);
        return true;
    }

    std::unordered_map<Topic, std::unordered_set<ClientId>>
    LoadTopics() override
    {
        return topics_;
    }

    bool CreateTopic(const Topic& topic, ClientId id) override
    {
        if (topics_.find(topic) != topics_.end())
        {
            return false;
        }

        topics_[topic].insert(id);
        return true;
    }

    bool RemoveFromTopic(const Topic& topic, ClientId id) override
    {
        auto it = topics_.find(topic);

        if (it == topics_.end())
        {
            return false;
        }

        it->second.erase(id);
        return true;
    }

private:
    std::unordered_map<Topic, std::unordered_set<ClientId>> topics_;
};


class MessageBrokerTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        auto clientRepository =
            std::make_unique<FakeClientRepository>();

        auto topicRepository =
            std::make_unique<FakeTopicRepository>();

        clientManager_ =
            std::make_unique<ClientManager>(
                std::move(clientRepository));

        topicManager_ =
            std::make_unique<TopicManager>(
                std::move(topicRepository));

        broker_ =
            std::make_unique<MessageBroker>(
                std::move(clientManager_),
                std::move(topicManager_));
    }

    std::unique_ptr<ClientManager> clientManager_;
    std::unique_ptr<TopicManager> topicManager_;
    std::unique_ptr<MessageBroker> broker_;
};