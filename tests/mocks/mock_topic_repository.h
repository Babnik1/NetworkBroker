#pragma once

#include "db/i_topic_repository.h"
#include <string>

class MockTopicRepository : public ITopicRepository
{
public:
    bool SaveTopic( const Topic& topic, const ClientId id ) override
    {
        if ( !saveResult_ )
        {
            return false;
        }

        topics_[ topic ].insert( id );
        return true;
    }

    std::unordered_map< Topic, std::unordered_set< ClientId > >
    LoadTopics() override
    {
        return topics_;
    }

    bool CreateTopic( const Topic& topic, const ClientId id ) override
    {
        if ( !createResult_ )
        {
            return false;
        }

        topics_[ topic ].insert( id );
        return true;
    }

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

    void SetSaveResult( bool result )
    {
        saveResult_ = result;
    }

    void SetCreateResult( bool result )
    {
        createResult_ = result;
    }

    void SetRemoveResult( bool result )
    {
        removeResult_ = result;
    }

    void AddTopic( const Topic& topic, const std::unordered_set< ClientId >& clients )
    {
        topics_[ topic ] = clients;
    }

private:
    std::unordered_map< Topic, std::unordered_set< ClientId > > topics_;

    bool saveResult_ = true;
    bool createResult_ = true;
    bool removeResult_ = true;
};