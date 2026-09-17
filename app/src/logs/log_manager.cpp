/// @file log_manager.cpp
///
/// Класс менеджера логирования. Реализация.
///


#include <ios>
#include <memory>
#include <thread>
#include <sstream>

#include "log_manager.h"
#include "log_configs.h"


std::unique_ptr< LogManager > LogManager::instance_ = nullptr; /// Инстанс.

namespace 
{
/// @brief Перевести строковую конфигурацию в LogLevels.
/// @param[in] level Строка конфигурации.
/// @return LogLevels.
LogLevels ConfigToLevel( const std::string& level )
{
    if ( level == "debug" )
    {
        return LogLevels::DEBUG;
    }
    else if ( level == "info" ) 
    {
        return LogLevels::INFO;
    }
    else if ( level == "warning" ) 
    {
        return LogLevels::WARNING;
    }
    else if ( level == "error" ) 
    {
        return LogLevels::ERROR;
    }
    else if ( level == "fatal" ) 
    {
        return LogLevels::FATAL;
    }
    else 
    {
        /// Дефолтный.
        return LogLevels::WARNING;
    }
}

} // anonimous namespace


LogManager::LogManager( const std::string& file, const std::string& level )
{
    level_ = ConfigToLevel( level );
    logger_ = std::make_unique< Logger >( file );
    shellPrinter_ = std::make_unique< ShellPrinter >();
}

void LogManager::Log( const LogLevels level, LogDirection direction, const char* file, int line, std::stringstream& stream )
{
    std::string msg = stream.str();
    if ( level >= level_ )
    {
        if ( direction == LogDirection::CONSOLE || direction == LogDirection::ALL )
        {
            shellPrinter_->SetQueue( msg );
        }
        if ( direction == LogDirection::LOG || direction == LogDirection::ALL )
        {
            logger_->SetQueue( level, file,line, msg );
        }
    }
}

void LogManager::Init( const std::string& file, const std::string& loglevel ) 
{
    if ( !instance_ ) 
    {
        instance_.reset( new LogManager( file, loglevel ) );
    }
}

LogManager& LogManager::Instance()
{
    if ( !instance_ ) 
    {
        Init( "app.log", "warning" ); 
    }
    return *instance_;
}
