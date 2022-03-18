# IbWinCppLib
A C++17 library for Windows and C++.

## How to use
### CMake project
[CMakeLists.txt](example/CMakeLists.txt):
```cmake
# FetchContent_MakeAvailable requires CMake 3.14+
cmake_minimum_required(VERSION 3.14)

project(example)

# fetch IbWinCppLib
include(FetchContent)
FetchContent_Declare(IbWinCpp
    GIT_REPOSITORY https://github.com/Chaoses-Ib/IbWinCppLib.git
    # it is recommanded to specify a version instead of using the master branch
    GIT_TAG        master
)
FetchContent_MakeAvailable(IbWinCpp)

# here is your project
add_executable(example example.cpp)
target_link_libraries(example PRIVATE IbWinCpp)
```
cmd:
```
mkdir build
cd build
cmake ..
cmake --build .
```

### Visual Studio project
#### CLI
Open VS Developer Command Prompt and run:
```cmd
git clone https://github.com/Chaoses-Ib/IbWinCppLib.git
cd IbWinCppLib
mkdir build
cd build
cmake ..
```
Then you can reference build/IbWinCpp.vcxproj in your project.

#### GUI
1. Clone or download this repository.
2. Open the directory with Visual Studio.
3. In CMake Settings/Advanced, change CMake generator to VS and save.
4. Reference out/build/IbWinCpp.vcxproj in your project.