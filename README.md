
---

#  Nova Engine (Legacy)

![C++](https://img.shields.io/badge/language-C%2B%2B-blue.svg) ![OpenGL](https://img.shields.io/badge/API-OpenGL-orange.svg) ![Status](https://img.shields.io/badge/status-archived-red.svg)

**Nova Engine** was my deep dive into low-level game engine development using C++ and OpenGL. It represents a significant period of my learning journey—a project born from ambition, shaped by countless bugs, and fueled by the desire to understand how game engines work under the hood.

> [IMPORTANT]  
> **This project is now archived.** Development has ceased in favor of a successor built with modern architectural approaches and technologies (**Rust + Vulkan**). The source code is released publicly for educational purposes and as a personal milestone.

---

## 🛠 Tech Stack
- **Language:** C++ (MSVC)
- **Graphics API:** OpenGL (via Glad)
- **Windowing & Input:** GLFW3
- **Model Loading:** Assimp
- **Build System:** CMake + vcpkg

---

## 🎮 Controls & Navigation

Nova Engine features a familiar, editor-like control scheme for interacting with the 3D scene:

*   **Camera Movement:** Hold **LMB (Left Mouse Button)** and use **WASD** keys to fly through the 3D space.
*   **Object Selection:** 
    *   Click **LMB** on an object to select it.
    *   Hold **Shift + LMB** to select multiple objects simultaneously.
*   **Transformations (Gizmo):** When a single object is selected, a transformation Gizmo will appear. Use the following hotkeys to switch modes:
    *   **[1]**: Translation (Move)
    *   **[2]**: Rotation
    *   **[3]**: Scaling

---


## 📥 Getting Started

### Method 1: Prebuilt Binary (For Users)
1. Go to the [Releases](https://github.com/Mike-Flint/Nova-Engine/releases) page.
2. Download the latest `.zip` archive.
3. Extract the files and run `Nova-Engine.exe`.

### Method 2: Build from Source (For Developers)

#### Requirements
- **Windows 10/11**
- **MSVC Build Tools**
- **CMake** (added to PATH)
- **Git**
- **vcpkg** installed and integrated

---

## 🚀 Build Instructions

### 1) Clone
```bash
# Clone the repository
git clone https://github.com/Mike-Flint/Nova-Engine.git
cd Nova-Engine

```

### 2) Configure CMake & Dependencies
⚠️ **Note:** Replace the toolchain path below with the actual path to your `vcpkg.cmake` file.

```powershell
# Install dependencies via vcpkg
vcpkg install glfw3 glad assimp --triplet x64-windows

cmake -S . -B build `
  -DCMAKE_TOOLCHAIN_FILE="C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake" `
  -DVCPKG_TARGET_TRIPLET=x64-windows
```

### 3) Build and Run
```powershell
# Compile the project
cmake --build build --config Release

# Deploy Assets to the output directory
xcopy /E /I /Y Assets build\Release\Assets

# Run the engine
./build/Release/Nova-Engine.exe
```

---
