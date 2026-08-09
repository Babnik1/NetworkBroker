#pragma once

/// @file rc.h
///
/// Коды возврата менеджера клиента.
///


/// @brief Перечисление кодов возврата менеджера клиента.
enum class ClientsCodes
{
    Ok                      = 0,    /// Успешно.

    ClientAlreadyExists     = 1,    /// Клиент уже существует.
    ClientNotFound          = 2,    /// Клиент не найден.
    ClientAlreadyConnected  = 3,    /// Клиент уже подключен.
    Unauthorized            = 8,    /// Не авторизован.

    InternalError           = 9,    /// Другая ошибка.
};