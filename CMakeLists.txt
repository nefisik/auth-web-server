cmake_minimum_required(VERSION 3.5)

project(Web-Server C CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -pedantic -lpthread")

add_subdirectory(modules)
add_subdirectory(Executable)
