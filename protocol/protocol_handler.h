#include "../fwd.h"
#include "string"

enum class CommandType
{
    Login = 0,
    Register,
    Publish,
    Subscribe
};


class ProtocolHandler
{
public:
    ProtocolHandler( MessageBrokerPtr broker );

    void Handle( SessionPtr session, const std::string& message );
private:
    MessageBrokerPtr broker_;
};
