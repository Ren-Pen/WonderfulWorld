cmake_minimum_required(VERSION 3.5.0)
project(
    slimenano
    VERSION 1.0.0
    DESCRIPTION "This is a slimenano utils package"
    LANGUAGES CXX)

add_compile_definitions(_SLIMENANO_LIBRARY)

add_subdirectory(math)

set(NAMESPACE Slimenano)

set(GLOBAL_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

install(
    FILES cmake/${NAMESPACE}Config.cmake
    DESTINATION lib/cmake/${NAMESPACE}
)

install(
    FILES ${GLOBAL_INCLUDE_DIR}/slimenano-api.h
    DESTINATION include/${NAMESPACE}
)

add_library(SlimenanoPackage INTERFACE)