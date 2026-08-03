#include "../fwd.h"
#include "rc.h"

namespace broker 
{

class MessageBroker
{
public:

    MessageBroker( ClientManagerPtr cliManager, TopicManagerPtr topManager );

    ReturnCodes Login( SessionPtr session, std::string& name );

    ReturnCodes Register( SessionPtr session, std::string& name );

    ReturnCodes Publish( Topic& topic, std::string& name, std::string& message );

    ReturnCodes Subscribe( Topic& topic, std::string& name );

private:
    ClientManagerPtr cliManager_;
    TopicManagerPtr topManager_;
};

} // namespace broker