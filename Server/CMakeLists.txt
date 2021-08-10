cmake_minimum_required(VERSION 3.5)

project(Server C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Poco COMPONENTS Net Util Data MongoDB REQUIRED)
find_package(Boost)
find_package(OpenSSL REQUIRED COMPONENTS SSL Crypto)
# find_package(PostgreSQL REQUIRED)

file(GLOB HEADERS_Server
		algorithms/include/*.h
		factory/include/*.h
		handlers/include/*.h
		database/mongodb/include/*.h
		# database/postgres/include/*.h
		server-run/include/*.h
	)

file(GLOB SOURCE_Server
		algorithms/source/*.cpp
		factory/source/*.cpp
		handlers/source/*.cpp
		database/mongodb/source/*.cpp
		# database/postgres/source/*.cpp
		server-run/source/*.cpp
	)

add_library(${PROJECT_NAME} SHARED ${HEADERS_Server} ${SOURCE_Server})

target_compile_definitions(${PROJECT_NAME} PUBLIC ${Poco_DEFINITIONS})

target_link_directories(${PROJECT_NAME} PUBLIC ${Poco_INCLUDE_DIRS} ${OpenSSL_INCLUDE_DIRS})

target_include_directories(${PROJECT_NAME} PUBLIC ${Poco_INCLUDE_DIRS} ${OpenSSL_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} PUBLIC ${Poco_LIBRARIES} ${OpenSSL_LIBRARIES})

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

set(${PROJECT_NAME}_DEFINITIONS
		CACHE INTERNAL "${PROJECT_NAME}: Definitions" FORCE)

set(${PROJECT_NAME}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include
		CACHE INTERNAL "${PROJECT_NAME}: Definitions" FORCE)
