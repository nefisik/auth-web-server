cmake_minimum_required(VERSION 3.5)

project(database C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Poco REQUIRED COMPONENTS Foundation Net Data Util MongoDB Redis)
find_package(OpenSSL REQUIRED COMPONENTS SSL Crypto)
find_package(cpp-jwt REQUIRED)

file(GLOB HEADERS_DataBase
        include/algorithms/*.h
        include/mongodb/*.h
        include/redis/*.h
        )

file(GLOB SOURCE_database
        source/algorithms/*.cpp
        source/mongodb/*.cpp
        source/redis/*.cpp
        )


add_library(${PROJECT_NAME} SHARED ${HEADERS_DataBase} ${SOURCE_DataBase})

target_compile_definitions(${PROJECT_NAME} PUBLIC ${Poco_DEFINITIONS} ${JWT_DEFINITIONS})

target_link_directories(${PROJECT_NAME} PUBLIC ${Poco_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR})

target_include_directories(${PROJECT_NAME} PUBLIC ${Poco_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} PUBLIC ${Poco_LIBRARIES} ${OPENSSL_LIBRARIES} cpp-jwt::cpp-jwt)

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

set(${PROJECT_NAME}_DEFINITIONS
        CACHE INTERNAL "${PROJECT_NAME}: Definitions" FORCE)

set(${PROJECT_NAME}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include
        CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)
