/// @file topic_manager.cpp
///
/// Менеджер топиков. Реализация.
///


#include "topic_manager.h"
#include "logs/log.h"
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
    if ( !db_->SaveTopic( topic , id ) )
    {
        ERROR_LOG( "Failed to subscribe on topic: " << topic << ". ID: " << id );
        return TopicCodes::InternalError;
    }

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

    if ( !db_->CreateTopic( topic, id ) )
    {
        ERROR_LOG( "Failed to create topic: " << topic << ". ID: " << id );
        return TopicCodes::InternalError;
    }
    topics_[ topic ].insert( id );

    INFO_LOG( "Topic " << topic << " created successfully" );
    return TopicCodes::Ok;
}

TopicCodes TopicManager::Unsubscribe( const ClientId id, const Topic& topic )
{
    auto it = topics_.find( topic );
    if ( it != topics_.end() )
    {
        auto& cliSet = it->second;
        auto itCli = cliSet.find( id );

        if ( itCli == cliSet.end() )
        {
            ERROR_LOG( "Not subscribed. Topic: " << topic << ". ID: " << id );
            return TopicCodes::NotSubscribed;
        }   

        if ( !db_->RemoveFromTopic( topic , id ) )
        {
            ERROR_LOG( "Failed to delete from topic: " << topic << ". ID: " << id );
            return TopicCodes::InternalError;
        }

        cliSet.erase( itCli );
    }
    else 
    {
        ERROR_LOG( "Topic " << topic << " is not found" );
        return TopicCodes::TopicNotFound;
    }

    INFO_LOG( "Topic " << topic << " changed successfully" );
    return TopicCodes::Ok;
}