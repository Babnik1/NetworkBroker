#pragma once

/// @file rc.h
/// 
/// Кооды возврата брокера.
///


/// @brief Коды возврата брокера.
enum class BrokerCodes
{
    Ok                      = 0,    /// Успешно.

    ClientAlreadyExists     = 1,    /// Клиент уже существует.
    ClientNotFound          = 2,    /// Клиент не найден.
    ClientAlreadyConnected  = 3,    /// Клиент уже подключен.

    TopicAlreadyExists      = 4,    /// Тема уже существует.
    TopicNotFound           = 5,    /// Тема не найдена.

    AlreadySubscribed       = 10,   /// Уже подписан.
    NotSubscribed           = 6,    /// Не подписан.

    InvalidCommand          = 7,    /// Неверная команда.
    Unauthorized            = 8,    /// Не авторизован.

    InternalError           = 9,    /// Другая ошибка.

    Unloged                 = 11,   /// Клиент успешно разлогинен.
};
