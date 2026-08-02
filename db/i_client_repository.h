#include <vector>
#include "../clients/client.h"

class IClientRepository
{
public:
    virtual ~IClientRepository() = default;

    virtual std::vector< Client > LoadClients() = 0;

    virtual void SaveClient( const Client& client ) = 0;

    virtual void DeleteClient( ClientId id ) = 0;
};