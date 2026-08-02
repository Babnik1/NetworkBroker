#include "i_client_repository.h"
#include <memory>
#include <string>


class JsonRepository : public IClientRepository
{
public:
    JsonRepository( std::string dbFile );

    virtual std::vector< Client > LoadClients();

    virtual void SaveClient( const Client& client );

    virtual void DeleteClient( ClientId id );
private:
    std::string dbFile_;
};

using IClientRepositoryPtr = std::shared_ptr< IClientRepository >;