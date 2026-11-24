# Shared test helpers (tests/common)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/common)

add_subdirectory(task_manager_test)

# All test binaries land in CMAKE_RUNTIME_OUTPUT_DIRECTORY (out/bin/) and are executed from there,
# so resource files placed next to them are resolvable via relative paths.
set(TEST_WORKDIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

add_test(NAME task_manager_test COMMAND task_manager_test WORKING_DIRECTORY ${TEST_WORKDIR})

# Treat any CMocka "[  FAILED  ]" line as a failure even if the binary exits 0.
set(CMOCKA_TESTS
    task_manager_test
)
set_tests_properties(${CMOCKA_TESTS} PROPERTIES
    FAIL_REGULAR_EXPRESSION "\\[  FAILED  \\]"
    TIMEOUT 30
)
