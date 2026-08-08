#pragma once

/// @file log_manager.h
///
/// Класс менеджера логирования. Объявление.
///

#include "log_configs.h"
#include "logger.h"
#include "shell_printer.h"


/// @brief Класс логирования сообщений.
class LogManager
{
public:
    /// @brief Логировать сообщение.
    /// @param[in] level Уровень логирования.
    /// @param[in] direction Направление вывода.
    /// @param[in] file Файл, где произошло событие.
    /// @param[in] line Строка.
    /// @param[in] stream Стрим.
    void Log( const LogLevels level, LogDirection direction, const char* file, int line, std::stringstream& stream );

    /// @brief Получить инстанс объекта.
    /// @return Интанс объекта.
    static LogManager& Instance();

    /// @brief Инициализация логгера.
    /// @param[in] file Лог-файл.
    /// @param[in] loglevel Уровень логирования.
    static void Init( const std::string& file, const std::string& loglevel );

    /// @brief Удаленный конструтор перемещения.
    LogManager( const LogManager& ) = delete;

    /// @brief Удаленный конструктор копирования.
    LogManager& operator=( const LogManager& ) = delete;

private:
    LoggerPtr logger_;                                  /// Обработчик записи в файл.
    ShellPrinterPtr shellPrinter_;                      /// Обработчик записи в консоль.
    LogLevels level_;                                   /// Уровень логирования.

    static std::unique_ptr< LogManager > instance_;     /// Инстанс.

    /// @brief Конструктор.
    /// @param[in] file Лог-файл.
    /// @param[in] loglevel Уровень логирования.
    LogManager( const std::string& file, const std::string& loglevel );      
};
