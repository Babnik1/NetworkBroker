cmake_minimum_required( VERSION 3.12 )
option( ENABLE_TESTS "Enable tests" ON )

project( Broker VERSION 1.0 LANGUAGES CXX )

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

find_package( Boost REQUIRED COMPONENTS program_options filesystem system )
find_package( nlohmann_json REQUIRED )

set( SOURCES 
        logs/log_manager.cpp 
        logs/shell_printer.cpp 
        logs/logger.cpp
        server/server.cpp
        configurator/configurator.cpp
        main.cpp 
)

add_executable( Broker ${SOURCES} )

target_include_directories( Broker PRIVATE ${CMAKE_SOURCE_DIR} )

target_include_directories( Broker PRIVATE 
    logs 
    server
    sessions
    configurator
)

target_include_directories( Broker PRIVATE ${Boost_INCLUDE_DIRS} )

target_link_libraries( Broker PRIVATE 
    nlohmann_json::nlohmann_json
#    Boost::program_options 
#    Boost::filesystem
#    Boost::system
)

set_target_properties( Broker PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR} )

#if( ENABLE_TESTS )
#    enable_testing()
#    add_subdirectory( tests )
# endif()
