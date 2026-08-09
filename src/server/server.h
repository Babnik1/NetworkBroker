#pragma once 

/// @file server.h
///
/// Класс TCP сервера. Объявление.
///


#include <boost/asio.hpp>
#include <memory>
#include <unordered_map>
#include <vector>
#include "logs/log.h"
#include "session.h"
#include "fwd.h"

/// @brief Класс сервера.
class Server
{
public:

    /// @brief Конструктор.
    /// @param[in] port Порт.
    /// @param[in] broker Брокер.
    Server( short port, std::shared_ptr< MessageBroker > broker );

    /// @brief Запуск сервера.
    void Start();

    /// @brief Остановка сервера.
    void Stop();

    /// @brief Отключение сессии.
    /// @param[in] id ID Сессии.
    void RemoveSession( SessionId id );

private:

    SessionId nextSessionId_{ 1 };                              /// Счетчик ID сессий.
    boost::asio::io_context ioContext_;                         /// Контекст ввода-вывода.
    boost::asio::ip::tcp::acceptor acceptor_;                   /// Ацептор.
    std::unordered_map< SessionId, SessionPtr > sessions_;      /// Активные сессии.
    std::shared_ptr< MessageBroker > broker_;                   /// Брокер.

    /// @brief Ожидание нового подключения.
    void DoAccept();
    
};

using ServerPtr = std::unique_ptr< Server >;