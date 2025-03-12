function(slimenano_get_sub_project_simple_name DIR OUT)
    file( GLOB SLIMENANO_PROJECT_FILES RELATIVE ${DIR} ${DIR}/*.slimenano.project )
    list(LENGTH SLIMENANO_PROJECT_FILES PROJECT_FILES_SIZE)
    if(PROJECT_FILES_SIZE GREATER 1)
        message(FATAL_ERROR "[SLIMENANO] Slimenano sub project only allow one project define file.\n[SLIMENANO] Sub project: ${DIR}")
    elseif (PROJECT_FILES_SIZE EQUAL 1)
        list(GET SLIMENANO_PROJECT_FILES 0 PROJECT_SIMPLE_NAME)
        string(REPLACE ".slimenano.project" "" PROJECT_SIMPLE_NAME ${PROJECT_SIMPLE_NAME})
        set(${OUT} ${PROJECT_SIMPLE_NAME} PARENT_SCOPE)
    else ()
        set(${OUT} "" PARENT_SCOPE)
    endif ()
endfunction()

function(slimenano_get_sub_project_configuration RegistryLibrary DIR SLIMENANO_PROJECT_SUB_CONTENT)
    file( GLOB SLIMENANO_PROJECT_FILES RELATIVE ${DIR} ${DIR}/*.slimenano.project )
    list(LENGTH SLIMENANO_PROJECT_FILES PROJECT_FILES_SIZE)
    if(PROJECT_FILES_SIZE GREATER 1)
        message(FATAL_ERROR "[SLIMENANO] Slimenano sub project only allow one project define file.\n[SLIMENANO] Sub project: ${DIR}")
    elseif (PROJECT_FILES_SIZE EQUAL 1)
        list(GET SLIMENANO_PROJECT_FILES 0 PROJECT_SIMPLE_NAME)
        file(STRINGS "${DIR}/${PROJECT_SIMPLE_NAME}" SLIMENANO_PROJECT_FILE_CONTENT)
        set(SLIMENANO_PROJECT_SUB_CONTENT "")
        foreach(LINE IN LISTS SLIMENANO_PROJECT_FILE_CONTENT)
            string(REGEX MATCH "#VERSION ([^\\s]*)" _ "${LINE}")
            if (NOT "${CMAKE_MATCH_1}" STREQUAL "")
                set_property(GLOBAL PROPERTY "SLIMENANO_PROJECT_VERSION_${RegistryLibrary}" "${CMAKE_MATCH_1}")
            endif()
            string(REGEX REPLACE "#.*" "" SLIMENANO_PROJECT_SUB_CONTENT_LINE "${LINE}")
            if (NOT "${SLIMENANO_PROJECT_SUB_CONTENT_LINE}" STREQUAL "")
                string(APPEND SLIMENANO_PROJECT_SUB_CONTENT "${SLIMENANO_PROJECT_SUB_CONTENT_LINE}\n")
            endif()
        endforeach()
        set(SLIMENANO_PROJECT_SUB_CONTENT "${SLIMENANO_PROJECT_SUB_CONTENT}" PARENT_SCOPE)
    endif ()
endfunction()

function(slimenano_scan_sub_project OUT)
    set(PackageFullLibraries "")
    file(GLOB subdirectory_list RELATIVE ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/*)
    foreach (subdirectory ${subdirectory_list})
        if (IS_DIRECTORY ${PROJECT_SOURCE_DIR}/${subdirectory})
            slimenano_get_sub_project_simple_name(${PROJECT_SOURCE_DIR}/${subdirectory} PROJECT_SIMPLE_NAME)
            if (NOT "${PROJECT_SIMPLE_NAME}" STREQUAL "")
                list(APPEND PackageFullLibraries ${subdirectory})
            endif (NOT "${PROJECT_SIMPLE_NAME}" STREQUAL "")
        endif (IS_DIRECTORY ${PROJECT_SOURCE_DIR}/${subdirectory})
    endforeach (subdirectory ${subdirectory_list})
    set(${OUT} ${PackageFullLibraries} PARENT_SCOPE)
endfunction()

function(slimenano_scan_source_file DIR OUT)
    set(SRC_FILES "")
    file(GLOB_RECURSE file_list RELATIVE ${DIR} ${DIR}/*.cpp ${DIR}/*.c)
    foreach (SRC_FILE ${file_list})
        list(APPEND SRC_FILES ${SRC_FILE})
    endforeach (SRC_FILE ${file_list})
    set(${OUT} ${SRC_FILES} PARENT_SCOPE)
endfunction()

function(slimenano_scan_header_file DIR OUT)
    set(SRC_FILES "")
    file(GLOB_RECURSE file_list RELATIVE ${DIR} ${DIR}/*.hpp ${DIR}/*.h)
    foreach (SRC_FILE ${file_list})
        list(APPEND SRC_FILES ${SRC_FILE})
    endforeach (SRC_FILE ${file_list})
    set(${OUT} ${SRC_FILES} PARENT_SCOPE)
endfunction()

function(slimenano_generate_includes_install_commands HEADER_PREFIX DESTINATION_PREFIX HEADER_SCAN_PATH OUT)
    slimenano_scan_header_file("${HEADER_SCAN_PATH}" INCLUDE_FILES)
    set(HEADER_NAMESPACE_LIST "")
    foreach (INCLUDE_FILE ${INCLUDE_FILES})
        string(REPLACE [=[/]=] ";" INCLUDE_PATH_LIST ${INCLUDE_FILE})
        list(GET INCLUDE_PATH_LIST -1 INCLUDE_FILE_NAME)
        list(POP_BACK INCLUDE_PATH_LIST)
        string(JOIN "_" INCLUDE_NAMESPACE ${INCLUDE_PATH_LIST})
        if("${INCLUDE_NAMESPACE}" STREQUAL "")
            set(INCLUDE_NAMESPACE __DEFAULT__)
        endif ("${INCLUDE_NAMESPACE}" STREQUAL "")
        if(NOT DEFINED TEMP_HEADER_NAMESPACE_LIST_MAP_${INCLUDE_NAMESPACE})
            set(TEMP_HEADER_NAMESPACE_LIST_MAP_${INCLUDE_NAMESPACE} "")
            list(APPEND HEADER_NAMESPACE_LIST "${INCLUDE_NAMESPACE}")
        endif (NOT DEFINED TEMP_HEADER_NAMESPACE_LIST_MAP_${INCLUDE_NAMESPACE})
        list(APPEND TEMP_HEADER_NAMESPACE_LIST_MAP_${INCLUDE_NAMESPACE} ${INCLUDE_FILE_NAME})
    endforeach ()

    set(SLIMENANO_PROJECT_INCLUDES_INSTALL "")
    foreach(INCLUDE_NAMESPACE ${HEADER_NAMESPACE_LIST})
        set(INCLUDE_INSTALL_BASE_PATH "")
        set(INCLUDE_INSTALL_FILE_PREFIX "")
        set(INCLUDE_INSTALL_FILE_DESTINATION "")

        if(NOT "${INCLUDE_NAMESPACE}" STREQUAL "__DEFAULT__")
            string(REPLACE "_" [=[/]=] INCLUDE_INSTALL_BASE_PATH "${INCLUDE_NAMESPACE}")
            set(INCLUDE_INSTALL_FILE_PREFIX "${INCLUDE_INSTALL_BASE_PATH}/")
            set(INCLUDE_INSTALL_FILE_DESTINATION "/${INCLUDE_INSTALL_BASE_PATH}")
        endif (NOT "${INCLUDE_NAMESPACE}" STREQUAL "__DEFAULT__")

        string(PREPEND INCLUDE_INSTALL_FILE_PREFIX ${HEADER_PREFIX})

        string(JOIN " ${INCLUDE_INSTALL_FILE_PREFIX}" INSTALL_INCLUDE_FILES ${TEMP_HEADER_NAMESPACE_LIST_MAP_${INCLUDE_NAMESPACE}})
        string(PREPEND INSTALL_INCLUDE_FILES "${INCLUDE_INSTALL_FILE_PREFIX}")

        set(TEMP_INSTALL_COMMAND [=[install(FILES ]=])
        string(APPEND TEMP_INSTALL_COMMAND "\"${INSTALL_INCLUDE_FILES}\"\n")
        string(APPEND TEMP_INSTALL_COMMAND "        DESTINATION ${DESTINATION_PREFIX}")
        string(APPEND TEMP_INSTALL_COMMAND "${INCLUDE_INSTALL_FILE_DESTINATION}")
        string(APPEND TEMP_INSTALL_COMMAND "\n)")

        list(APPEND SLIMENANO_PROJECT_INCLUDES_INSTALL "${TEMP_INSTALL_COMMAND}")
    endforeach ()

    string(JOIN "\n\n" SLIMENANO_PROJECT_INCLUDES_INSTALL ${SLIMENANO_PROJECT_INCLUDES_INSTALL})
    set(${OUT} ${SLIMENANO_PROJECT_INCLUDES_INSTALL} PARENT_SCOPE)
endfunction()

function(slimenano_generate_subproject RegistryLibrary)

    if (NOT EXISTS ${PROJECT_SOURCE_DIR}/${RegistryLibrary})
        message(FATAL_ERROR "[SLIMENANO] Directory '${PROJECT_SOURCE_DIR}/${RegistryLibrary}' not found")
    endif ()

    slimenano_get_sub_project_simple_name(${PROJECT_SOURCE_DIR}/${RegistryLibrary} SLIMENANO_PROJECT_SIMPLE_NAME)

    message(STATUS "[SLIMENANO] Generating subproject: ${RegistryLibrary} Simple-Name: ${SLIMENANO_PROJECT_SIMPLE_NAME}")

    set(SLIMENANO_PROJECT_LIB_NAME "${RegistryLibrary}")

    string(TOUPPER ${RegistryLibrary} SLIMENANO_PROJECT_LIB_NAME_UPPER)

    string(TOLOWER ${RegistryLibrary} SLIMENANO_PROJECT_LIB_NAME_LOWER)

    set(SLIMENANO_PROJECT_LIB_VERSION "SLIMENANO_${SLIMENANO_PROJECT_LIB_NAME_UPPER}_VERSION")

    if(NOT EXISTS ${PROJECT_SOURCE_DIR}/${RegistryLibrary}/src/${SLIMENANO_PROJECT_SIMPLE_NAME}.cpp)
        configure_file(${CMAKE_CURRENT_FUNCTION_LIST_DIR}/cmake/SlimenanoSubProjectEntry.cpp.in ${PROJECT_SOURCE_DIR}/${RegistryLibrary}/src/${SLIMENANO_PROJECT_SIMPLE_NAME}.cpp)
    endif ()

    if(NOT EXISTS ${PROJECT_SOURCE_DIR}/${RegistryLibrary}/src/${SLIMENANO_PROJECT_SIMPLE_NAME}.h)
        configure_file(${CMAKE_CURRENT_FUNCTION_LIST_DIR}/cmake/SlimenanoSubProjectEntry.h.in ${PROJECT_SOURCE_DIR}/${RegistryLibrary}/src/${SLIMENANO_PROJECT_SIMPLE_NAME}.h)
    endif ()

    slimenano_scan_source_file("${PROJECT_SOURCE_DIR}/${RegistryLibrary}/src" SRC_FILES)
    string(JOIN [=[" "${SRC_DIR}/]=] SLIMENANO_PROJECT_SOURCE_FILES ${SRC_FILES})
    string(PREPEND SLIMENANO_PROJECT_SOURCE_FILES [=["${SRC_DIR}/]=])
    string(APPEND SLIMENANO_PROJECT_SOURCE_FILES [=["]=])

    slimenano_generate_includes_install_commands([=[${SRC_DIR}/]=]
            [=[include/${NAMESPACE}]=]
            "${PROJECT_SOURCE_DIR}/${RegistryLibrary}/src"
            SLIMENANO_PROJECT_INCLUDES_INSTALL)

    slimenano_get_sub_project_configuration(${RegistryLibrary} ${PROJECT_SOURCE_DIR}/${RegistryLibrary} SLIMENANO_PROJECT_SUB_CONTENT)
    get_property(SLIMENANO_PROJECT_VERSION GLOBAL PROPERTY "SLIMENANO_PROJECT_VERSION_${RegistryLibrary}")
    if ("${SLIMENANO_PROJECT_VERSION}" STREQUAL "")
        set(SLIMENANO_PROJECT_VERSION ${PROJECT_VERSION})
    endif()

    configure_file("${CMAKE_CURRENT_FUNCTION_LIST_DIR}/cmake/SlimenanoSubProject.cmake.in" "${PROJECT_SOURCE_DIR}/${RegistryLibrary}/CMakeLists.txt" @ONLY)

    get_property(SLIMENANO_VERSION_DEFINITIONS GLOBAL PROPERTY "SLIMENANO_VERSION_DEFINITIONS")

    list(APPEND SLIMENANO_VERSION_DEFINITIONS "#    define SLIMENANO_${SLIMENANO_PROJECT_LIB_NAME_UPPER}_VERSION \"${SLIMENANO_PROJECT_VERSION}\"")

    set_property(GLOBAL PROPERTY "SLIMENANO_VERSION_DEFINITIONS" "${SLIMENANO_VERSION_DEFINITIONS}")

endfunction()

function(slimenano_create_project NAMESPACE TARGET GLOBAL_INCLUDE_PATH RegistryLibrary)

    string(TOUPPER ${RegistryLibrary} LibraryOptionName)

    option(SLIMENANO_PACKAGE_INCLUDE_${LibraryOptionName} "Add Slimenano::${RegistryLibrary} library to Slimenano::Package" ON)

    message(STATUS "[SLIMENANO] ADDED OPTION: SLIMENANO_PACKAGE_INCLUDE_${LibraryOptionName}")

    if (${SLIMENANO_PACKAGE_INCLUDE_${LibraryOptionName}})

        file(RELATIVE_PATH SLIMENANO_GLOBAL_INCLUDE_PATH ${PROJECT_SOURCE_DIR}/${RegistryLibrary} ${GLOBAL_INCLUDE_PATH})

        string(PREPEND SLIMENANO_GLOBAL_INCLUDE_PATH [=[${PROJECT_SOURCE_DIR}/]=])

        slimenano_generate_subproject("${RegistryLibrary}")

        add_subdirectory(${RegistryLibrary})

        target_link_libraries(${TARGET} INTERFACE ${NAMESPACE}::${RegistryLibrary})

    endif (${SLIMENANO_PACKAGE_INCLUDE_${LibraryOptionName}})

endfunction()

function(slimenano_process_available_libraries NAMESPACE TARGET)
    set(SLIMENANO_PACKAGE_AVAILABLE_LIBRARIES "")

    get_target_property(PackageAvailableLibraries ${TARGET} INTERFACE_LINK_LIBRARIES)

    if (NOT PackageAvailableLibraries STREQUAL "PackageAvailableLibraries-NOTFOUND")

        foreach (AvailableLibrary IN LISTS PackageAvailableLibraries)

            string(REPLACE "::" "" AvailableLibraryConfigName ${AvailableLibrary})

            set(SLIMENANO_PACKAGE_IMPORT_COMMAND "")

            message(STATUS "[SLIMENANO] DETECTED ACTIVE LIBRARY IN PACKAGE: ${AvailableLibrary}")

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

    configure_file(${CMAKE_CURRENT_FUNCTION_LIST_DIR}/cmake/SlimenanoPackageConfig.cmake.in ${PROJECT_SOURCE_DIR}/cmake/${NAMESPACE}Config.cmake @ONLY)
endfunction()

function(slimenano_initialize_package_project NAMESPACE PACKAGE_LIB_NAME)

    set(SLIMENANO_PROJECT_LIB_NAME ${PACKAGE_LIB_NAME})
    set(SLIMENANO_PROJECT_NAMESPACE "${NAMESPACE}")

    if(NOT DEFINED SLIMENANO_PROJECT_INSTALL_HEADER_PREFIX)
        set(SLIMENANO_PROJECT_INSTALL_HEADER_PREFIX "include/${SLIMENANO_PROJECT_NAMESPACE}")
    endif (NOT DEFINED SLIMENANO_PROJECT_INSTALL_HEADER_PREFIX)

    set(GLOBAL_INCLUDE_PATH ${PROJECT_SOURCE_DIR}/include)

    add_library(${SLIMENANO_PROJECT_LIB_NAME} INTERFACE)
    add_library(${SLIMENANO_PROJECT_NAMESPACE}::${SLIMENANO_PROJECT_LIB_NAME} ALIAS ${SLIMENANO_PROJECT_LIB_NAME})

    message(STATUS "[SLIMENANO] ADDED Project package library: ${SLIMENANO_PROJECT_NAMESPACE}::${SLIMENANO_PROJECT_LIB_NAME}")

    slimenano_scan_sub_project(PackageFullLibraries)

    set_property(GLOBAL PROPERTY "SLIMENANO_VERSION_DEFINITIONS" "")

    foreach (RegistryLibrary ${PackageFullLibraries})

        slimenano_create_project(${SLIMENANO_PROJECT_NAMESPACE}
                ${SLIMENANO_PROJECT_LIB_NAME}
                ${GLOBAL_INCLUDE_PATH}
                ${RegistryLibrary}
        )

        string(TOUPPER ${RegistryLibrary} SLIMENANO_PROJECT_LIB_NAME_UPPER)

    endforeach (RegistryLibrary ${PackageFullLibraries})

    slimenano_process_available_libraries(
            ${SLIMENANO_PROJECT_NAMESPACE}
            ${SLIMENANO_PROJECT_LIB_NAME}
    )

    slimenano_generate_includes_install_commands(
            [=[${PROJECT_SOURCE_DIR}/include/]=]
            "${SLIMENANO_PROJECT_INSTALL_HEADER_PREFIX}"
            ${GLOBAL_INCLUDE_PATH}
            SLIMENANO_PROJECT_GLOBAL_INCLUDES_INSTALL)


    get_property(SLIMENANO_VERSION_DEFINITIONS GLOBAL PROPERTY "SLIMENANO_VERSION_DEFINITIONS")
    list(JOIN SLIMENANO_VERSION_DEFINITIONS "\n" SLIMENANO_VERSION_DEFINITIONS)

    configure_file(${CMAKE_CURRENT_FUNCTION_LIST_DIR}/cmake/SlimenanoProject.cmake.in ${PROJECT_SOURCE_DIR}/cmake/${SLIMENANO_PROJECT_NAMESPACE}Project.cmake @ONLY)
    configure_file(${CMAKE_CURRENT_FUNCTION_LIST_DIR}/cmake/slimenano-version.h.in ${GLOBAL_INCLUDE_PATH}/slimenano-version.h @ONLY)

    include(${PROJECT_SOURCE_DIR}/cmake/${SLIMENANO_PROJECT_NAMESPACE}Project.cmake)

endfunction()