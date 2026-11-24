# Provides the `cmocka` target from the system package (sudo apt install libcmocka-dev), if present.
find_package(PkgConfig REQUIRED)
pkg_check_modules(CMOCKA IMPORTED_TARGET cmocka)
if(CMOCKA_FOUND)
    add_library(cmocka ALIAS PkgConfig::CMOCKA)
endif()
