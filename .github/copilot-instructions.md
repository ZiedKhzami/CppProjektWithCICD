# AI Coding Guidelines for FirstProject

## Project Architecture
- **Core Library**: `core_lib` in `src/` implements business logic, headers in `include/project/`
- **Applications**: Executables in `app/`, link against `core_lib`
- **Namespace**: All code in `project` namespace
- **Structure**: Headers in `include/project/`, implementations in `src/`, tests in `tests/`, benchmarks in `bench/`

## Build System
- Use CMake presets: `linux-gcc-debug`, `linux-clang-debug`, `linux-gcc-release`, `linux-clang-release`
- Configure: `cmake --preset linux-gcc-debug` (copies `compile_commands.json` to root)
- Build: `cmake --build --preset gcc-debug`
- Test: `ctest --test-dir build/linux-gcc-debug --output-on-failure`
- Format: `cmake --build build/linux-gcc-debug --target format`
- Docs: `cmake --build build/linux-gcc-debug --target docs`

## Code Conventions
- **C++20** with trailing return types: `auto add(int a, int b) -> int`
- **clang-tidy** enabled on `core_lib` and `app` targets with modernize/*, bugprone/*, readability/* checks
- **clang-format** auto-formats on save in VS Code
- **GoogleTest** for unit tests: `TEST(Math, Add) { EXPECT_EQ(project::add(2, 3), 5); }`
- **Google Benchmark** for performance tests: `BENCHMARK(bm_add);` with NOLINTBEGIN/END

## Development Workflow
- **VS Code Integration**: clangd for IntelliSense, CMake Tools extension for builds
- **CI/CD**: GitHub Actions (Windows MSVC), Jenkins (Linux matrix: GCC/Clang × Debug/Release)
- **Sanitizers**: Enabled by default on Clang debug builds (`-DENABLE_SANITIZERS=ON`)
- **Package Managers**: vcpkg/Conan compatible, add dependencies via `find_package()` and `target_link_libraries()`
- **Running Applications**: After building, executables are in `build/<preset>/` (e.g., `./build/linux-gcc-debug/app`)

## Adding New Code
- **Library functions**: Add header to `include/project/`, implementation to `src/`, update `CMakeLists.txt` (add to `add_library(core_lib ...)`), add test to `tests/test_*.cpp`, add benchmark to `bench/bench_*.cpp` with NOLINT
- **Tests**: Add `tests/test_*.cpp` with GoogleTest fixtures
- **Benchmarks**: Add `bench/bench_*.cpp` with Google Benchmark, NOLINT for clang-tidy
- **Executables**: Add to `app/` directory, create new target in `CMakeLists.txt`

## Key Files
- `CMakeLists.txt`: Target definitions, clang-tidy/format setup
- `CMakePresets.json`: Platform/compiler configurations
- `include/project/math.hpp`: Example header with namespace
- `src/math.cpp`: Implementation example
- `tests/test_math.cpp`: Unit test pattern
- `bench/bench_math.cpp`: Benchmark pattern with NOLINT</content>
<parameter name="filePath">/home/zied/Workspace/CPP/CppProjektWithCICD/.github/copilot-instructions.md