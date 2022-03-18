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