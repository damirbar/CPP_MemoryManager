cmake_minimum_required(VERSION 3.8)
project(cpp_ex3_v3)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp src/MemPool.cpp hdr/MemPool.h src/FreeNode.cpp hdr/FreeNode.h src/FreeList.cpp hdr/FreeList.h src/MemoryManager.cpp hdr/MemoryManager.h)
add_executable(cpp_ex3_v3 ${SOURCE_FILES})