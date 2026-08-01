/// @file logger.h
///
/// Класс логгера, пишущего в файл - лог. Реализация.
///


#include "logger.h"
#include <mutex>
#include <thread>
#include <iostream>


Logger::Logger( const std::string& file )
{
    stop_.store( false );
    out_.open( file, std::ios::app );
    writer_ = std::thread( &Logger::Log, this );
}


/// @todo Conditional Variable, чтобы спать пока не придет, обрабатывать и потом снова спать.
/// @todo Обрубать только названия файла а не весь путь, для красоты.
/// @todo Добавить время срабатывания.
void Logger::Log()
{
    if ( !out_.is_open() )
    {
        return;
    }

    while ( !stop_.load() || !queue_.empty() )
    {
        std::string message = "";
        bool hasData = false;
        
        mutex_.lock();
        if ( !queue_.empty() )
        {
            message = queue_.front();
            queue_.pop();
            hasData = true;
        }
        mutex_.unlock();

        if ( hasData )
        {
            out_ << message << std::endl;
        }
        else 
        {
            std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
        }
    }
}

void Logger::SetQueue( const LogLevels level, const char* file, const int line, const std::string& msg )
{
    std::string result = std::string( file ) + " - " +  std::to_string( line ) + " [ " + loggerMap.find( level )->second + " ] " + msg;
    std::lock_guard< std::mutex > lock( mutex_ );
    queue_.push( result );
    
}

Logger::~Logger()
{
    stop_.store( true );
    if ( writer_.joinable() )
    {
        writer_.join();
    }

    if ( out_.is_open() )
    {
        out_.close();
    }
}