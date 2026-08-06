#include "protocol_handler.h"
#include "../broker/message_broker.h"

namespace protocol
{

void ProtocolHandler::Handle( SessionPtr session, const std::string& message )
{
    auto command = parser_.Parse( message );

    switch ( command.type )
    {
        case CommandType::Login:
        {
            broker_->Login( session, command.name );
            break;
        }
        case CommandType::Register:
        {
            broker_->Register( session, command.name );
            break;
        }
        case CommandType::Publish:
        {
            broker_->Publish( session,
                             command.topic,
                             command.payload );
            break;
        }
        case CommandType::Subscribe:
        {
            broker_->Subscribe( session,
                                command.topic,
                                command.playload );
            break;
        }
    }
}

} // namespace protocol