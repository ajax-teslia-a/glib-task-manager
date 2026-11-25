set(SOURCES
    task_manager_test.c
    # ${PROJECT_SOURCE_DIR}/src/task_manager.c
)

add_executable(task_manager_test ${SOURCES})

target_include_directories(task_manager_test PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_compile_options(task_manager_test PRIVATE -g -O0)
# Mocking uses the GNU linker --wrap, e.g.:
# target_link_options(task_manager_test PRIVATE -Wl,--wrap=g_queue_new)
target_link_libraries(task_manager_test PRIVATE PkgConfig::GLIB cmocka)
