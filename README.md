# The Kyle Huang Engine
This is a 2d physics engine built fully in C++ and OpenGL. It is cross platform (CMake), and extremely simple to build, as we're using the Conan package manager. This engine uses the Fourth Order Runge Kutta method for simulating n-body gravity. A body can be spawned by clicking in the window. Collisions are supported by the engine.
# Libraries used
- GLFW (For window and input handling)
- GLAD (For loading OpenGL Functions)
- FMT (For console outputs)
# Build Requirements
To build this project, you'll need a few prerequisites on your system.
- A working C++ Compiler
- CMake
- Conan Package Manager
## Windows
Windows doesn't ship with a C++ Compiler, so you can install one with MSYS2. Click [here](https://code.visualstudio.com/docs/cpp/config-mingw#_prerequisites) to learn how to do that. For CMake, download a [binary distribution](https://cmake.org/download/) and check "Add to Path" during installation. You can check if it was installed properly by rebooting your system, opening a terminal, and typing
```powershell
cmake --version
```
For the Conan package manager, you can use PIP, the Python package manager to install it with
```powershell
pip install conan
```
## MacOS
MacOS ships with Clang, but you can't use it until you install the Xcode Command Line tools with
```zsh
xcode-select --install
```
You can install CMake and Conan with the Homebrew package manager easily by using the commands
```zsh
brew cmake conan
```
## Build Instructions
First, clone the repository and make it your working directory.
```bash
git clone https://github.com/thekylehuang/gravity-engine.git
cd gravity-engine
```
If you haven't used Conan before, you'll need it to detect your system profile with
```bash
conan profile detect --force
```
This saves your system info to Conan. To install all required libraries (GLFW, GLAD, FMT), run
```bash
conan install . --output-folder=build --build=missing
```
This has Conan install all the libraries this engine uses into the build folder. It also makes a CMake preset called "conan-release". To set that preset, run the following commands:
```bash
cd build
cmake --preset conan-release
cmake --build --preset conan-release
```
Wait a moment, and it will generate a file called "gravity_engine.exe". You can now run it with
```bash
./gravity_engine
```
