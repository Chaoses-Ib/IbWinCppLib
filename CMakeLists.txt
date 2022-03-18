cmake_minimum_required(VERSION 3.12)

project(IbWinCpp)

# Header files
set(cppHeaderFiles
    auto_cast.hpp
    basic_type.hpp
    common.hpp
    constexpr.hpp
    holder.hpp
    iterator.hpp
    macro_variable.hpp
    zstring.hpp
    )
list(TRANSFORM cppHeaderFiles PREPEND include/IbWinCpp/Cpp/)
source_group("Header Files/Cpp" FILES ${cppHeaderFiles})

set(winHeaderFiles
    Common.hpp
    Diagnostic.hpp impl/Diagnostic.hpp
    FileSystem.hpp
    Gui.hpp
    Memory.hpp
    Process.hpp
    WinCpp.hpp
    )
list(TRANSFORM winHeaderFiles PREPEND include/IbWinCpp/)
source_group("Header Files" FILES ${winHeaderFiles})

# Source files
set(sourceFiles
    FileSystem.cpp
    Gui.cpp
    Memory.cpp
    Process.cpp
)
list(TRANSFORM sourceFiles PREPEND source/)

add_library(IbWinCpp ${cppHeaderFiles} ${winHeaderFiles} ${sourceFiles})
target_include_directories(IbWinCpp PUBLIC include)
target_compile_features(IbWinCpp PUBLIC cxx_std_17)

# Tests
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    option(BUILD_TESTING "Build the testing tree." OFF)
    include(CTest)
    if(BUILD_TESTING)
        find_package(Catch2 REQUIRED)
        
        set(cppTestFiles
            auto_cast.cpp
            holder.cpp
            zstring.cpp
        )
        list(TRANSFORM cppTestFiles PREPEND test/Cpp/)

        set(winTestFiles
            main.cpp
            Diagnostic.cpp
            FileSystem.cpp
            Memory.cpp
        )
        list(TRANSFORM winTestFiles PREPEND test/)

        add_executable(ibwincpp_test ${cppTestFiles} ${winTestFiles})
        target_link_libraries(ibwincpp_test PRIVATE IbWinCpp Catch2::Catch2)
    endif()
endif()