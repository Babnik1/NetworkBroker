/// @file server_test.cpp
///
/// Тесты сервера.
///


#include <gtest/gtest.h>
#include <boost/asio.hpp>
#include "server/server.h"
#include "broker/message_broker.h"


namespace
{

/// @brief Получение свободного TCP-порта.
/// @return Порт.
unsigned short GetFreePort()
{
    boost::asio::io_context ioContext;

    boost::asio::ip::tcp::acceptor acceptor(
        ioContext,
        boost::asio::ip::tcp::endpoint(
            boost::asio::ip::tcp::v4(),
            0 ) );

    return acceptor.local_endpoint().port();
}

} // namespace


/// @brief Тест: создание сервера.
TEST( ServerTest, Create )
{
    const auto port = GetFreePort();

    auto broker =
        std::make_shared< MessageBroker >(
            nullptr,
            nullptr );

    EXPECT_NO_THROW(
        Server server(
            static_cast< short >( port ),
            broker ) );
}


/// @brief Тест: удаление несуществующей сессии.
TEST( ServerTest, RemoveSessionNotFound )
{
    const auto port = GetFreePort();

    auto broker =
        std::make_shared< MessageBroker >(
            nullptr,
            nullptr );

    Server server(
        static_cast< short >( port ),
        broker );

    EXPECT_NO_THROW(
        server.RemoveSession( 42 ) );
}


/// @brief Тест: остановка сервера.
TEST( ServerTest, Stop )
{
    const auto port = GetFreePort();

    auto broker =
        std::make_shared< MessageBroker >(
            nullptr,
            nullptr );

    Server server(
        static_cast< short >( port ),
        broker );

    EXPECT_NO_THROW(
        server.Stop() );
}