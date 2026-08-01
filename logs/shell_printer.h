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

class ShellPrinter
{
public:
    ShellPrinter();

    ~ShellPrinter();

    void SetQueue( const std::string& msg );

private:
    std::queue< std::string > queue_;
    std::mutex mutex_;
    std::atomic< bool > stop_;
    std::thread writer_;

    void Log();
};

using ShellPrinterPtr = std::unique_ptr< ShellPrinter >;