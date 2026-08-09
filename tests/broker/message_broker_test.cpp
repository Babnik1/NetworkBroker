#include <gtest/gtest.h>
#include "broker/message_broker.h"
#include "mocks/mock_client_manager.h"
#include "mocks/mock_topic_manager.h"


///
/// Тесты связанные с клиентом.
///


/// @brief Тест: Пустая комнада.
TEST( MessageBrokerTest, EmptyCommand )
{
    auto clientManager = std::make_unique< MockClientManager >();
    auto topicManager = std::make_unique< MockTopicManager >();

    MessageBroker broker(
        std::move( clientManager ),
        std::move( topicManager ) );

    EXPECT_EQ(
        broker.HandleCommand( 1, "", {} ),
        "ERROR Invalid command\n" );
}

/// @brief Тест: Неверная команда.
TEST( MessageBrokerTest, InvalidCommand )
{
    auto clientManager = std::make_unique< MockClientManager >();
    auto topicManager = std::make_unique< MockTopicManager >();

    MessageBroker broker(
        std::move( clientManager ),
        std::move( topicManager ) );

    EXPECT_EQ(
        broker.HandleCommand( 1, "FOO alex", {} ),
        "ERROR Invalid command\n" );
}

/// @brief Тест: Успешный логин.
TEST( MessageBrokerTest, Login )
{
    auto clientManager = std::make_unique< MockClientManager >();
    auto topicManager = std::make_unique< MockTopicManager >();

    auto* clientManagerPtr = clientManager.get();

    MessageBroker broker(
        std::move( clientManager ),
        std::move( topicManager ) );

    EXPECT_EQ(
        broker.HandleCommand( 42, "LOGIN alex", {} ),
        "OK\n" );

    EXPECT_EQ(
        clientManagerPtr->lastConnectedName,
        "alex" );

    EXPECT_EQ(
        clientManagerPtr->lastSessionId,
        42 );
}

/// @brief Тест: Неуспешный логин. Несуществующий клиент.
TEST( MessageBrokerTest, LoginClientNotFound )
{
    auto clientManager = std::make_unique< MockClientManager >();
    auto topicManager = std::make_unique< MockTopicManager >();

    clientManager->connectResult =
        ClientsCodes::ClientNotFound;

    MessageBroker broker(
        std::move( clientManager ),
        std::move( topicManager ) );

    EXPECT_EQ(
        broker.HandleCommand( 42, "LOGIN alex", {} ),
        "ERROR Client not found\n" );
}

/// @brief Тест: Неуспешный логин. Клиент уже подключен.
TEST( MessageBrokerTest, LoginAlreadyConnected )
{
    auto clientManager = std::make_unique< MockClientManager >();
    auto topicManager = std::make_unique< MockTopicManager >();

    clientManager->connectResult =
        ClientsCodes::ClientAlreadyConnected;

    MessageBroker broker(
        std::move( clientManager ),
        std::move( topicManager ) );

    EXPECT_EQ(
        broker.HandleCommand( 42, "LOGIN alex", {} ),
        "ERROR Client already connected\n" );
}

/// @brief Тест: Успешная регистрация.
TEST( MessageBrokerTest, Register )
{
    auto clientManager = std::make_unique< MockClientManager >();
    auto topicManager = std::make_unique< MockTopicManager >();

    auto* clientManagerPtr = clientManager.get();

    MessageBroker broker(
        std::move( clientManager ),
        std::move( topicManager ) );

    EXPECT_EQ(
        broker.HandleCommand( 42, "REGISTER alex", {} ),
        "OK\n" );

    EXPECT_EQ(
        clientManagerPtr->lastCreatedName,
        "alex" );

    EXPECT_EQ(
        clientManagerPtr->lastConnectedName,
        "alex" );

    EXPECT_EQ(
        clientManagerPtr->lastSessionId,
        42 );
}

/// @brief Тест: Неуспешная регистрация. Клиент уже существует.
TEST( MessageBrokerTest, RegisterExistingClient )
{
    auto clientManager = std::make_unique< MockClientManager >();
    auto topicManager = std::make_unique< MockTopicManager >();

    clientManager->createResult =
        ClientsCodes::ClientAlreadyExists;

    MessageBroker broker(
        std::move( clientManager ),
        std::move( topicManager ) );

    EXPECT_EQ(
        broker.HandleCommand( 42, "REGISTER alex", {} ),
        "ERROR Client already exists\n" );
}

/// @brief Тест:: Неуспешная регистрация.
TEST( MessageBrokerTest, RegisterInternalError )
{
    auto clientManager = std::make_unique< MockClientManager >();
    auto topicManager = std::make_unique< MockTopicManager >();

    clientManager->createResult =
        ClientsCodes::InternalError;

    MessageBroker broker(
        std::move( clientManager ),
        std::move( topicManager ) );

    EXPECT_EQ(
        broker.HandleCommand( 42, "REGISTER alex", {} ),
        "ERROR Internal error\n" );
}

/// @todo LOGOUT

