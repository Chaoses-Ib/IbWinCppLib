cmake_minimum_required(VERSION 3.2)

project(IbWinCpp)

add_library(IbWinCpp source/src.cpp)
target_include_directories(IbWinCpp PUBLIC include)
target_compile_features(IbWinCpp PUBLIC cxx_std_17)