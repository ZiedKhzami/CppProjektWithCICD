📘 FirstProject v1.0.0 – Modern C++20 Project with Tests, Benchmarks and Tooling
Ein vollständig modernes, professionelles C++20 Projekt‑Template mit:
-   CMake (modern, target-based)
-   Ninja + MSVC
-   clang‑tidy (Target-based, MSVC‑kompatibel)
-   clang-format
-   GoogleTest
-   Google Benchmark
-   Doxygen
-   VS Code Integration (clangd + CMake Tools)
-   Sanitizer Support (Clang/GCC)
-   vcpkg / Conan kompatibel
-   Saubere Projektstruktur + Best Practices

Dieses Template eignet sich perfekt für neue C++ Projekte — privat, beruflich oder für Lernzwecke.

📂 Projektstruktur
FirstProject/
│
├── CMakeLists.txt
├── CMakePresets.json
├── .clang-tidy
├── .clang-format
├── .gitignore
├── README.md
│
├── app/
│   └── main.cpp
│
├── include/
│   └── project/
│       └── math.hpp
│
├── src/
│   └── math.cpp
│
├── tests/
│   ├── test_main.cpp
│   └── test_math.cpp
│
├── benchmarks/
│   └── bench_math.cpp
│
└── docs/
    └── Doxyfile.in


🚀 Voraussetzungen
Windows

-   Visual Studio 2022 Build Tools
-   LLVM + clangd + clang-tidy
    https://github.com/llvm/llvm-project/releases
-   Ninja Build System
    https://github.com/ninja-build/ninja/releases
-   CMake ≥ 3.20
    https://cmake.org/download/

VS Code Extensions
-   ms-vscode.cmake-tools
-   llvm-vs-code-extensions.vscode-clangd
-   ms-vscode.cpptools
-   optional: twxs.cmake


WICHTIG:
VS Code immer über Developer PowerShell for VS 2022 starten:
    code .

🔧 Build-System: CMake Presets
Dieses Projekt nutzt CMake Presets für eine einheitliche Build‑Konfiguration.
Debug Build
    cmake --preset ninja-msvc-debug
    cmake --build --preset build-debug

Release Build
    cmake --preset ninja-msvc-release
    cmake --build --preset build-release
Alle Binaries landen unter:
    build/<config>/bin/


🧪 Tests ausführen (GoogleTest)
Tests kompilieren:
    cmake --build --preset build-debug
Tests ausführen:
    ctest --preset test-debug --output-on-failure
Oder direkt:
    build/debug/bin/tests/unit_tests.exe

📊 Benchmarks (Google Benchmark)
Release-Benchmarks bauen:
    cmake --build --preset build-release --target benchmarks
Ausführen:
    build/release/bin/bench/benchmarks.exe


📝 Dokumentation (Doxygen)
Doxygen HTML-Dokumentation generieren:
    cmake --build build/debug --target docsWeitere Zeilen anzeigen
HTML-Startseite:
    build/debug/docs/html/index.html


🧹 Code-Qualität
clang-format (automatisch)
Code formatieren:
    cmake --build build/debug --target formatWeitere Zeilen anzeigen
Check ohne Änderungen (für CI):
    cmake --build build/debug --target format-checkWeitere Zeilen anzeigen
VS Code formatiert automatisch bei Speichern.

clang‑tidy (target-based, MSVC-kompatibel)
clang‑tidy läuft nur auf:
    - core_lib
    - app

Deine .clang-tidy aktiviert:
    - modernize‑*
    - bugprone‑*
    - readability‑*
    - cppcoreguidelines‑*
    - hicpp‑*
    - performance‑*
    - misc‑*

Vendor-Code (GoogleTest, Benchmark) wird korrekt ignoriert.

🧰 Sanitizer Support
Für Clang/GCC aktivieren:
    cmake --preset ninja-clang-debug -DENABLE_SANITIZERS=ON
    cmake --build --preset build-clang-debugWeitere Zeilen anzeigen

Enthält:
    - AddressSanitizer (ASan)
    - UndefinedBehaviorSanitizer (UBSan)


🔌 VS Code Integration
Dieses Projekt enthält eine vollständige VS Code Konfiguration:

    .vscode/settings.json
        → clangd aktiviert
        → compile_commands.json verlinkt
        → Format-on-save
    .vscode/launch.json
        → Debuggen von app, unit_tests, benchmarks
    .vscode/tasks.json
        → Build, Test, Format, Docs, Benchmarks als Tasks


📦 Paketmanager (optional)
vcpkg (empfohlen)
CMakePresets.json vorbereiten:
    "CMAKE_TOOLCHAIN_FILE": "C:/vcpkg/scripts/buildsystems/vcpkg.cmake",
    "VCPKG_TARGET_TRIPLET": "x64-windows"Weitere Zeilen anzeigen
Dann Packages nutzen:
    find_package(fmt CONFIG REQUIRED)
    target_link_libraries(core_lib PUBLIC fmt::fmt)Weitere Zeilen anzeigen

Conan (Alternative)
INI[requires]fmt/10.2.1[generators]CMakeDepsCMakeToolchain[layout]cmake_layoutWeitere Zeilen anzeigen

🏗️ Projekt erweitern
Dieses Template lässt sich einfach erweitern:

    - Weitere Libraries hinzufügen (add_library(...))
    - Weitere Apps hinzufügen (add_executable(...))
    - Weitere Tests (tests/*.cpp)
    - Weitere Benchmarks (benchmarks/*.cpp)
    - API erweitern (include/project/*.hpp)


🤝 Mitwirken / Forken
Dieses Template ist ideal für:
    - Lernprojekte
    - Firmeninterne Starter-Kits
    - Clean-Code Workshops
    - CMake/VSCode/clang‑tidy Trainings
    - Universitäre Projekte
    - Fortgeschrittene C++20/23 Projekte
