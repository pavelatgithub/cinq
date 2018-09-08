cmake_minimum_required(VERSION 3.1)
cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project(cinq)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin)

include_directories(${CMAKE_HOME_DIRECTORY}/3rdparty)
include_directories(${CMAKE_HOME_DIRECTORY}/include)

enable_testing()
add_subdirectory(test)
