#include "topic_manager.h"


TopicManager::TopicManager( ITopicRepositoryPtr db )
    : db_{ std::move( db ) }
{}