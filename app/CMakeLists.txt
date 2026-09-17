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

target_include_directories( Broker PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} )

target_include_directories( Broker PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${Boost_INCLUDE_DIRS}
)

target_link_libraries( Broker PRIVATE 
    CONAN_PKG::nlohmann_json
    CONAN_PKG::boost
)

set_target_properties( Broker PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR} )

if( ENABLE_TESTS )
    enable_testing()
    add_subdirectory( tests )
 endif()
