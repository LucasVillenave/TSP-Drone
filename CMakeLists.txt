cmake_minimum_required (VERSION 2.9)

project (VRP-Drone)

set(CMAKE_BUILD_TYPE Debug)

include_directories(include)

file(GLOB_RECURSE SRCS src/*.cpp src/Graph/*.cpp)

message(STATUS "SRCS : ${SRCS}")

add_executable(StructureTest.exe Mains/StructureTest.cpp ${SRCS})

add_executable(main.exe Mains/main.cpp ${SRCS})
