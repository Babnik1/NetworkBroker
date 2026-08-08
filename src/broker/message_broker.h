#pragma once
/// @file message_broker.h
///
/// Брокер. Объявление.
///


#include "fwd.h"
#include "rc.h"

/// @brief Класс брокера.
class MessageBroker
{
public:

    /// @brief Конструктор.
    /// @param[in] cliManager Менеджер клиентов.
    /// @param[in] topManager Менеджер топиков.
    /// @return Код возврата.
    MessageBroker( ClientManagerPtr cliManager, TopicManagerPtr topManager );

    /// @brief Обработка команды.
    /// @param[in] id ID сессии.
    /// @param[in] command Сообщение.
    /// @param[in] session Слабая ссылка на сессию.
    /// @details Получает слабую ссылку на сессию, чтобы потом была возможность отправлять ответ.
    /// @return Строка-ответ.
    std::string HandleCommand( SessionId id, const std::string& command, SessionWeakPtr session );

    /// @brief Отключение клиента.
    /// @param[in] id ID сессии.
    void Disconnect( SessionId id );

private:
    /// @brief Авторизация клиента.
    /// @param[in] id ID сессии.
    /// @param[in] name Имя пользователя.
    /// @param[in] session Слабая ссылка на сессию.
    /// @return Код возврата.
    BrokerCodes Login( SessionId id, const std::string& name, SessionWeakPtr session );

    /// @brief Регистрация клиента.
    /// @param[in] id ID сессии.
    /// @param[in] name Имя пользователя.
    /// @param[in] session Слабая ссылка на сессию.
    /// @return Код возврата.
    BrokerCodes Register( SessionId id, const std::string& name, SessionWeakPtr session );

    /// @brief Публикация от клиента.
    /// @param[in] id ID сессии.
    /// @param[in] topic Топик.
    /// @param[in] message Сообщение.
    /// @return Код возврата.
    BrokerCodes Publish( SessionId id, const Topic& topic, const std::string& message );

    /// @brief Подписка от клиента.
    /// @param[in] id ID сессии.
    /// @param[in] topic Топик.
    /// @return Код возврата.
    BrokerCodes Subscribe( SessionId id, const Topic& topic );

    /// @brief Создание топика.
    /// @param[in] id ID сессии.
    /// @param[in] topic Топик.
    /// @return Код возврата.
    BrokerCodes Create( SessionId id, const Topic& topic  );

    /// @brief отписка от топика.
    /// @param[in] id ID сессии.
    /// @param[in] topic Топик.
    /// @return Код возврата.
    BrokerCodes Unsubscribe( SessionId id, const Topic& topic );

private:
    ClientManagerPtr cliManager_;       /// Менеджер клиентов.
    TopicManagerPtr topManager_;        /// Менеджер топиков.
};
