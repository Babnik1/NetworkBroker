#include <vector>
#include "../clients/client.h"

class IClientRepository
{
public:
    virtual ~IClientRepository() = default;

    virtual std::vector< Client > LoadClients() = 0;

    virtual bool SaveClient( const Client& client ) = 0;

    virtual bool DeleteClient( ClientId id ) = 0;
};
