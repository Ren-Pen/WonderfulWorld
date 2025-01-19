cmake_minimum_required(VERSION 3.5.0)
project(
        slimenano
        VERSION 1.0.0
        DESCRIPTION "This is a slimenano utils package"
        LANGUAGES CXX)

add_compile_definitions(SLIMENANO_LIBRARY)

set(NAMESPACE Slimenano)
set(LIB_NAME Package)

set(GLOBAL_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

add_library(${LIB_NAME} INTERFACE)

add_library(${NAMESPACE}::${LIB_NAME} ALIAS ${LIB_NAME})

set(PackageFullLibraries "")

file(GLOB subdirectory_list RELATIVE ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/*)
foreach (subdirectory ${subdirectory_list})
    if (IS_DIRECTORY ${PROJECT_SOURCE_DIR}/${subdirectory})
        if (EXISTS ${PROJECT_SOURCE_DIR}/${subdirectory}/CMakeLists.txt)
            list(APPEND PackageFullLibraries ${subdirectory})
        endif (EXISTS ${PROJECT_SOURCE_DIR}/${subdirectory}/CMakeLists.txt)
    endif (IS_DIRECTORY ${PROJECT_SOURCE_DIR}/${subdirectory})
endforeach (subdirectory ${subdirectory_list})

foreach (RegistryLibrary ${PackageFullLibraries})

    string(TOUPPER ${RegistryLibrary} LibraryOptionName)

    option(SLIMENANO_PACKAGE_INCLUDE_${LibraryOptionName} "Add Slimenano::${RegistryLibrary} library to Slimenano::Package" ON)

    message(STATUS "ADDED OPTION: SLIMENANO_PACKAGE_INCLUDE_${LibraryOptionName}")

    if (${SLIMENANO_PACKAGE_INCLUDE_${LibraryOptionName}})

        add_subdirectory(${RegistryLibrary})

        target_link_libraries(${LIB_NAME} INTERFACE Slimenano::${RegistryLibrary})

    endif (${SLIMENANO_PACKAGE_INCLUDE_${LibraryOptionName}})

endforeach (RegistryLibrary ${PackageFullLibraries})
#add_subdirectory(UI)
#target_link_libraries(${LIB_NAME} INTERFACE Slimenano::UI)
#
#add_subdirectory(event)
#target_link_libraries(${LIB_NAME} INTERFACE Slimenano::Event)

set(SLIMENANO_PACKAGE_AVAILABLE_LIBRARIES "")

get_target_property(PackageAvailableLibraries Slimenano::Package INTERFACE_LINK_LIBRARIES)

if (NOT PackageAvailableLibraries STREQUAL "PackageAvailableLibraries-NOTFOUND")

    foreach (AvailableLibrary IN LISTS PackageAvailableLibraries)

        string(REPLACE "::" "" AvailableLibraryConfigName ${AvailableLibrary})

        set(SLIMENANO_PACKAGE_IMPORT_COMMAND "")

        message(STATUS "DETECTED ACTIVE LIBRARY IN PACKAGE: ${AvailableLibrary}")

        string(APPEND SLIMENANO_PACKAGE_IMPORT_COMMAND [=[if(NOT TARGET ]=])
        string(APPEND SLIMENANO_PACKAGE_IMPORT_COMMAND ${AvailableLibrary})
        string(APPEND SLIMENANO_PACKAGE_IMPORT_COMMAND ")\n")
        string(APPEND SLIMENANO_PACKAGE_IMPORT_COMMAND [=[    include(${CMAKE_CURRENT_LIST_DIR}/]=])
        string(APPEND SLIMENANO_PACKAGE_IMPORT_COMMAND ${AvailableLibraryConfigName})
        string(APPEND SLIMENANO_PACKAGE_IMPORT_COMMAND "Targets.cmake)\n")
        string(APPEND SLIMENANO_PACKAGE_IMPORT_COMMAND [=[endif(NOT TARGET ]=])
        string(APPEND SLIMENANO_PACKAGE_IMPORT_COMMAND ${AvailableLibrary})
        string(APPEND SLIMENANO_PACKAGE_IMPORT_COMMAND ")")

        list(APPEND SLIMENANO_PACKAGE_AVAILABLE_LIBRARIES "${SLIMENANO_PACKAGE_IMPORT_COMMAND}")

        unset(SLIMENANO_PACKAGE_IMPORT_COMMAND)
        unset(AvailableLibraryConfigName)

    endforeach (AvailableLibrary IN LISTS PackageAvailableLibraries)

endif (NOT PackageAvailableLibraries STREQUAL "PackageAvailableLibraries-NOTFOUND")

string(JOIN "\n\n" SLIMENANO_PACKAGE_AVAILABLE_LIBRARIES ${SLIMENANO_PACKAGE_AVAILABLE_LIBRARIES})

configure_file(${PROJECT_SOURCE_DIR}/cmake/${NAMESPACE}Config.cmake.in ${PROJECT_SOURCE_DIR}/cmake/${NAMESPACE}Config.cmake @ONLY)

unset(SLIMENANO_PACKAGE_AVAILABLE_LIBRARIES)

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