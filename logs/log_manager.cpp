/// @file log_manager.cpp
///
/// Класс менеджера логирования. Реализация.
///


#include <ios>
#include <memory>
#include <thread>
#include <sstream>

#include "log_manager.h"

namespace logs
{

std::unique_ptr< LogManager > LogManager::instance_ = nullptr;


LogManager::LogManager( const std::string& file )
{
    logger_ = std::make_unique< Logger >( file );
    shellPrinter_ = std::make_unique< ShellPrinter >();
}

void LogManager::Log( const LogLevels level, LogDirection direction, const char* file, int line, std::stringstream& stream )
{
    std::string msg = stream.str();
    if ( direction == LogDirection::CONSOLE || direction == LogDirection::ALL )
    {
        shellPrinter_->SetQueue( msg );
    }
    if ( direction == LogDirection::LOG || direction == LogDirection::ALL )
    {
        logger_->SetQueue( level, file,line, msg );
    }
}

void LogManager::Init( const std::string& file ) 
{
    if ( !instance_ ) 
    {
        instance_.reset( new LogManager( file ) );
    }
}

LogManager& LogManager::Instance()
{
    if ( !instance_ ) 
    {
        Init("app.log"); 
    }
    return *instance_;
}

} // namespace logs