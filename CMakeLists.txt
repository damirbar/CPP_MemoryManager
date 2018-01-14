#  Cmake minimum version required.
cmake_minimum_required(VERSION 3.8)

#  Project's name.
project(cpp_ex3_v3)

#  C++ Version.
set(CMAKE_CXX_STANDARD 11)

#  Include the following directory (or directories, separated by space).
include_directories(/hdr)

#  Includue all .cpp files from src directory.
file(GLOB SRC_FILES src/*.cpp main.cpp)

#  Generate executable.
add_executable(cpp_ex3_v3 ${SRC_FILES})
