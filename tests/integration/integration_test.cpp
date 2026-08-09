/// @file integration_test.cpp
///
/// Интеграционные тесты.
///


#include <gtest/gtest.h>
#include <boost/asio.hpp>

#include <cstdio>
#include <memory>
#include <string>

#include "broker/message_broker.h"
#include "clients/clients_manager.h"
#include "topics/topic_manager.h"

#include "db/json_client_repository.h"
#include "db/json_topic_repository.h"
#include "server/session.h"

namespace
{

/// @brief Файл БД клиентов.
const std::string ClientsDb = "test_integration_clients.json";

/// @brief Файл БД топиков.
const std::string TopicsDb = "test_integration_topics.json";

/// @brief Подмена создания сессии.
/// @param[in] id ID сессии.
/// @param[in] ioContext Контекст.
/// @return Указатель на сессию.
SessionPtr CreateSession(
    SessionId id,
    boost::asio::io_context& ioContext )
{
    boost::asio::ip::tcp::socket socket( ioContext );

    return std::make_shared< Session >(
        id,
        std::move( socket ),
        nullptr );
}

/// @brief Создание брокера.
/// @return Указатель на брокер.
std::shared_ptr< MessageBroker > CreateBroker()
{
    auto clientRepository =
        std::make_unique< JsonRepository >( ClientsDb );

    auto topicRepository =
        std::make_unique< JsonTopicRepository >( TopicsDb );

    auto clientManager =
        std::make_shared< ClientManager >(
            std::move( clientRepository ) );

    auto topicManager =
        std::make_shared< TopicManager >(
            std::move( topicRepository ) );

    return std::make_shared< MessageBroker >(
        clientManager,
        topicManager );
}

/// @brief Удаление тестового файла БД.
void RemoveTestDatabases()
{
    std::remove( ClientsDb.c_str() );
    std::remove( TopicsDb.c_str() );
}

/// @brief Структура соединения.
struct TestConnection
{
    boost::asio::ip::tcp::socket client;
    boost::asio::ip::tcp::socket server;

    explicit TestConnection(
        boost::asio::io_context& ioContext )
    : client( ioContext )
    , server( ioContext )
    {
        boost::asio::ip::tcp::acceptor acceptor(
            ioContext,
            boost::asio::ip::tcp::endpoint(
                boost::asio::ip::tcp::v4(),
                0 ) );

        client.connect(
            acceptor.local_endpoint() );

        acceptor.accept( server );
    }
};

/// @brief Тестовый класс.
class MessageBrokerIntegrationTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        RemoveTestDatabases();
    }

    void TearDown() override
    {
        RemoveTestDatabases();
    }
};

} // namespace


/// @brief Тест: регистрация клиента и создание топика.
TEST_F( MessageBrokerIntegrationTest, CreateTopic )
{
    RemoveTestDatabases();

    auto broker = CreateBroker();

    boost::asio::io_context ioContext;

    auto session =
        CreateSession( 42, ioContext );

    EXPECT_EQ(
        broker->HandleCommand(
            42,
            "REGISTER alex",
            session ),
        "OK\n" );

    EXPECT_EQ(
        broker->HandleCommand(
            42,
            "CREATE test",
            session ),
        "OK\n" );

    JsonTopicRepository repository( TopicsDb );

    auto topics = repository.LoadTopics();

    ASSERT_EQ(
        topics.size(),
        1 );

    ASSERT_TRUE(
        topics.find( "test" ) != topics.end() );

    const auto& clients =
        topics.at( "test" );

    EXPECT_EQ(
    clients.size(),
    1 );
}


/// @brief Тест: создание и подписка на топик.
TEST_F( MessageBrokerIntegrationTest, SubscribeTopic )
{
    RemoveTestDatabases();

    auto broker = CreateBroker();

    boost::asio::io_context ioContext;

    auto sessionAlex =
        CreateSession( 42, ioContext );

    auto sessionBob =
        CreateSession( 43, ioContext );

    ASSERT_EQ(
        broker->HandleCommand(
            42,
            "REGISTER alex",
            sessionAlex ),
        "OK\n" );

    ASSERT_EQ(
        broker->HandleCommand(
            43,
            "REGISTER bob",
            sessionBob ),
        "OK\n" );

    ASSERT_EQ(
        broker->HandleCommand(
            42,
            "CREATE test",
            sessionAlex ),
        "OK\n" );

    EXPECT_EQ(
        broker->HandleCommand(
            43,
            "SUBSCRIBE test",
            sessionBob ),
        "OK\n" );

    JsonTopicRepository repository( TopicsDb );

    auto topics = repository.LoadTopics();

    ASSERT_TRUE(
        topics.find( "test" ) != topics.end() );

    const auto& clients =
        topics.at( "test" );

    EXPECT_EQ(
        clients.size(),
        2 );
}


