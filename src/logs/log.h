#pragma once
/// @file log.h
///
/// Объявление макросов для логирования.
///


#include <sstream>
#include "log_manager.h"


/// @brief Вспомогательный макрос.
#define LOG_INTERNAL( level, direction ) \
    if ( false ) {} else \
    for ( struct { std::stringstream ss; bool done; } _s = { {}, false }; \
          !_s.done; \
          LogManager::Instance().Log( level, direction, __FILE__, __LINE__, _s.ss ), _s.done = true ) \
        _s.ss


/// @brief Макросы записи в файл.
#define DEBUG_LOG( msg )   LOG_INTERNAL( LogLevels::DEBUG,   LogDirection::LOG ) << msg
#define INFO_LOG( msg )    LOG_INTERNAL( LogLevels::INFO,    LogDirection::LOG ) << msg
#define WARNING_LOG( msg ) LOG_INTERNAL( LogLevels::WARNING, LogDirection::LOG ) << msg
#define ERROR_LOG( msg )   LOG_INTERNAL( LogLevels::ERROR,   LogDirection::LOG ) << msg
#define FATAL_LOG( msg )   LOG_INTERNAL( LogLevels::FATAL,   LogDirection::LOG ) << msg

/// @brief Макросы записи в файл и в консоль.
#define DEBUG_ALL( msg )   LOG_INTERNAL( LogLevels::DEBUG,   LogDirection::ALL ) << msg
#define INFO_ALL( msg )    LOG_INTERNAL( LogLevels::INFO,    LogDirection::ALL ) << msg
#define WARNING_ALL( msg ) LOG_INTERNAL( LogLevels::WARNING, LogDirection::ALL ) << msg
#define ERROR_ALL( msg )   LOG_INTERNAL( LogLevels::ERROR,   LogDirection::ALL ) << msg
#define FATAL_ALL( msg )   LOG_INTERNAL( LogLevels::FATAL,   LogDirection::ALL ) << msg

/// @brief Макросы записи в консоль.
#define DEBUG_SHELL( msg )   LOG_INTERNAL( LogLevels::DEBUG,   LogDirection::CONSOLE ) << msg
#define INFO_SHELL( msg )    LOG_INTERNAL( LogLevels::INFO,    LogDirection::CONSOLE ) << msg
#define WARNING_SHELL( msg ) LOG_INTERNAL( LogLevels::WARNING, LogDirection::CONSOLE ) << msg
#define ERROR_SHELL( msg )   LOG_INTERNAL( LogLevels::ERROR,   LogDirection::CONSOLE ) << msg
#define FATAL_SHELL( msg )   LOG_INTERNAL( LogLevels::FATAL,   LogDirection::CONSOLE ) << msg