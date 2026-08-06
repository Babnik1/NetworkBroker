#pragma once
/// @file session.h
///
/// Сетевая сессия. Объявление.
///


#include <deque>
#include <memory>
#include <boost/asio.hpp>
#include "../broker/message_broker.h"

/// @brief ID сессии.
using SessionId = uint64_t;

/// @brief Очередь сообщений клиенту.
using MessageQueue = std::deque< std::string >;


/// @brief Класс сессии.
class Session
{
public:

    /// @brief Конструктор.
    /// @param[in] id ID сессии.
    /// @param[in] socket Сокет.
    /// @param[in] broker Указатель на брокер.
    Session( SessionId id, boost::asio::ip::tcp::socket socket, MessageBrokerPtr broker );

    /// @brief Старт сессии.
    void Start();

    /// @brief Получить ID сессии.
    /// @return ID сессии.
    SessionId GetId() const;

    /// @brief Отправка сообщения клиенту.
    /// @param [in] msg Сообщение.
    void Send( const std::string& msg );

    /// @brief Установка указателя на себя.
    /// @param [in] self Указатель на себя.
    void SetSelf( std::weak_ptr< Session > self );

private:

    /// @brief Прочитать сообщение от клиента.
    void Read();

    /// @brief Записать сообщение на отправку.
    void DoWrite();

    /// @brief Отключение сессии.
    void Disconnect();

    boost::asio::ip::tcp::socket socket_;   /// Сокет.
    boost::asio::streambuf buffer_;         /// Указатель на брокер.

    SessionId id_;                          /// ID сессии.
    MessageQueue outgoing_;                 /// Очередь на отправку.
    MessageBrokerPtr broker_;               /// Указатель на брокер.
    std::weak_ptr< Session > self_;         /// Указатель на саму себя.
};

using SessionPtr = std::shared_ptr< Session >;
using SessionWeakPtr = std::weak_ptr< Session >;