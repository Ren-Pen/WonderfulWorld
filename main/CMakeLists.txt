cmake_minimum_required(VERSION 3.5.0)
project(MainProject)

set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)

set(SRC_FILES
    ${SRC_DIR}/main.cpp
)

set(EXE_NAME Main)

add_executable(${EXE_NAME} ${SRC_FILES})

find_package(Slimenano CONFIG)

target_link_libraries(${EXE_NAME} PRIVATE Slimenano::Math)
target_link_libraries(${EXE_NAME} PRIVATE Slimenano::UI)
target_link_libraries(${EXE_NAME} PRIVATE Slimenano::Event)

install(TARGETS ${EXE_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)