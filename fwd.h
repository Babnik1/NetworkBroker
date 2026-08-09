#pragma once

/// @file fwd.h
///
/// Форвардная декларация.
///


#include <memory>
#include <cstdint>

using ClientId = uint64_t;
using Topic = std::string;
using SessionId = uint64_t;

class Session;
class MessageBroker;
class ClientManager;
class TopicManager;
class IClientRepository;
class JsonRepository;
class Server;
class ITopicRepository;
class IClientManager;

using TopicManagerPtr = std::shared_ptr< TopicManager >;

using ClientManagerPtr = std::shared_ptr< ClientManager >;

using IClientRepositoryPtr = std::unique_ptr< IClientRepository >;

using ITopicRepositoryPtr = std::unique_ptr< ITopicRepository >;

using JsonRepositoryPtr = std::unique_ptr< JsonRepository >;

using SessionPtr = std::shared_ptr< Session >;

using SessionWeakPtr = std::weak_ptr< Session >;

using MessageBrokerPtr = std::shared_ptr< MessageBroker >;

using IClientManagerPtr = std::shared_ptr< IClientManager >;