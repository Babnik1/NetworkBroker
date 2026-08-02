#include <memory>
#include <unordered_map>
#include "client.h"
#include "../db/i_client_repository.h"



class ClientManager
{
public:
    ClientManager( IClientRepositoryPtr db );

    void CreateClient( std::string name );

    ClientId ConnectClient( std::string name, SessionPtr session  );

    void DisconnectClient( ClientId id );

    void RemoveClient( ClientId id );

private:
    std::unordered_map< ClientId, Client > clients_;
    IClientRepositoryPtr db_;

    void LoadClients();
};

using ClientManagerPtr = std::unique_ptr< ClientManager >;