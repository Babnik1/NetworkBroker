#include <gtest/gtest.h>

#include <boost/asio.hpp>

#include "clients/clients_manager.h"
#include "mocks/mock_client_repository.h"

namespace
{

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

} // namespace


/// @brief Тест: успешное создание клиента.
TEST( ClientManagerTest, CreateClient )
{
    auto repository = std::make_unique< MockClientRepository >();
    auto* repositoryPtr = repository.get();

    ClientManager manager(
        std::move( repository ) );

    EXPECT_EQ(
        manager.CreateClient( "alex" ),
        ClientsCodes::Ok );

    auto clients = repositoryPtr->LoadClients();

    ASSERT_EQ(
        clients.size(),
        1 );

    EXPECT_EQ(
        clients[ 0 ].GetName(),
        "alex" );

    EXPECT_NE(
        clients[ 0 ].GetId(),
        invalidClientId );
}


/// @brief Тест: ошибка сохранения клиента в БД.
TEST( ClientManagerTest, CreateClientDatabaseError )
{
    auto repository = std::make_unique< MockClientRepository >();

    repository->SetSaveResult( false );

    ClientManager manager(
        std::move( repository ) );

    EXPECT_EQ(
        manager.CreateClient( "alex" ),
        ClientsCodes::InternalError );
}


/// @brief Тест: успешное подключение клиента.
TEST( ClientManagerTest, ConnectClient )
{
    auto repository = std::make_unique< MockClientRepository >();

    repository->AddClient(
        Client( 100, "alex" ) );

    ClientManager manager(
        std::move( repository ) );

    boost::asio::io_context ioContext;
    auto session = CreateSession( 42, ioContext );

    EXPECT_EQ(
        manager.ConnectClient(
            "alex",
            42,
            session ),
        ClientsCodes::Ok );

    ClientId clientId = invalidClientId;

    EXPECT_EQ(
        manager.GetClientId(
            42,
            clientId ),
        ClientsCodes::Ok );

    EXPECT_EQ(
        clientId,
        100 );
}


/// @brief Тест: подключение несуществующего клиента.
TEST( ClientManagerTest, ConnectClientNotFound )
{
    auto repository = std::make_unique< MockClientRepository >();

    ClientManager manager(
        std::move( repository ) );

    boost::asio::io_context ioContext;
    auto session = CreateSession( 42, ioContext );

    EXPECT_EQ(
        manager.ConnectClient(
            "alex",
            42,
            session ),
        ClientsCodes::ClientNotFound );
}


/// @brief Тест: повторное подключение клиента.
TEST( ClientManagerTest, ConnectClientAlreadyConnected )
{
    auto repository = std::make_unique< MockClientRepository >();

    repository->AddClient(
        Client( 100, "alex" ) );

    ClientManager manager(
        std::move( repository ) );

    boost::asio::io_context ioContext;

    auto firstSession =
        CreateSession( 42, ioContext );

    auto secondSession =
        CreateSession( 43, ioContext );

    ASSERT_EQ(
        manager.ConnectClient(
            "alex",
            42,
            firstSession ),
        ClientsCodes::Ok );

    EXPECT_EQ(
        manager.ConnectClient(
            "alex",
            43,
            secondSession ),
        ClientsCodes::ClientAlreadyConnected );
}


/// @brief Тест: получение ID подключённого клиента.
TEST( ClientManagerTest, GetClientId )
{
    auto repository = std::make_unique< MockClientRepository >();

    repository->AddClient(
        Client( 100, "alex" ) );

    ClientManager manager(
        std::move( repository ) );

    boost::asio::io_context ioContext;
    auto session = CreateSession( 42, ioContext );

    ASSERT_EQ(
        manager.ConnectClient(
            "alex",
            42,
            session ),
        ClientsCodes::Ok );

    ClientId clientId = invalidClientId;

    EXPECT_EQ(
        manager.GetClientId(
            42,
            clientId ),
        ClientsCodes::Ok );

    EXPECT_EQ(
        clientId,
        100 );
}


/// @brief Тест: получение ID клиента по неизвестной сессии.
TEST( ClientManagerTest, GetClientIdNotFound )
{
    auto repository = std::make_unique< MockClientRepository >();

    repository->AddClient(
        Client( 100, "alex" ) );

    ClientManager manager(
        std::move( repository ) );

    ClientId clientId = invalidClientId;

    EXPECT_EQ(
        manager.GetClientId(
            42,
            clientId ),
        ClientsCodes::ClientNotFound );

    EXPECT_EQ(
        clientId,
        invalidClientId );
}


/// @brief Тест: отключение клиента.
TEST( ClientManagerTest, DisconnectClient )
{
    auto repository = std::make_unique< MockClientRepository >();

    repository->AddClient(
        Client( 100, "alex" ) );

    ClientManager manager(
        std::move( repository ) );

    boost::asio::io_context ioContext;
    auto session = CreateSession( 42, ioContext );

    ASSERT_EQ(
        manager.ConnectClient(
            "alex",
            42,
            session ),
        ClientsCodes::Ok );

    manager.DisconnectClient( 42 );

    ClientId clientId = invalidClientId;

    EXPECT_EQ(
        manager.GetClientId(
            42,
            clientId ),
        ClientsCodes::ClientNotFound );
}


/// @brief Тест: отключение неизвестной сессии.
TEST( ClientManagerTest, DisconnectClientNotFound )
{
    auto repository = std::make_unique< MockClientRepository >();

    repository->AddClient(
        Client( 100, "alex" ) );

    ClientManager manager(
        std::move( repository ) );

    EXPECT_NO_THROW(
        manager.DisconnectClient( 42 ) );
}


/// @brief Тест: успешное удаление клиента.
TEST( ClientManagerTest, RemoveClient )
{
    auto repository = std::make_unique< MockClientRepository >();

    repository->AddClient(
        Client( 100, "alex" ) );

    auto* repositoryPtr = repository.get();

    ClientManager manager(
        std::move( repository ) );

    EXPECT_EQ(
        manager.RemoveClient( 100 ),
        ClientsCodes::Ok );

    EXPECT_TRUE(
        repositoryPtr->LoadClients().empty() );
}

/// @brief Тест: удаление несуществующего клиента.
TEST( ClientManagerTest, RemoveClientNotFound )
{
    auto repository = std::make_unique< MockClientRepository >();

    ClientManager manager(
        std::move( repository ) );

    EXPECT_EQ(
        manager.RemoveClient( 100 ),
        ClientsCodes::ClientNotFound );
}


/// @brief Тест: ошибка БД при удалении клиента.
TEST( ClientManagerTest, RemoveClientDatabaseError )
{
    auto repository = std::make_unique< MockClientRepository >();

    repository->AddClient(
        Client( 100, "alex" ) );

    repository->SetDeleteResult( false );

    ClientManager manager(
        std::move( repository ) );

    EXPECT_EQ(
        manager.RemoveClient( 100 ),
        ClientsCodes::InternalError );
}