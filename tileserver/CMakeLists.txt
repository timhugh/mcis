cmake_minimum_required(VERSION 3.16)

project(
    tileserver
    VERSION 1.0
    LANGUAGES CXX)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
endif()

add_subdirectory(libs/postgis)
add_subdirectory(src)

if(BUILD_TESTING)
    add_subdirectory(test)
endif()
