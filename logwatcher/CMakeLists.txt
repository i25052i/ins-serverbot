cmake_minimum_required(VERSION 3.0.0)
project(logwatcher VERSION 0.1.0)

include(CTest)
enable_testing()

configure_file(logwatcher.h.in logwatcher.h)

add_subdirectory(rcon)
add_subdirectory(fsm)

add_executable(logwatcher main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

add_definitions(-D_WIN32_WINNT=0x0A00)

target_link_libraries(logwatcher PUBLIC rcon fsm)

target_include_directories(logwatcher PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           "${PROJECT_SOURCE_DIR}/rcon"
                           )