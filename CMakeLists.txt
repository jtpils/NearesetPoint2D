cmake_minimum_required(VERSION 3.15)
project(NearestPoint2D)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(NearestPoint2D main.cpp)
target_link_libraries(NearestPoint2D ${OpenCV_LIBS})

add_executable(BFS-DFS BFS-DFS.cpp)
target_link_libraries(BFS-DFS ${OpenCV_LIBS})