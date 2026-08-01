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

    ~Logger();

    void SetQueue( const LogLevels level, const char* file, const int line, const std::string& msg );

private:
    std::ofstream out_;
    std::queue< std::string > queue_;
    std::mutex mutex_;
    std::atomic< bool > stop_;
    std::thread writer_;

    void Log();
};

using LoggerPtr = std::unique_ptr< Logger >;
