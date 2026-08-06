#pragma once

/// @file log_manager.h
///
/// Структуры конфигурации логгера.
///

#include <map>
#include <string>


/// @brief Перечисление уровней логирования.
enum class LogLevels
{
    DEBUG   = 0,
    INFO    = 1,
    WARNING = 2,
    ERROR   = 3,
    FATAL   = 4
};


/// @brief Перечислений мест назначение вывода.
enum class LogDirection
{
    CONSOLE = 0,    /// Вывод в консоль.
    LOG     = 1,    /// Вывод в файл.
    ALL     = 2,    /// Вывод и в консоль и в файл.
};


/// @brief Карта преобразования перечислений уровней логирования в строку.
const std::map< LogLevels, std::string > loggerMap
{
    { LogLevels::DEBUG,     "DEBUG"     },
    { LogLevels::INFO,      "INFO"      },
    { LogLevels::WARNING,   "WARNING"   },
    { LogLevels::ERROR,     "ERROR"     },
    { LogLevels::FATAL,     "FATAL"     }
};
