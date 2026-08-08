/// @file logger.h
///
/// Класс логгера, пишущего в файл - лог. Реализация.
///


#include "logger.h"
#include <mutex>
#include <thread>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>


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
    std::string fullPath( file );
    size_t lastSlash = fullPath.find_last_of( "/\\" );
    std::string fileName = ( lastSlash == std::string::npos ) ? fullPath : fullPath.substr( lastSlash + 1 );
    std::string fileLocation = fileName + ":" + std::to_string( line );

    auto now = std::chrono::system_clock::now();
    auto timeTime = std::chrono::system_clock::to_time_t( now );
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>( now.time_since_epoch() ) % 1000;

    std::tm buf;
#if defined(_WIN32) || defined(_WIN64)
    localtime_s( &buf, &timeTime ); // Windows
#else
    localtime_r( &timeTime, &buf ); // Linux / macOS
#endif

    std::ostringstream timeStream;
    timeStream << std::put_time( &buf, "%H:%M:%S" ) << '.' << std::setfill( '0' ) << std::setw( 3 ) << ms.count();

    std::ostringstream logStream;
    
    logStream << "[ " << timeStream.str() << " ]  ";
    
    logStream << std::left << std::setw( 35 ) << fileLocation;
    
    std::string levelStr = "[ " + loggerMap.find( level )->second + " ]";
    logStream << std::left << std::setw( 11 ) << levelStr;
    
    logStream << msg;

    std::string result = logStream.str();

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