cmake_minimum_required(VERSION 3.5.0)
project(
    Slimenano-math
    VERSION 1.0.0
    DESCRIPTION "This is a Slimenano free math library"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(GLOBAL_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/../include)

set(SRC_FILES
    ${SRC_DIR}/slm.cpp
)

set(NAMESPACE Slimenano)
set(LIB_NAME Math)

add_library(${LIB_NAME} ${SRC_FILES})

target_include_directories(${LIB_NAME} PUBLIC
    $<BUILD_INTERFACE:${SRC_DIR}>
    $<BUILD_INTERFACE:${GLOBAL_INCLUDE_DIR}>
    $<INSTALL_INTERFACE:include/${NAMESPACE}>
)

if(MSVC)
    target_compile_options(${LIB_NAME} PRIVATE /W4 /WX)
else()
    target_compile_options(${LIB_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

add_library(${NAMESPACE}::${LIB_NAME} ALIAS ${LIB_NAME})

install(TARGETS ${LIB_NAME} EXPORT ${NAMESPACE}${LIB_NAME}Targets
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(FILES ${SRC_DIR}/slm.h
    DESTINATION include/${NAMESPACE}
)

install(FILES ${SRC_DIR}/slm/slm-vector.h
    DESTINATION include/${NAMESPACE}/slm
)


install(EXPORT ${NAMESPACE}${LIB_NAME}Targets
    NAMESPACE ${NAMESPACE}::
    DESTINATION lib/cmake/${NAMESPACE}
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${NAMESPACE}${LIB_NAME}ConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/${NAMESPACE}${LIB_NAME}ConfigVersion.cmake
    DESTINATION lib/cmake/${NAMESPACE}
)