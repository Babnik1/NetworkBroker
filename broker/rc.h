
namespace broker 
{

enum class ReturnCodes
{
    Ok,

    ClientAlreadyExists,
    ClientNotFound,
    ClientAlreadyConnected,

    TopicAlreadyExists,
    TopicNotFound,

    NotSubscribed,

    InvalidCommand,
    Unauthorized,

    InternalError
};

} // namespace broker