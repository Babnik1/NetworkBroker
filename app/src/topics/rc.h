#pragma once

/// @file rc.h
///
/// Коды возврата топик-менеджера.
///

/// @brief Перечисление кодов возврата топик менеджера.
enum class TopicCodes
{
    Ok                      = 0,    /// Успешно.

    TopicAlreadyExists      = 4,    /// Тема уже существует.
    TopicNotFound           = 5,    /// Тема не найдена.

    AlreadySubscribed       = 10,   /// Уже подписан.
    NotSubscribed           = 6,    /// Не подписан.

    InternalError           = 9,    /// Другая ошибка.
};