#include <memory>
#include <boost/asio.hpp>

using SessionId = uint64_t;

class Session
{
public:
    Session( SessionId id, boost::asio::ip::tcp::socket socket );

    void Start();

    void Stop();

    SessionId GetId() const;

private:

    void Read();

    void Write();

    boost::asio::ip::tcp::socket socket_;
    SessionId id_;
    MessageQueue outgoing_;

};

using SessionPtr = std::shared_ptr< Session >;