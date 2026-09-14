/// @file client_test.cpp
///
/// Тесты клиента.
///


#include <gtest/gtest.h>
#include <boost/asio.hpp>
#include "clients/client.h"

namespace
{

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

} // namespace


/// @brief Тест: получение ID клиента.
TEST( ClientTest, GetId )
{
    Client client( 100, "alex" );

    EXPECT_EQ(
        client.GetId(),
        100 );
}


/// @brief Тест: получение имени клиента.
TEST( ClientTest, GetName )
{
    Client client( 100, "alex" );

    EXPECT_EQ(
        client.GetName(),
        "alex" );
}


/// @brief Тест: новый клиент не подключён.
TEST( ClientTest, SessionIdWhenDisconnected )
{
    Client client( 100, "alex" );

    EXPECT_EQ(
        client.GetSessionId(),
        invalidSessionId );
}


/// @brief Тест: установка сессии.
TEST( ClientTest, SetSession )
{
    boost::asio::io_context ioContext;

    auto session = CreateSession( 42, ioContext );

    Client client( 100, "alex" );

    client.SetSession( session );

    EXPECT_EQ(
        client.GetSessionId(),
        42 );
}


/// @brief Тест: отключение клиента.
TEST( ClientTest, Disconnect )
{
    boost::asio::io_context ioContext;

    auto session = CreateSession( 42, ioContext );

    Client client( 100, "alex" );

    client.SetSession( session );

    ASSERT_EQ(
        client.GetSessionId(),
        42 );

    client.Disconnect();

    EXPECT_EQ(
        client.GetSessionId(),
        invalidSessionId );
}


/// @brief Тест: сессия уничтожена, weak_ptr становится недействительным.
TEST( ClientTest, SessionDestroyed )
{
    boost::asio::io_context ioContext;

    Client client( 100, "alex" );

    {
        auto session = CreateSession( 42, ioContext );

        client.SetSession( session );

        ASSERT_EQ(
            client.GetSessionId(),
            42 );
    }

    EXPECT_EQ(
        client.GetSessionId(),
        invalidSessionId );
}


/// @brief Тест: отправка сообщения отключённому клиенту.
TEST( ClientTest, SendTopicMessageWhenDisconnected )
{
    Client client( 100, "alex" );

    EXPECT_NO_THROW(
        client.SendTopicMessage( "Hello" ) );
}


/// @brief Тест: SendTopicMessage не падает после уничтожения сессии.
TEST( ClientTest, SendTopicMessageAfterSessionDestroyed )
{
    boost::asio::io_context ioContext;

    Client client( 100, "alex" );

    {
        auto session = CreateSession( 42, ioContext );

        client.SetSession( session );

        ASSERT_EQ(
            client.GetSessionId(),
            42 );
    }

    EXPECT_EQ(
        client.GetSessionId(),
        invalidSessionId );

    EXPECT_NO_THROW(
        client.SendTopicMessage( "Hello" ) );
}