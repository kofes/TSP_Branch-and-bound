cmake_minimum_required(VERSION 3.5)
project(TSP)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Werror=return-type -ansi -pedantic -Werror=pedantic")

set(SOURCE_FILES main.cpp)
add_executable(TSP ${SOURCE_FILES})
target_link_libraries(TSP ${CMAKE_SOURCE_DIR}/qsopt.a)
