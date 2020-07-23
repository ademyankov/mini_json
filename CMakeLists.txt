cmake_minimum_required(VERSION 3.10)

project(mjson)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

add_library(mjson INTERFACE)
target_include_directories(mjson INTERFACE include)

add_subdirectory(apps)
add_subdirectory(test)
