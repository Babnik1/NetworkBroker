


#include <memory>
#include <string>
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
    std::string db_; 
};

using TopicManagerPtr = std::shared_ptr< TopicManager >;