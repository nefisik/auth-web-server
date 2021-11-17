cmake_minimum_required(VERSION 3.5)

project(Server C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Poco REQUIRED COMPONENTS Foundation Net NetSSL Data Util MongoDB Redis)
find_package(OpenSSL REQUIRED COMPONENTS SSL Crypto)
find_package(cpp-jwt REQUIRED)
find_package(Boost COMPONENTS system filesystem REQUIRED)

file(GLOB HEADERS_Server
	database/algorithms/include/*.hpp
	database/mongodb/include/*.hpp
	database/redis/include/*.hpp
    database/mail/include/*.hpp
	handlers/include/*.hpp
	methods/include/*.hpp
	server/include/*.hpp
	server/include/Base/*.hpp
	server/include/Base/static/*.hpp
    server/include/Base/libs/*.hpp
	)

file(GLOB SOURCE_Server
	database/algorithms/source/*.cpp
	database/mongodb/source/*.cpp
	database/redis/source/*.cpp
    database/mail/source/*.cpp
	handlers/source/*.cpp
	handlers/source/Base/*.cpp
	methods/source/*.cpp
	server/source/*.cpp
	server/source/Base/*.cpp
	)

add_library(${PROJECT_NAME} SHARED ${HEADERS_Server} ${SOURCE_Server})

target_link_libraries(${PROJECT_NAME} PUBLIC ${Poco_LIBRARIES} ${OPENSSL_LIBRARIES} ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} cpp-jwt::cpp-jwt)