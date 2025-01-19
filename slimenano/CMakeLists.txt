cmake_minimum_required(VERSION 3.5.0)
project(
    slimenano
    VERSION 1.0.0
    DESCRIPTION "This is a slimenano utils package"
    LANGUAGES CXX)

add_compile_definitions(_SLIMENANO_LIBRARY)

set(NAMESPACE Slimenano)
set(LIB_NAME Package)

set(GLOBAL_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

add_library(${LIB_NAME} INTERFACE)

add_library(${NAMESPACE}::${LIB_NAME} ALIAS ${LIB_NAME})

add_subdirectory(math)
target_link_libraries(${LIB_NAME} INTERFACE Slimenano::Math)

add_subdirectory(ui)
target_link_libraries(${LIB_NAME} INTERFACE Slimenano::UI)

add_subdirectory(event)
target_link_libraries(${LIB_NAME} INTERFACE Slimenano::Event)

get_target_property(PackageAvailableLibraries Slimenano::Package INTERFACE_LINK_LIBRARIES)

set(SLIMENANO_PACKAGE_AVAILABLE_LIBRARIES "")

foreach (AvailableLibrary IN LISTS PackageAvailableLibraries)

    string(REPLACE "::" "" AvailableLibraryConfigName ${AvailableLibrary})

    set(SLIMENANO_PACKAGE_IMPORT_COMMAND "")

    string(APPEND SLIMENANO_PACKAGE_IMPORT_COMMAND [=[if(NOT TARGET ]=])
    string(APPEND SLIMENANO_PACKAGE_IMPORT_COMMAND ${AvailableLibrary})
    string(APPEND SLIMENANO_PACKAGE_IMPORT_COMMAND ")\n")
    string(APPEND SLIMENANO_PACKAGE_IMPORT_COMMAND [=[    include(${CMAKE_CURRENT_LIST_DIR}/]=])
    string(APPEND SLIMENANO_PACKAGE_IMPORT_COMMAND ${AvailableLibraryConfigName})
    string(APPEND SLIMENANO_PACKAGE_IMPORT_COMMAND "Targets.cmake)\n")
    string(APPEND SLIMENANO_PACKAGE_IMPORT_COMMAND [=[endif(NOT TARGET ]=])
    string(APPEND SLIMENANO_PACKAGE_IMPORT_COMMAND ${AvailableLibrary})
    string(APPEND SLIMENANO_PACKAGE_IMPORT_COMMAND ")\n")

    string(APPEND SLIMENANO_PACKAGE_AVAILABLE_LIBRARIES "${SLIMENANO_PACKAGE_IMPORT_COMMAND}\n")

endforeach (AvailableLibrary IN LISTS PackageAvailableLibraries)

configure_file(${PROJECT_SOURCE_DIR}/cmake/${NAMESPACE}Config.cmake.in ${PROJECT_SOURCE_DIR}/cmake/${NAMESPACE}Config.cmake @ONLY)

install(
        FILES cmake/${NAMESPACE}Config.cmake
        DESTINATION lib/cmake/${NAMESPACE}
)

install(TARGETS ${LIB_NAME} EXPORT ${NAMESPACE}${LIB_NAME}Targets
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
)

install(EXPORT ${NAMESPACE}${LIB_NAME}Targets
        NAMESPACE ${NAMESPACE}::
        DESTINATION lib/cmake/${NAMESPACE}
)

install(
        FILES ${GLOBAL_INCLUDE_DIR}/slimenano-api.h
        DESTINATION include/${NAMESPACE}
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