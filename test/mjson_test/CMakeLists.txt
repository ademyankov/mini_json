project(mjson_test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_PLATFORM_INDEPENDENT_CODE ON)

#find_path(ITRU_HPP NAMES itru.hpp HINTS "${CMAKE_SOURCE_DIR}/apps/getspsidentity/src")
#message(STATUS "ITRU_HPP=${ITRU_HPP}")

add_executable(${PROJECT_NAME}
    src/mjson_test.cpp
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
	mjson
	Catch2
)

add_test(${PROJECT_NAME} ${PROJECT_NAME})
