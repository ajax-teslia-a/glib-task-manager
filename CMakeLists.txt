cmake_minimum_required(VERSION 3.16)

project(glib_task_manager VERSION 0.1.0 LANGUAGES C)

set(TARGET glib_task_manager)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # for clangd

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()
message(STATUS "CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Flags
add_compile_options(-Wall -Wextra -Waddress -Warray-bounds=1 -Wlogical-not-parentheses)
add_compile_options(-Werror=unused-result)
add_compile_options(-Wno-implicit-fallthrough -Wno-unused-parameter -Wno-unused-function)
add_compile_definitions(_REENTRANT)

# Dependencies
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIB REQUIRED IMPORTED_TARGET glib-2.0 gio-2.0)

# Targets
add_library(task_manager STATIC src/cli.c
                                src/manager.c
                                src/tag.c
                                src/task.c
                                src/task_db.c)
target_include_directories(task_manager PUBLIC include)
target_link_libraries(task_manager PUBLIC PkgConfig::GLIB)

add_executable(${TARGET} src/main.c)
target_link_libraries(${TARGET} PRIVATE task_manager)

# Tests
option(BUILD_TESTS "Build CMocka tests" ON)
if(BUILD_TESTS)
    include(cmake/cmocka.cmake)
    if(TARGET cmocka)
        enable_testing()
        add_subdirectory(tests)
    else()
        message(WARNING "cmocka not found, tests are skipped (sudo apt install libcmocka-dev)")
    endif()
endif()