///
/// Тесты связанные с топиками.
///

/// @brief Тест: Успешное создание топика.
TEST( MessageBrokerTest, CreateTopic )
{
    auto clientManager = std::make_unique< MockClientManager >();
    auto topicManager = std::make_unique< MockTopicManager >();

    auto* clientManagerPtr = clientManager.get();
    auto* topicManagerPtr = topicManager.get();

    clientManagerPtr->clientIdToReturn = 100;

    MessageBroker broker(
        std::move( clientManager ),
        std::move( topicManager ) );

    EXPECT_EQ(
        broker.HandleCommand( 42, "CREATE test", {} ),
        "OK\n" );

    EXPECT_TRUE(
        topicManagerPtr->createCalled );

    EXPECT_EQ(
        topicManagerPtr->lastCreateClientId,
        100 );

    EXPECT_EQ(
        topicManagerPtr->lastCreateTopic,
        "test" );
}

/// @brief Тест: Неуспешное создание топика. Клиент не авторизован.
TEST( MessageBrokerTest, CreateTopicUnauthorized )
{
    auto clientManager = std::make_unique< MockClientManager >();
    auto topicManager = std::make_unique< MockTopicManager >();

    clientManager->getClientIdResult =
        ClientsCodes::ClientNotFound;

    auto* topicManagerPtr = topicManager.get();

    MessageBroker broker(
        std::move( clientManager ),
        std::move( topicManager ) );

    EXPECT_EQ(
        broker.HandleCommand( 42, "CREATE test", {} ),
        "ERROR You are unauthorized\n" );

    EXPECT_FALSE(
        topicManagerPtr->createCalled );
}

/// @brief Тест: Успешная подписка на топик.
TEST( MessageBrokerTest, Subscribe )
{
    auto clientManager = std::make_unique< MockClientManager >();
    auto topicManager = std::make_unique< MockTopicManager >();

    clientManager->clientIdToReturn = 100;

    auto* topicManagerPtr = topicManager.get();

    MessageBroker broker(
        std::move( clientManager ),
        std::move( topicManager ) );

    EXPECT_EQ(
        broker.HandleCommand( 42, "SUBSCRIBE test", {} ),
        "OK\n" );

    EXPECT_TRUE(
        topicManagerPtr->subscribeCalled );

    EXPECT_EQ(
        topicManagerPtr->lastSubscribeClientId,
        100 );

    EXPECT_EQ(
        topicManagerPtr->lastSubscribeTopic,
        "test" );
}

/// @brief Тест: Неуспешная подписка на топик. Топика не существует.
TEST( MessageBrokerTest, SubscribeTopicNotFound )
{
    auto clientManager = std::make_unique< MockClientManager >();
    auto topicManager = std::make_unique< MockTopicManager >();

    topicManager->subscribeResult =
        TopicCodes::TopicNotFound;

    MessageBroker broker(
        std::move( clientManager ),
        std::move( topicManager ) );

    EXPECT_EQ(
        broker.HandleCommand( 42, "SUBSCRIBE test", {} ),
        "ERROR Topic not found\n" );
}

/// @todo Неуспешная подписка. Уже подписан.

/// @brief Тест: Успешная публикация сообщения.
TEST( MessageBrokerTest, Publish )
{
    auto clientManager = std::make_unique< MockClientManager >();
    auto topicManager = std::make_unique< MockTopicManager >();

    clientManager->clientIdToReturn = 100;

    topicManager->publishClients =
    {
        200,
        300
    };

    auto* clientManagerPtr = clientManager.get();
    auto* topicManagerPtr = topicManager.get();

    MessageBroker broker(
        std::move( clientManager ),
        std::move( topicManager ) );

    EXPECT_EQ(
        broker.HandleCommand(
            42,
            "PUBLISH test Hello world",
            {} ),
        "OK\n" );

    EXPECT_TRUE(
        topicManagerPtr->publishCalled );

    EXPECT_EQ(
        topicManagerPtr->lastPublishTopic,
        "test" );

    EXPECT_EQ(
        topicManagerPtr->lastPublishMessage,
        "Hello world" );

    EXPECT_TRUE(
        clientManagerPtr->sendMessageCalled );

    EXPECT_EQ(
        clientManagerPtr->lastMessage,
        "Hello world" );

    EXPECT_EQ(
        clientManagerPtr->lastClients,
        topicManagerPtr->publishClients );
}

/// @brief Тест: Неуспешная публикация сообщения. Клиент не авторизован.
TEST( MessageBrokerTest, PublishUnauthorized )
{
    auto clientManager = std::make_unique< MockClientManager >();
    auto topicManager = std::make_unique< MockTopicManager >();

    clientManager->getClientIdResult =
        ClientsCodes::ClientNotFound;

    auto* topicManagerPtr = topicManager.get();
    auto* clientManagerPtr = clientManager.get();

    MessageBroker broker(
        std::move( clientManager ),
        std::move( topicManager ) );

    EXPECT_EQ(
        broker.HandleCommand(
            42,
            "PUBLISH test Hello",
            {} ),
        "ERROR You are unauthorized\n" );

    EXPECT_FALSE(
        topicManagerPtr->publishCalled );

    EXPECT_FALSE(
        clientManagerPtr->sendMessageCalled );
}

