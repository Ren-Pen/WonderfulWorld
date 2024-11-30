cmake_minimum_required(VERSION 3.5.0)
project(WonderfulWorld)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

if (WIN32)
    set(CMAKE_SHARED_LIBRARY_PREFIX "")
    set(CMAKE_STATIC_LIBRARY_PREFIX "")
endif ()

set(BUILD_SHARED_LIBS ON)

add_subdirectory(slimenano)
add_subdirectory(main)