/// @file json_client_repository_test.cpp
///
/// Тесты БД клиентов.
///


#include <gtest/gtest.h>
#include <fstream>
#include <string>

#include "db/json_client_repository.h"

namespace
{

/// @brief Тестовый файл БД.
const std::string testDbFile = "test_clients.json";

/// @brief Удаление тестового файла БД.
void RemoveTestDatabase()
{
    std::remove( testDbFile.c_str() );
}

/// @brief Создание тестового файла БД.
void CreateDatabase( const std::string& content )
{
    std::ofstream file( testDbFile );
    ASSERT_TRUE( file.is_open() );

    file << content;
    file.close();
}

} // namespace

/// @brief Тест: загрузка клиентов из БД.
TEST( JsonClientRepositoryTest, LoadClients )
{
    RemoveTestDatabase();

    CreateDatabase(
        R"({
            "100": {
                "name": "alex"
            },
            "200": {
                "name": "bob"
            }
        })" );

    JsonRepository repository( testDbFile );

    auto clients = repository.LoadClients();

    ASSERT_EQ(
        clients.size(),
        2 );

    EXPECT_EQ(
        clients[ 0 ].GetId(),
        100 );

    EXPECT_EQ(
        clients[ 0 ].GetName(),
        "alex" );

    EXPECT_EQ(
        clients[ 1 ].GetId(),
        200 );

    EXPECT_EQ(
        clients[ 1 ].GetName(),
        "bob" );

    RemoveTestDatabase();
}

/// @brief Тест: загрузка клиентов при отсутствии файла.
TEST( JsonClientRepositoryTest, LoadClientsFileNotFound )
{
    RemoveTestDatabase();

    JsonRepository repository( testDbFile );

    auto clients = repository.LoadClients();

    EXPECT_TRUE(
        clients.empty() );

    std::ifstream file( testDbFile );

    EXPECT_TRUE(
        file.is_open() );

    RemoveTestDatabase();
}

/// @brief Тест: ошибка загрузки повреждённого JSON.
TEST( JsonClientRepositoryTest, LoadClientsInvalidJson )
{
    RemoveTestDatabase();

    CreateDatabase(
        R"({
            "100": {
                "name": "alex"
        })" );

    JsonRepository repository( testDbFile );

    auto clients = repository.LoadClients();

    EXPECT_TRUE(
        clients.empty() );

    RemoveTestDatabase();
}

/// @brief Тест: сохранение нового клиента.
TEST( JsonClientRepositoryTest, SaveClient )
{
    RemoveTestDatabase();

    CreateDatabase( "{}" );

    JsonRepository repository( testDbFile );

    Client client( 100, "alex" );

    EXPECT_TRUE(
        repository.SaveClient( client ) );

    auto clients = repository.LoadClients();

    ASSERT_EQ(
        clients.size(),
        1 );

    EXPECT_EQ(
        clients[ 0 ].GetId(),
        100 );

    EXPECT_EQ(
        clients[ 0 ].GetName(),
        "alex" );

    RemoveTestDatabase();
}

/// @brief Тест: обновление существующего клиента.
TEST( JsonClientRepositoryTest, SaveClientUpdate )
{
    RemoveTestDatabase();

    CreateDatabase(
        R"({
            "100": {
                "name": "alex"
            }
        })" );

    JsonRepository repository( testDbFile );

    Client client( 100, "bob" );

    EXPECT_TRUE(
        repository.SaveClient( client ) );

    auto clients = repository.LoadClients();

    ASSERT_EQ(
        clients.size(),
        1 );

    EXPECT_EQ(
        clients[ 0 ].GetId(),
        100 );

    EXPECT_EQ(
        clients[ 0 ].GetName(),
        "bob" );

    RemoveTestDatabase();
}

/// @brief Тест: сохранение клиента при отсутствии файла.
TEST( JsonClientRepositoryTest, SaveClientFileNotFound )
{
    RemoveTestDatabase();

    JsonRepository repository( testDbFile );

    Client client( 100, "alex" );

    EXPECT_FALSE(
        repository.SaveClient( client ) );

    RemoveTestDatabase();
}

/// @brief Тест: ошибка сохранения при повреждённом JSON.
TEST( JsonClientRepositoryTest, SaveClientInvalidJson )
{
    RemoveTestDatabase();

    CreateDatabase(
        R"({
            "100": {
                "name": "alex"
        })" );

    JsonRepository repository( testDbFile );

    Client client( 200, "bob" );

    EXPECT_FALSE(
        repository.SaveClient( client ) );

    RemoveTestDatabase();
}

/// @brief Тест: удаление клиента.
TEST( JsonClientRepositoryTest, DeleteClient )
{
    RemoveTestDatabase();

    CreateDatabase(
        R"({
            "100": {
                "name": "alex"
            },
            "200": {
                "name": "bob"
            }
        })" );

    JsonRepository repository( testDbFile );

    EXPECT_TRUE(
        repository.DeleteClient( 100 ) );

    auto clients = repository.LoadClients();

    ASSERT_EQ(
        clients.size(),
        1 );

    EXPECT_EQ(
        clients[ 0 ].GetId(),
        200 );

    EXPECT_EQ(
        clients[ 0 ].GetName(),
        "bob" );

    RemoveTestDatabase();
}

/// @brief Тест: удаление несуществующего клиента.
TEST( JsonClientRepositoryTest, DeleteClientNotFound )
{
    RemoveTestDatabase();

    CreateDatabase(
        R"({
            "100": {
                "name": "alex"
            }
        })" );

    JsonRepository repository( testDbFile );

    EXPECT_FALSE(
        repository.DeleteClient( 200 ) );

    auto clients = repository.LoadClients();

    ASSERT_EQ(
        clients.size(),
        1 );

    EXPECT_EQ(
        clients[ 0 ].GetId(),
        100 );

    RemoveTestDatabase();
}

/// @brief Тест: удаление клиента при отсутствии файла.
TEST( JsonClientRepositoryTest, DeleteClientFileNotFound )
{
    RemoveTestDatabase();

    JsonRepository repository( testDbFile );

    EXPECT_FALSE(
        repository.DeleteClient( 100 ) );

    RemoveTestDatabase();
}

/// @brief Тест: ошибка удаления при повреждённом JSON.
TEST( JsonClientRepositoryTest, DeleteClientInvalidJson )
{
    RemoveTestDatabase();

    CreateDatabase(
        R"({
            "100": {
                "name": "alex"
        })" );

    JsonRepository repository( testDbFile );

    EXPECT_FALSE(
        repository.DeleteClient( 100 ) );

    RemoveTestDatabase();
}