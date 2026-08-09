#include <gtest/gtest.h>

#include "topics/topic_manager.h"
#include "mocks/mock_topic_repository.h"

/// @brief Тест: успешное создание топика.
TEST( TopicManagerTest, CreateTopic )
{
    auto repository = std::make_unique< MockTopicRepository >();
    auto* repositoryPtr = repository.get();

    TopicManager manager(
        std::move( repository ) );

    EXPECT_EQ(
        manager.Create( 100, "test" ),
        TopicCodes::Ok );

    auto topics = repositoryPtr->LoadTopics();

    ASSERT_EQ(
        topics.size(),
        1 );

    ASSERT_TRUE( topics.find( "test" ) != topics.end() );

    auto& set_100 = topics.at( "test" );
    EXPECT_TRUE( set_100.find( 100 ) != set_100.end() );
}

/// @brief Тест: создание уже существующего топика.
TEST( TopicManagerTest, CreateTopicAlreadyExists )
{
    auto repository = std::make_unique< MockTopicRepository >();

    repository->AddTopic(
        "test",
        { 100 } );

    TopicManager manager(
        std::move( repository ) );

    EXPECT_EQ(
        manager.Create( 200, "test" ),
        TopicCodes::TopicAlreadyExists );
}

/// @brief Тест: ошибка БД при создании топика.
TEST( TopicManagerTest, CreateTopicDatabaseError )
{
    auto repository = std::make_unique< MockTopicRepository >();

    repository->SetCreateResult( false );

    TopicManager manager(
        std::move( repository ) );

    EXPECT_EQ(
        manager.Create( 100, "test" ),
        TopicCodes::InternalError );
}

/// @brief Тест: успешная подписка на топик.
TEST( TopicManagerTest, Subscribe )
{
    auto repository = std::make_unique< MockTopicRepository >();
    auto* repositoryPtr = repository.get();

    repository->AddTopic(
        "test",
        {} );

    TopicManager manager(
        std::move( repository ) );

    EXPECT_EQ(
        manager.Subscribe( 100, "test" ),
        TopicCodes::Ok );

    auto topics = repositoryPtr->LoadTopics();

    auto& current_set = topics.at( "test" );
    EXPECT_TRUE( current_set.find( 100 ) != current_set.end() );
}

/// @brief Тест: подписка на несуществующий топик.
TEST( TopicManagerTest, SubscribeTopicNotFound )
{
    auto repository = std::make_unique< MockTopicRepository >();

    TopicManager manager(
        std::move( repository ) );

    EXPECT_EQ(
        manager.Subscribe( 100, "test" ),
        TopicCodes::TopicNotFound );
}

/// @brief Тест: ошибка БД при подписке.
TEST( TopicManagerTest, SubscribeDatabaseError )
{
    auto repository = std::make_unique< MockTopicRepository >();

    repository->AddTopic(
        "test",
        {} );

    repository->SetSaveResult( false );

    TopicManager manager(
        std::move( repository ) );

    EXPECT_EQ(
        manager.Subscribe( 100, "test" ),
        TopicCodes::InternalError );
}

/// @brief Тест: успешная отписка от топика.
TEST( TopicManagerTest, Unsubscribe )
{
    auto repository = std::make_unique< MockTopicRepository >();
    auto* repositoryPtr = repository.get();

    repository->AddTopic(
        "test",
        { 100, 200 } );

    TopicManager manager(
        std::move( repository ) );

    EXPECT_EQ(
        manager.Unsubscribe( 100, "test" ),
        TopicCodes::Ok );

    auto topics = repositoryPtr->LoadTopics();

    auto& current_set = topics.at( "test" );

    EXPECT_FALSE( current_set.find( 100 ) != current_set.end() ); 

    EXPECT_TRUE( current_set.find( 200 ) != current_set.end() );
}

/// @brief Тест: отписка от несуществующего топика.
TEST( TopicManagerTest, UnsubscribeTopicNotFound )
{
    auto repository = std::make_unique< MockTopicRepository >();

    TopicManager manager(
        std::move( repository ) );

    EXPECT_EQ(
        manager.Unsubscribe( 100, "test" ),
        TopicCodes::TopicNotFound );
}

/// @brief Тест: отписка клиента, который не подписан.
TEST( TopicManagerTest, UnsubscribeNotSubscribed )
{
    auto repository = std::make_unique< MockTopicRepository >();

    repository->AddTopic(
        "test",
        { 200 } );

    TopicManager manager(
        std::move( repository ) );

    EXPECT_EQ(
        manager.Unsubscribe( 100, "test" ),
        TopicCodes::NotSubscribed );
}

/// @brief Тест: ошибка БД при отписке.
TEST( TopicManagerTest, UnsubscribeDatabaseError )
{
    auto repository = std::make_unique< MockTopicRepository >();

    repository->AddTopic(
        "test",
        { 100 } );

    repository->SetRemoveResult( false );

    TopicManager manager(
        std::move( repository ) );

    EXPECT_EQ(
        manager.Unsubscribe( 100, "test" ),
        TopicCodes::InternalError );
}

/// @brief Тест: успешная публикация.
TEST( TopicManagerTest, Publish )
{
    auto repository = std::make_unique< MockTopicRepository >();

    repository->AddTopic(
        "test",
        { 100, 200, 300 } );

    TopicManager manager(
        std::move( repository ) );

    std::unordered_set< ClientId > clients;

    EXPECT_EQ(
        manager.Publish(
            "test",
            "Hello",
            clients ),
        TopicCodes::Ok );

    EXPECT_EQ(
        clients,
        ( std::unordered_set< ClientId >{ 100, 200, 300 } ) );
}

/// @brief Тест: публикация в несуществующий топик.
TEST( TopicManagerTest, PublishTopicNotFound )
{
    auto repository = std::make_unique< MockTopicRepository >();

    TopicManager manager(
        std::move( repository ) );

    std::unordered_set< ClientId > clients;

    EXPECT_EQ(
        manager.Publish(
            "test",
            "Hello",
            clients ),
        TopicCodes::TopicNotFound );

    EXPECT_TRUE(
        clients.empty() );
}

/// @brief Тест: загрузка топиков из БД.
TEST( TopicManagerTest, LoadTopics )
{
    auto repository = std::make_unique< MockTopicRepository >();

    repository->AddTopic(
        "test",
        { 100, 200 } );

    repository->AddTopic(
        "news",
        { 300 } );

    TopicManager manager(
        std::move( repository ) );

    std::unordered_set< ClientId > clients;

    EXPECT_EQ(
        manager.Publish(
            "test",
            "Hello",
            clients ),
        TopicCodes::Ok );

    EXPECT_EQ(
        clients,
        ( std::unordered_set< ClientId >{ 100, 200 } ) );

    clients.clear();

    EXPECT_EQ(
        manager.Publish(
            "news",
            "Hello",
            clients ),
        TopicCodes::Ok );

    EXPECT_EQ(
        clients,
        ( std::unordered_set< ClientId >{ 300 } ) );
}