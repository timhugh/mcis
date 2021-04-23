cmake_minimum_required(VERSION 3.16)
project(tileserver VERSION 1.0 LANGUAGES CXX)

find_package(PostgreSQL REQUIRED)

include(FetchContent)

FetchContent_Declare(
    spdlog
    GIT_REPOSITORY  https://github.com/gabime/spdlog.git
    GIT_TAG         v1.8.5
)
FetchContent_Declare(
    httplib
    GIT_REPOSITORY  https://github.com/yhirose/cpp-httplib.git
    GIT_TAG         v0.8.7
)
FetchContent_MakeAvailable(
    spdlog
    httplib
)

add_subdirectory(tileserver)
add_subdirectory(tests)