cmake_minimum_required(VERSION 3.10)

project(mini_json)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(Catch2)

set(SOURCE src/mjson.cpp)
add_executable(${PROJECT_NAME} ${SOURCE})

target_link_libraries(${PROJECT_NAME} Catch2)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        include
)

add_subdirectory(test)
