#include "i_topic_repository.h"
#include <string>

class JsonTopicRepository : public ITopicRepository
{
public:
    JsonTopicRepository( const std::string dbFile );

    virtual bool SaveTopic();

    virtual bool CreateTopic();

private:

    std::string dbFile_;
};