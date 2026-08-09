#include <gtest/gtest.h>

#include <cstdio>
#include <fstream>
#include <string>

#include "db/json_topic_repository.h"

namespace
{

const std::string TestDb = "test_topics.json";

void WriteFile( const std::string& content )
{
    std::ofstream file( TestDb );
    file << content;
}

std::string ReadFile()
{
    std::ifstream file( TestDb );

    return {
        std::istreambuf_iterator< char >( file ),
        std::istreambuf_iterator< char >()
    };
}

class JsonTopicRepositoryTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        std::remove( TestDb.c_str() );
    }

    void TearDown() override
    {
        std::remove( TestDb.c_str() );
    }
};

} // namespace


/// @brief Тест: успешное создание топика.
TEST_F( JsonTopicRepositoryTest, CreateTopic )
{
    JsonTopicRepository repository( TestDb );

    EXPECT_TRUE(
        repository.CreateTopic(
            "test",
            100 ) );

    auto topics = repository.LoadTopics();

    ASSERT_EQ(
        topics.size(),
        1 );

    ASSERT_TRUE(
        topics.find( "test" ) != topics.end() );

    EXPECT_TRUE(
        topics.at( "test" ).find( 100 ) !=
        topics.at( "test" ).end() );
}


/// @brief Тест: создание уже существующего топика.
TEST_F( JsonTopicRepositoryTest, CreateTopicAlreadyExists )
{
    JsonTopicRepository repository( TestDb );

    ASSERT_TRUE(
        repository.CreateTopic(
            "test",
            100 ) );

    EXPECT_FALSE(
        repository.CreateTopic(
            "test",
            200 ) );

    auto topics = repository.LoadTopics();

    ASSERT_EQ(
        topics.size(),
        1 );

    EXPECT_TRUE(
        topics.at( "test" ).find( 100 ) !=
        topics.at( "test" ).end() );

    EXPECT_TRUE(
        topics.at( "test" ).find( 200 ) ==
        topics.at( "test" ).end() );
}


/// @brief Тест: загрузка топиков.
TEST_F( JsonTopicRepositoryTest, LoadTopics )
{
    WriteFile(
        R"({
            "test": {
                "clients": [100, 200]
            },
            "other": {
                "clients": [300]
            }
        })" );

    JsonTopicRepository repository( TestDb );

    auto topics = repository.LoadTopics();

    ASSERT_EQ(
        topics.size(),
        2 );

    ASSERT_TRUE(
        topics.find( "test" ) != topics.end() );

    EXPECT_EQ(
        topics.at( "test" ).size(),
        2 );

    EXPECT_TRUE(
        topics.at( "test" ).find( 100 ) !=
        topics.at( "test" ).end() );

    EXPECT_TRUE(
        topics.at( "test" ).find( 200 ) !=
        topics.at( "test" ).end() );

    EXPECT_EQ(
        topics.at( "other" ).size(),
        1 );
}


/// @brief Тест: загрузка при отсутствии файла.
TEST_F( JsonTopicRepositoryTest, LoadTopicsFileNotFound )
{
    JsonTopicRepository repository( TestDb );

    auto topics = repository.LoadTopics();

    EXPECT_TRUE(
        topics.empty() );

    std::ifstream file( TestDb );

    EXPECT_TRUE(
        file.is_open() );
}


/// @brief Тест: успешная подписка клиента на топик.
TEST_F( JsonTopicRepositoryTest, SaveTopic )
{
    WriteFile(
        R"({
            "test": {
                "clients": [100]
            }
        })" );

    JsonTopicRepository repository( TestDb );

    EXPECT_TRUE(
        repository.SaveTopic(
            "test",
            200 ) );

    auto topics = repository.LoadTopics();

    ASSERT_EQ(
        topics.at( "test" ).size(),
        2 );

    EXPECT_TRUE(
        topics.at( "test" ).find( 100 ) !=
        topics.at( "test" ).end() );

    EXPECT_TRUE(
        topics.at( "test" ).find( 200 ) !=
        topics.at( "test" ).end() );
}


/// @brief Тест: повторная подписка клиента.
TEST_F( JsonTopicRepositoryTest, SaveTopicAlreadySubscribed )
{
    WriteFile(
        R"({
            "test": {
                "clients": [100]
            }
        })" );

    JsonTopicRepository repository( TestDb );

    EXPECT_FALSE(
        repository.SaveTopic(
            "test",
            100 ) );

    auto topics = repository.LoadTopics();

    EXPECT_EQ(
        topics.at( "test" ).size(),
        1 );
}


/// @brief Тест: подписка на несуществующий топик.
TEST_F( JsonTopicRepositoryTest, SaveTopicNotFound )
{
    WriteFile( "{}" );

    JsonTopicRepository repository( TestDb );

    EXPECT_FALSE(
        repository.SaveTopic(
            "test",
            100 ) );
}


/// @brief Тест: успешная отписка клиента.
TEST_F( JsonTopicRepositoryTest, RemoveFromTopic )
{
    WriteFile(
        R"({
            "test": {
                "clients": [100, 200]
            }
        })" );

    JsonTopicRepository repository( TestDb );

    EXPECT_TRUE(
        repository.RemoveFromTopic(
            "test",
            100 ) );

    auto topics = repository.LoadTopics();

    ASSERT_EQ(
        topics.at( "test" ).size(),
        1 );

    EXPECT_TRUE(
        topics.at( "test" ).find( 100 ) ==
        topics.at( "test" ).end() );

    EXPECT_TRUE(
        topics.at( "test" ).find( 200 ) !=
        topics.at( "test" ).end() );
}


/// @brief Тест: отписка клиента, которого нет в топике.
TEST_F( JsonTopicRepositoryTest, RemoveFromTopicClientNotFound )
{
    WriteFile(
        R"({
            "test": {
                "clients": [100]
            }
        })" );

    JsonTopicRepository repository( TestDb );

    EXPECT_FALSE(
        repository.RemoveFromTopic(
            "test",
            200 ) );

    auto topics = repository.LoadTopics();

    EXPECT_EQ(
        topics.at( "test" ).size(),
        1 );
}


/// @brief Тест: отписка от несуществующего топика.
TEST_F( JsonTopicRepositoryTest, RemoveFromTopicNotFound )
{
    WriteFile( "{}" );

    JsonTopicRepository repository( TestDb );

    EXPECT_FALSE(
        repository.RemoveFromTopic(
            "test",
            100 ) );
}