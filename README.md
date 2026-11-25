# glib-task-manager

GLib-only based task manager with priorities, queues, indexes and tags

## Prerequisites

```bash
sudo apt install cmake pkg-config libglib2.0-dev valgrind libcmocka-dev
```

## Build

```bash
cmake --preset debug          # Debug, writes build/debug/compile_commands.json (used by clangd)
cmake --build --preset debug -- -j$(nproc)
./build/debug/out/bin/glib_task_manager
```

## Valgrind

```bash
G_SLICE=always-malloc G_DEBUG=gc-friendly valgrind --leak-check=full ./build/debug/out/bin/glib_task_manager
```

Or run the `valgrind` task in Zed

## Tests

CMocka tests live in `tests/<name>_test/` (one dir per test binary, registered in `tests/CMakeLists.txt`).

```bash
cmake --build --preset debug && ctest --test-dir build/debug --output-on-failure
```

Binaries land in `build/debug/bin/` and run from there. Mocking uses the GNU linker `--wrap`.
Without `libcmocka-dev` the tests are skipped with a warning (or pass `-DBUILD_TESTS=OFF`).

## Code style

Same as `mcu_communicator` (`.clang-format`): 3 spaces, 140 columns, Mozilla braces, left pointer alignment.
Module-prefixed functions (`TM_Push`), `_t` types, `-Wall -Wextra` flags.
Editor setup is in `.zed/` (clangd + format on save) and `.clangd` (clang-tidy checks).
