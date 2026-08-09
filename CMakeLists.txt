cmake_minimum_required( VERSION 3.12 )
option( ENABLE_TESTS "Enable tests" ON )

set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

project( Broker VERSION 1.0 LANGUAGES CXX )

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

find_package( Boost REQUIRED COMPONENTS program_options filesystem system )
find_package( nlohmann_json REQUIRED )

set( SOURCES 
        src/configurator/configurator.cpp
        src/logs/log_manager.cpp 
        src/logs/shell_printer.cpp 
        src/logs/logger.cpp
        src/server/server.cpp
        src/server/session.cpp 
        src/broker/message_broker.cpp
        src/clients/clients_manager.cpp
        src/clients/client.cpp
        src/topics/topic_manager.cpp 
        src/db/json_client_repository.cpp 
        src/db/json_topic_repository.cpp
        main.cpp 
)

add_executable( Broker ${SOURCES} )

add_subdirectory( client )

target_include_directories( Broker PRIVATE ${CMAKE_SOURCE_DIR} )

target_include_directories( Broker PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${Boost_INCLUDE_DIRS}
)
target_link_libraries( Broker PRIVATE 
    nlohmann_json::nlohmann_json
)

set_target_properties( Broker PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR} )

if( ENABLE_TESTS )
    enable_testing()
    add_subdirectory( tests )
 endif()
