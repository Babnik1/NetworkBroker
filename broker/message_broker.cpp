#include "message_broker.h"
#include "../clients//clients_manager.h"

namespace broker
{

MessageBroker::MessageBroker( ClientManagerPtr cliManager, TopicManagerPtr topManager )
    : cliManager_{ cliManager }
    , topManager_{ topManager }
{}

ReturnCodes MessageBroker::Login( SessionPtr session, std::string& name )
{
    if ( cliManager_->ConnectClient( name , session ) == -1 )
    {
        return ReturnCodes::ClientNotFound;
    }
    else 
    {
        return ReturnCodes::Ok;
    }
}

ReturnCodes MessageBroker::Register( SessionPtr session, std::string& name )
{
    if ( cliManager_->CreateClient( name ) == -1 )
    {

    }
}

ReturnCodes MessageBroker::Publish( Topic& topic, std::string& name, std::string& message )
{

}

ReturnCodes MessageBroker::Subscribe( Topic& topic, std::string& name )
{

}

} // namespace broker