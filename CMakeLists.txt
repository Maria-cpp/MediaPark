cmake_minimum_required(VERSION 3.17)
project(Smart_Pointers)

set(CMAKE_CXX_STANDARD 17)
find_package(Threads)
add_executable(Smart_Pointers main.cpp unique_ptr.cpp unique_ptr.h shared_ptr.cpp shared_ptr.h)
target_link_libraries(Smart_Pointers LINK_PUBLIC Threads::Threads)