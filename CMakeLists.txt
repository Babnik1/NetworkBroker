cmake_minimum_required( VERSION 3.30 )
cmake_policy( SET CMP0167 NEW )

option( ENABLE_TESTS "Enable tests" ON )

set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

project( Broker VERSION 1.0 LANGUAGES CXX )

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

include( ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake )
conan_basic_setup( TARGETS )

add_subdirectory( app )

add_subdirectory( client )