#include "i_client_repository.h"
#include <memory>
#include <string>


class JsonRepository : public IClientRepository
{
public:
    JsonRepository( std::string dbFile );

    virtual std::vector< Client > LoadClients() override;

    virtual void SaveClient( const Client& client ) override;

    virtual void DeleteClient( ClientId id ) override;
private:
    std::string dbFile_;
};