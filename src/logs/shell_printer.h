#pragma once

/// @file shell_printer.h
///
/// Класс логгера, пишущего в консоль. Объявление.
///

#include "log_configs.h"
#include "log_configs.h"
#include <atomic>
#include <memory>
#include <queue>
#include <mutex>
#include <thread>


/// @brief Класс вывода сообщений в консоль сервера.
class ShellPrinter
{
public:

    /// @brief Конструктор.
    ShellPrinter();

    /// @brief Деструктор.
    ~ShellPrinter();

    /// @brief Поставить в очередь на запись в консоль.
    /// @param[in] msg Сообщение.
    void SetQueue( const std::string& msg );

private:
    std::queue< std::string > queue_;       /// Очередь сообщений.
    std::mutex mutex_;                      /// Мьютекс вывода.
    std::atomic< bool > stop_;              /// Сигнал об остановке программы.
    std::thread writer_;                    /// Пишущий поток.

    /// @brief Залогировать сообщение.
    void Log();
};

using ShellPrinterPtr = std::unique_ptr< ShellPrinter >;