#include <memory>
#include <boost/asio.hpp>
#include "../broker/message_broker.h"

using SessionId = uint64_t;

class Session
{
public:
    Session( boost::asio::ip::tcp::socket socket );

    SessionId GetId() const;

private:

    void Read();

    void Send( std::string& msg );

    boost::asio::ip::tcp::socket socket_;
    SessionId id_;
    MessageQueue outgoing_;
};

using SessionPtr = std::shared_ptr< Session >;