


#include <memory>
class ITopicRepository
{
public:

    ITopicRepository() = default;

    virtual bool SaveTopic() = 0;

    virtual bool CreateTopic() = 0;
};

using ITopicRepositoryPtr = std::unique_ptr< ITopicRepository >;