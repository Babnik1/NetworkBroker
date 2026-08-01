#pragma once

/// @file log_manager.h
///
/// Класс менеджера логирования. Объявление.
///

#include "log_configs.h"
#include "logger.h"
#include "shell_printer.h"


class LogManager
{
public:

    void Log( const LogLevels level, LogDirection direction, const char* file, int line, std::stringstream& stream );

    static LogManager& Instance();

    static void Init( const std::string& file );

    LogManager( const LogManager& ) = delete;

    LogManager& operator=( const LogManager& ) = delete;

private:
    LoggerPtr logger_;
    ShellPrinterPtr shellPrinter_;

    static std::unique_ptr< LogManager > instance_;

    LogManager( const std::string& file );
};