cmake_minimum_required(VERSION 3.5.0)
set(NAMESPACE Slimenano)
set(PACKAGE_LIB_NAME Package)

project(
        ${NAMESPACE}${PACKAGE_LIB_NAME}
        VERSION 1.0.0
        DESCRIPTION "This is a ${NAMESPACE} package library"
        LANGUAGES CXX)

add_compile_definitions(SLIMENANO_LIBRARY)

include(../slimenano-cmake-utils/SlimenanoCmakeUtils.cmake)
slimenano_initialize_package_project(${NAMESPACE} ${PACKAGE_LIB_NAME})
