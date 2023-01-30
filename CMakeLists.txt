cmake_minimum_required (VERSION 2.9)

project (VRP-Drone)

set(CMAKE_BUILD_TYPE Debug)

set(GUROBI_ROOT "/opt/gurobi1000/linux64/")
set(GUROBI_INCLUDE_DIR "${GUROBI_ROOT}include")
message(STATUS "GUROBI_INCLUDE_DIR : ${GUROBI_INCLUDE_DIR}")

set(GUROBI_LIBRARY_CPLUS "${GUROBI_ROOT}lib/libgurobi_c++.a")
set(GUROBI_LIBRARY "${GUROBI_ROOT}lib/libgurobi100.so")
set(GUROBI_LIBRARIES ${GUROBI_LIBRARY_CPLUS} ${GUROBI_LIBRARY})
message(STATUS "GUROBI_LIBRARIES : ${GUROBI_LIBRARIES}")

include_directories(${GUROBI_INCLUDE_DIR} include)

file(GLOB_RECURSE SRCS src/*.cpp src/Graph/*.cpp src/Solution/*.cpp)

message(STATUS "SRCS : ${SRCS}")

add_executable(StructureTest.exe Mains/structureTest.cpp ${SRCS})

add_executable(main.exe Mains/main.cpp ${SRCS})

add_executable(simpleTest.exe Mains/simpleTest.cpp ${SRCS})

target_link_libraries(StructureTest.exe ${GUROBI_LIBRARIES})
target_link_libraries(simpleTest.exe ${GUROBI_LIBRARIES})
target_link_libraries(main.exe ${GUROBI_LIBRARIES})