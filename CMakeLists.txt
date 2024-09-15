cmake_minimum_required(VERSION 3.10)

# Project name and version
project(Library VERSION 1.0)

# Include header files from inc folder
include_directories(${CMAKE_SOURCE_DIR}/inc)

# Add the executable by specifying the source files in the src folder
add_executable(Multithreading src/main.cpp)