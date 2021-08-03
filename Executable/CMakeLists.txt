cmake_minimum_required(VERSION 3.5)

project(Executable C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(${PROJECT_NAME} main.cpp)

target_compile_definitions(${PROJECT_NAME} PUBLIC ${SERVER_DEFINITIONS})

target_link_directories(${PROJECT_NAME} PUBLIC ${SERVER_INCLUDE_DIRS})

target_include_directories(${PROJECT_NAME} PUBLIC ${SERVER_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} PUBLIC Server)