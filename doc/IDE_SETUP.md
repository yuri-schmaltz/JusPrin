# IDE Setup Guide

This guide describes how to set up your development environment for JusPrin using Visual Studio Code or CLion.

## Visual Studio Code (Recommended)

We provide a comprehensive `.vscode` configuration that works out-of-the-box.

### Prerequisites

1.  Install [Visual Studio Code](https://code.visualstudio.com/).
2.  Install the **C/C++ Extension Pack** (ms-vscode.cpptools-extension-pack).
3.  Install **CMake Tools** (ms-vscode.cmake-tools).

### Setup

1.  Open the `JusPrin` folder in VS Code.
2.  When prompted, select a **Kit** matching your compiler (e.g., GCC 9+ or Clang 10+).
3.  VS Code will automatically configure the project using CMake.

### Workflow

-   **Build**: Press `F7` or click "Build" in the status bar.
-   **Debug**: Press `F5` to start debugging with the pre-configured launch profile.
-   **Test**: Run tests via the Testing sidebar or `Ctrl+Shift+P` -> `CMake: Run Tests`.

### Useful Commands

-   `Ctrl+Shift+B`: Run build task.
-   `Ctrl+Shift+P` -> `CMake: Configure`: Re-run CMake configuration.

---

## CLion

CLion is a powerful IDE by JetBrains with excellent C++ support.

### Setup

1.  Open CLion.
2.  Select **Open** and choose the `JusPrin` folder (containing `CMakeLists.txt`).
3.  CLion will detect the CMake project and load it.

### Configuration

We have provided shared run configurations in `.idea/runConfigurations`.

-   **JusPrin (Release)**: Builds and runs the application in Release mode.
-   **Debug**: You can create a Debug profile by going to `File -> Settings -> Build, Execution, Deployment -> CMake` and adding a new profile with "Debug" build type.

### Debugging

1.  Select the **JusPrin** configuration from the top-right dropdown.
2.  Click the **Debug** icon (bug).
3.  Breakpoints works as expected.

---

## Common Issues

### CMake configuration fails
-   Ensure you have all dependencies installed (see `scripts/Dockerfile` for the list).
-   If using Docker, start VS Code inside the container using the "Remote - Containers" extension.

### IntelliSense not working
-   Run `CMake: Configure` to generate the compilation database.
-   Ensure `.vscode/c_cpp_properties.json` matches your system paths (Linux/Mac/Windows).
