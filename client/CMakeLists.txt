set( CLIENT_SOURCES
    main.cpp
)

add_executable( BrokerClient ${CLIENT_SOURCES} )

target_include_directories( BrokerClient PRIVATE
    ${Boost_INCLUDE_DIRS}
)

set_target_properties( BrokerClient PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
)