/// @brief Тест: отписка клиента от топика.
TEST_F( MessageBrokerIntegrationTest, UnsubscribeTopic )
{
    RemoveTestDatabases();

    auto broker = CreateBroker();

    boost::asio::io_context ioContext;

    auto sessionAlex =
        CreateSession( 42, ioContext );

    auto sessionBob =
        CreateSession( 43, ioContext );

    ASSERT_EQ(
        broker->HandleCommand(
            42,
            "REGISTER alex",
            sessionAlex ),
        "OK\n" );

    ASSERT_EQ(
        broker->HandleCommand(
            43,
            "REGISTER bob",
            sessionBob ),
        "OK\n" );

    ASSERT_EQ(
        broker->HandleCommand(
            42,
            "CREATE test",
            sessionAlex ),
        "OK\n" );

    ASSERT_EQ(
        broker->HandleCommand(
            43,
            "SUBSCRIBE test",
            sessionBob ),
        "OK\n" );

    EXPECT_EQ(
        broker->HandleCommand(
            43,
            "UNSUBSCRIBE test",
            sessionBob ),
        "OK\n" );

    JsonTopicRepository repository( TopicsDb );

    auto topics = repository.LoadTopics();

    ASSERT_TRUE(
        topics.find( "test" ) != topics.end() );

    const auto& clients =
        topics.at( "test" );

    EXPECT_EQ(
        clients.size(),
        1 );
}


/// @brief Тест: публикация сообщения.
TEST_F( MessageBrokerIntegrationTest, Publish )
{
    RemoveTestDatabases();

    auto broker = CreateBroker();

    boost::asio::io_context ioContext;

    auto sessionAlex =
        CreateSession( 42, ioContext );

    auto sessionBob =
        CreateSession( 43, ioContext );

    ASSERT_EQ(
        broker->HandleCommand(
            42,
            "REGISTER alex",
            sessionAlex ),
        "OK\n" );

    ASSERT_EQ(
        broker->HandleCommand(
            43,
            "REGISTER bob",
            sessionBob ),
        "OK\n" );

    ASSERT_EQ(
        broker->HandleCommand(
            42,
            "CREATE test",
            sessionAlex ),
        "OK\n" );

    ASSERT_EQ(
        broker->HandleCommand(
            43,
            "SUBSCRIBE test",
            sessionBob ),
        "OK\n" );

    EXPECT_EQ(
        broker->HandleCommand(
            42,
            "PUBLISH test Hello world",
            sessionAlex ),
        "OK\n" );
}


/// @brief Тест: полный сценарий работы брокера.
TEST_F( MessageBrokerIntegrationTest, FullScenario )
{
    RemoveTestDatabases();

    auto broker = CreateBroker();

    boost::asio::io_context ioContext;

    auto sessionAlex =
        CreateSession( 42, ioContext );

    auto sessionBob =
        CreateSession( 43, ioContext );

    // Регистрация Alex.
    ASSERT_EQ(
        broker->HandleCommand(
            42,
            "REGISTER alex",
            sessionAlex ),
        "OK\n" );

    // Регистрация Bob.
    ASSERT_EQ(
        broker->HandleCommand(
            43,
            "REGISTER bob",
            sessionBob ),
        "OK\n" );

    // Alex создаёт топик.
    ASSERT_EQ(
        broker->HandleCommand(
            42,
            "CREATE test",
            sessionAlex ),
        "OK\n" );

    // Bob подписывается.
    ASSERT_EQ(
        broker->HandleCommand(
            43,
            "SUBSCRIBE test",
            sessionBob ),
        "OK\n" );

    // Bob отписывается.
    ASSERT_EQ(
        broker->HandleCommand(
            43,
            "UNSUBSCRIBE test",
            sessionBob ),
        "OK\n" );

    // Alex публикует сообщение.
    EXPECT_EQ(
        broker->HandleCommand(
            42,
            "PUBLISH test Hello world",
            sessionAlex ),
        "OK\n" );
}


TEST_F( MessageBrokerIntegrationTest, PublishToSubscriber )
{
    RemoveTestDatabases();

    auto broker = CreateBroker();

    boost::asio::io_context ioContext;

    TestConnection alexConnection( ioContext );
    TestConnection bobConnection( ioContext );

    auto alexSession =
        std::make_shared< Session >(
            42,
            std::move( alexConnection.server ),
            broker );

    auto bobSession =
        std::make_shared< Session >(
            43,
            std::move( bobConnection.server ),
            broker );

    ASSERT_EQ(
        broker->HandleCommand(
            42,
            "REGISTER alex",
            alexSession ),
        "OK\n" );

    ASSERT_EQ(
        broker->HandleCommand(
            43,
            "REGISTER bob",
            bobSession ),
        "OK\n" );

    ASSERT_EQ(
        broker->HandleCommand(
            42,
            "CREATE test",
            alexSession ),
        "OK\n" );

    ASSERT_EQ(
        broker->HandleCommand(
            43,
            "SUBSCRIBE test",
            bobSession ),
        "OK\n" );

    EXPECT_EQ(
        broker->HandleCommand(
            42,
            "PUBLISH test Hello world",
            alexSession ),
        "OK\n" );
}