/// @brief Тест: Неуспешная публикация сообщения. Топика не существует.
TEST( MessageBrokerTest, PublishTopicNotFound )
{
    auto clientManager = std::make_unique< MockClientManager >();
    auto topicManager = std::make_unique< MockTopicManager >();

    clientManager->clientIdToReturn = 100;
    topicManager->publishResult =
        TopicCodes::TopicNotFound;

    auto* clientManagerPtr = clientManager.get();
    auto* topicManagerPtr = topicManager.get();

    MessageBroker broker(
        std::move( clientManager ),
        std::move( topicManager ) );

    EXPECT_EQ(
        broker.HandleCommand(
            42,
            "PUBLISH test Hello",
            {} ),
        "ERROR Topic not found\n" );

    EXPECT_TRUE(
        topicManagerPtr->publishCalled );

    EXPECT_FALSE(
        clientManagerPtr->sendMessageCalled );
}

/// @brief Тест: Успешная отписка от топика.
TEST( MessageBrokerTest, Unsubscribe )
{
    auto clientManager = std::make_unique< MockClientManager >();
    auto topicManager = std::make_unique< MockTopicManager >();

    clientManager->clientIdToReturn = 100;

    auto* topicManagerPtr = topicManager.get();

    MessageBroker broker(
        std::move( clientManager ),
        std::move( topicManager ) );

    EXPECT_EQ(
        broker.HandleCommand(
            42,
            "UNSUBSCRIBE test",
            {} ),
        "OK\n" );

    EXPECT_TRUE(
        topicManagerPtr->unsubscribeCalled );

    EXPECT_EQ(
        topicManagerPtr->lastUnsubscribeClientId,
        100 );

    EXPECT_EQ(
        topicManagerPtr->lastUnsubscribeTopic,
        "test" );
}

/// @brief Тест: Отписка от топика без авторизации.
TEST( MessageBrokerTest, UnsubscribeUnauthorized )
{
    auto clientManager = std::make_unique< MockClientManager >();
    auto topicManager = std::make_unique< MockTopicManager >();

    clientManager->getClientIdResult =
        ClientsCodes::ClientNotFound;

    auto* topicManagerPtr = topicManager.get();

    MessageBroker broker(
        std::move( clientManager ),
        std::move( topicManager ) );

    EXPECT_EQ(
        broker.HandleCommand(
            42,
            "UNSUBSCRIBE test",
            {} ),
        "ERROR You are unauthorized\n" );

    EXPECT_FALSE(
        topicManagerPtr->unsubscribeCalled );
}

/// @brief Тест: Отписка от несуществующего топика.
TEST( MessageBrokerTest, UnsubscribeTopicNotFound )
{
    auto clientManager = std::make_unique< MockClientManager >();
    auto topicManager = std::make_unique< MockTopicManager >();

    clientManager->clientIdToReturn = 100;
    topicManager->unsubscribeResult =
        TopicCodes::TopicNotFound;

    auto* topicManagerPtr = topicManager.get();

    MessageBroker broker(
        std::move( clientManager ),
        std::move( topicManager ) );

    EXPECT_EQ(
        broker.HandleCommand(
            42,
            "UNSUBSCRIBE test",
            {} ),
        "ERROR Topic not found\n" );

    EXPECT_TRUE(
        topicManagerPtr->unsubscribeCalled );

    EXPECT_EQ(
        topicManagerPtr->lastUnsubscribeClientId,
        100 );

    EXPECT_EQ(
        topicManagerPtr->lastUnsubscribeTopic,
        "test" );
}

/// @brief Тест: Парсинг сообщения.
TEST( MessageBrokerTest, PublishMessageWithSpaces )
{
    auto clientManager = std::make_unique< MockClientManager >();
    auto topicManager = std::make_unique< MockTopicManager >();

    auto* topicManagerPtr = topicManager.get();

    MessageBroker broker(
        std::move( clientManager ),
        std::move( topicManager ) );

    EXPECT_EQ(
        broker.HandleCommand(
            1,
            "PUBLISH test Hello this is a message",
            {} ),
        "OK\n" );

    EXPECT_EQ(
        topicManagerPtr->lastPublishTopic,
        "test" );

    EXPECT_EQ(
        topicManagerPtr->lastPublishMessage,
        "Hello this is a message" );
}

///
/// Общие тесты.
///

/// @brief Команда без аргумента.
TEST( MessageBrokerTest, CommandWithoutArgument )
{
    auto clientManager = std::make_unique< MockClientManager >();
    auto topicManager = std::make_unique< MockTopicManager >();

    MessageBroker broker(
        std::move( clientManager ),
        std::move( topicManager ) );

    EXPECT_EQ(
        broker.HandleCommand( 1, "LOGIN", {} ),
        "ERROR Invalid command\n" );
}