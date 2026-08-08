


#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include "rc.h"
#include "../fwd.h"
#include "../db/i_topic_repository.h"

class TopicManager
{
public:

    TopicManager( ITopicRepositoryPtr db );

    TopicCodes Publish( ClientId id, Topic topic, std::string message );

    TopicCodes Subscribe( ClientId id, Topic topic );

private:
    ITopicRepositoryPtr db_; 
    std::unordered_map< Topic, std::unordered_set< ClientId > > topics_;
};

using TopicManagerPtr = std::shared_ptr< TopicManager >;