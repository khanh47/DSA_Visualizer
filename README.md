# DSA Visualizer

An interactive visualizer for Data Structures built with C++ and SFML.  
Supports step-by-step animation for **Linked List**, **Hash Table**, **Kruskal's MST**, and **Trie**.

---

## Prerequisites

Before building, make sure you have the following installed:

| Tool | Version | Notes |
|---|---|---|
| [MSYS2](https://www.msys2.org/) | latest | Provides the MinGW-w64 compiler on Windows |
| GCC (MinGW-w64) | 13+ | Install via MSYS2 (see below) |
| CMake | 3.16+ | Install via MSYS2 (see below) |
| [SFML](https://www.sfml-dev.org/download.php) | 3.x | Download the GCC MinGW build |

### Install GCC and CMake via MSYS2

Open the MSYS2 terminal and run:

```bash
pacman -S --needed base-devel mingw-w64-ucrt-x86_64-toolchain
pacman -S mingw-w64-ucrt-x86_64-cmake
```

Then add `C:\msys64\ucrt64\bin` to your system `PATH`.

### Install SFML

1. Download the **GCC MinGW** build of SFML 3.x from [sfml-dev.org](https://www.sfml-dev.org/download.php)
2. Extract it to `C:\SFML`

---

## Building from Source

> **Note:** `CMakeLists.txt`, `build/`, `.vscode/`, `.exe`, and `.dll` files are excluded from the repository via `.gitignore`. You need to create the `CMakeLists.txt` yourself (provided below) and run the build commands each time you clone the project.

### Step 1 — Create CMakeLists.txt

Create a file named `CMakeLists.txt` in the **project root** (same level as `src/` and `include/`) with the following content:

```cmake
cmake_minimum_required(VERSION 3.16)
project(DSA_Visualizer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(DSA_Visualizer ${SOURCES})

target_include_directories(DSA_Visualizer PRIVATE
    include
    include/Core
    include/Command
    include/DataStructures
    include/Scene
    include/UI
    include/Visualizers
    C:/SFML/include
)

target_link_directories(DSA_Visualizer PRIVATE C:/SFML/lib)
target_link_libraries(DSA_Visualizer PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
)
```

> If you installed SFML to a different path, update `C:/SFML` accordingly.

### Step 2 — Configure and Build

Open a terminal (MSYS2 MINGW64 or any terminal with MinGW on `PATH`) in the project root and run:

```bash
rm -rf build/
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build
```

### Step 3 — Copy SFML DLLs

Copy all `.dll` files from `C:\SFML\bin` into the `build\` folder:

```bash
cp /c/SFML/bin/*.dll build/
```

Also copy the MinGW runtime DLLs from your MSYS2 installation:

```bash
cp /c/msys64/ucrt64/bin/libgcc_s_seh-1.dll build/
cp /c/msys64/ucrt64/bin/libstdc++-6.dll build/
cp /c/msys64/ucrt64/bin/libwinpthread-1.dll build/
```

### Step 4 — Run

```bash
./build/DSA_Visualizer.exe
```

> Always run the executable from the **project root**, not from inside `build/`. The program loads fonts and assets from the `assets/` folder relative to the working directory.

---

## Project Structure

```
DSA_Visualizer/
├── assets/             # Fonts and icons
├── include/            # Header files
│   ├── Command/
│   ├── Core/
│   ├── DataStructures/
│   ├── Scene/
│   ├── UI/
│   └── Visualizers/
├── src/                # Source files
│   ├── Command/
│   ├── Core/
│   ├── DataStructures/
│   ├── Scene/
│   ├── UI/
│   └── Visualizers/
├── .gitignore
└── README.md
```

---

## Features

- **Linked List** — Insert (head / tail / index), delete, search, update with step-by-step animation
- **Hash Table** — Chaining-based hash table with insert, delete, search
- **Kruskal's MST** — Visualizes the Minimum Spanning Tree algorithm edge by edge
- **Trie** — Insert, search, and delete words with animated node traversal and pseudocode highlighting

All visualizers include:
- Pseudocode panel with line-by-line highlighting
- Playback controls (step forward/back, auto-play, speed adjustment)
- Zoom and pan support

---

## References

See the `References` folder for academic and implementation references used in this project.
