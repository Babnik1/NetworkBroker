#include "session.h"
#include <memory>
#include <unordered_set>

using ClientId = uint64_t;

struct Client
{
    Client( ClientId id, std::string name );

    ClientId GetId() const;

    std::string GetName();

    ClientId id;
    std::unordered_set< Topic > subscriptions;
    SessionPtr session; 
};

