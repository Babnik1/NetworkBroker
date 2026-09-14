/// @file shell_printer.cpp
///
/// Класс логгера, пишущего в консоль. Реализация.
///


#include "shell_printer.h"
#include <iostream>


ShellPrinter::ShellPrinter()
{
    stop_.store( false );
    writer_ = std::thread( &ShellPrinter::Log, this );
}

/// @todo Conditional Variable, чтобы спать пока не придет, обрабатывать и потом снова спать.
void ShellPrinter::Log()
{
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
            std::cout << message << std::endl;
        }
        else 
        {
            std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
        }
    }
}

void ShellPrinter::SetQueue( const std::string& msg )
{
    std::lock_guard< std::mutex > lock( mutex_ );
    queue_.push( msg );
}

ShellPrinter::~ShellPrinter()
{
    stop_.store( true );
    if ( writer_.joinable() )
    {
        writer_.join();
    }
}