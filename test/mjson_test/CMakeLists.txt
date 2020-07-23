project(mjson_test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(Catch2)

add_executable(${PROJECT_NAME}
    src/mjson_test.cpp
)

target_link_libraries(${PROJECT_NAME} mjson Catch2)

add_test(${PROJECT_NAME} ${PROJECT_NAME})
