/// @file topic_manager.cpp
///
/// Менеджер топиков. Реализация.
///


#include "topic_manager.h"
#include "../logs/log.h"
#include <unordered_set>


TopicManager::TopicManager( ITopicRepositoryPtr db )
    : db_{ std::move( db ) }
{
    LoadTopics();
}

void TopicManager::LoadTopics()
{
    topics_.clear();
    topics_ = db_->LoadTopics();
}

TopicCodes TopicManager::Publish( const Topic& topic, const std::string& message, std::unordered_set< ClientId >& clients )
{
    auto it = topics_.find( topic );
    if ( it == topics_.end() )
    {
        ERROR_LOG( "Topic " << topic << " is not found" );
        return TopicCodes::TopicNotFound;
    }

    clients = topics_[ topic ];
    return TopicCodes::Ok;
}

TopicCodes TopicManager::Subscribe( const ClientId id, const Topic& topic )
{
    auto it = topics_.find( topic );
    if ( it == topics_.end() )
    {
        ERROR_LOG( "Topic " << topic << " is not found" );
        return TopicCodes::TopicNotFound;
    }

    it->second.insert( id );
    db_->SaveTopic( topic , id );

    INFO_LOG( "Topic " << topic << " changed successfully" );
    return TopicCodes::Ok;
}

TopicCodes TopicManager::Create( const ClientId id, const Topic& topic )
{
    auto it = topics_.find( topic );
    if ( it != topics_.end() )
    {
        ERROR_LOG( "Topic " << topic << " already exist" );
        return TopicCodes::TopicAlreadyExists;
    }

    topics_[ topic ].insert( id );
    db_->CreateTopic( topic, id );

    INFO_LOG( "Topic " << topic << " created successfully" );
    return TopicCodes::Ok;
}