cmake_minimum_required(VERSION 3.16)

project(
    postgis
    VERSION 1.0
    LANGUAGES CXX
)

find_package(PostgreSQL REQUIRED)

add_library(postgis
    src/db.cpp
)

target_include_directories(postgis
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(postgis PRIVATE cxx_std_20)
target_compile_options(postgis PRIVATE -Wall -Wextra -Wpedantic)

target_link_libraries(postgis
    PRIVATE
        PostgreSQL::PostgreSQL
)

include(GNUInstallDirs)

install(TARGETS postgis
    EXPORT postgis-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDR}
)

install(EXPORT postgis-targets
    FILE
        postgisTargets.cmake
    NAMESPACE
        postgis::
    DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/cmake/postgis
)
