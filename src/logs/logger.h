#pragma once

/// @file logger.h
///
/// Класс логгера, пишущего в файл - лог. Объявление.
///

#include "log_configs.h"
#include <atomic>
#include <memory>
#include <queue>
#include <mutex>
#include <fstream>
#include <thread>


/// @brief Класс логгера.
class Logger
{
public:
    /// @brief Конструктор.
    Logger( const std::string& file );

    /// @brief Деструктор.
    ~Logger();

    /// @brief Поставить в очередь на запись в лог-файл.
    /// @param[in] level Уровень логирования.
    /// @param[in] file Файл, где произошло событие.
    /// @param[in] line Строка.
    /// @param[in] msg Сообщение.
    void SetQueue( const LogLevels level, const char* file, const int line, const std::string& msg );

private:
    std::ofstream out_;                 /// Дескриптор файла.
    std::queue< std::string > queue_;   /// Очередь на вывод.
    std::mutex mutex_;                  /// Мьютекс вывода.
    std::atomic< bool > stop_;          /// Сигнал об остановке программы.
    std::thread writer_;                /// Поток записи.

    /// @brief Залогировать сообщение.
    void Log();
};

using LoggerPtr = std::unique_ptr< Logger >;
