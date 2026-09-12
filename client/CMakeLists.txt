set( CLIENT_SOURCES
    main.cpp
)

add_executable( BrokerClient ${CLIENT_SOURCES} )

target_link_libraries( BrokerClient PRIVATE
    CONAN_PKG::boost
)

set_target_properties( BrokerClient PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
)