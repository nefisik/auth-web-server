cmake_minimum_required(VERSION 3.5)

project(Server C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Poco REQUIRED COMPONENTS Foundation Net Data Util MongoDB Redis)
find_package(OpenSSL REQUIRED COMPONENTS SSL Crypto)
find_package(cpp-jwt REQUIRED)
find_package(Boost COMPONENTS system filesystem REQUIRED)

file(GLOB HEADERS_Server
	database/algorithms/include/*.hpp
	database/mongodb/include/*.hpp
	database/redis/include/*.hpp
	handlers/include/*.hpp
	handlers/include/Base/*.hpp
	server-run/include/*.hpp
	)

file(GLOB SOURCE_Server
	database/algorithms/source/*.cpp
	database/mongodb/source/*.cpp
	database/redis/source/*.cpp
	handlers/source/*.cpp
	handlers/source/Base/*.cpp
	server-run/source/*.cpp
	)

add_library(${PROJECT_NAME} SHARED ${HEADERS_Server} ${SOURCE_Server})

target_compile_definitions(${PROJECT_NAME} PUBLIC ${Poco_DEFINITIONS} ${JWT_DEFINITIONS})

target_link_directories(${PROJECT_NAME} PUBLIC ${Poco_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR})

target_include_directories(${PROJECT_NAME} PUBLIC ${Poco_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} PUBLIC ${Poco_LIBRARIES} ${OPENSSL_LIBRARIES} ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} cpp-jwt::cpp-jwt)

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

set(${PROJECT_NAME}_DEFINITIONS
	CACHE INTERNAL "${PROJECT_NAME}: Definitions" FORCE)

set(${PROJECT_NAME}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include
	CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)