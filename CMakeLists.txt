cmake_minimum_required(VERSION 3.0.0)
project(CRC16 VERSION 0.1.0)

include(CTest)
enable_testing()

SET(CMAKE_CXX_FLAGS "-g")
SET(CMAKE_CXX_FLAGS_DEBUG "-O0")
SET(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}CRC/INC)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}SERVER/INC)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}CLIENT/INC)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/OUTPUT/bin)

add_subdirectory(CRC)
add_subdirectory(SERVER)
add_subdirectory(CLIENT)

