find_package( GTest REQUIRED )

set(TEST_SOURCES
    broker/message_broker_test.cpp
)

set(APP_SOURCES_FOR_TESTS
    ../src/broker/message_broker.cpp
    ../src/clients/clients_manager.cpp
    ../src/clients/client.cpp
    ../src/topics/topic_manager.cpp
    ../src/logs/log_manager.cpp 
    ../src/logs/shell_printer.cpp 
    ../src/logs/logger.cpp
    ../src/server/session.cpp
)

add_executable( BrokerTests ${TEST_SOURCES} ${APP_SOURCES_FOR_TESTS} )

target_include_directories(BrokerTests PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}   
    ${CMAKE_CURRENT_SOURCE_DIR}/..    
    ${CMAKE_CURRENT_SOURCE_DIR}/../src 
    ${Boost_INCLUDE_DIRS}
    ${GTEST_INCLUDE_DIRS}
)

target_link_libraries( BrokerTests PRIVATE 
    nlohmann_json::nlohmann_json
    GTest::GTest
    GTest::Main
)

add_test( NAME AllBrokerTests COMMAND BrokerTests )