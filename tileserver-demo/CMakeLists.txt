cmake_minimum_required(VERSION 3.14)
project(tileserver-demo VERSION 1.0 LANGUAGES CXX)
add_executable(tileserver-demo demo.cpp)
target_compile_features(tileserver-demo PRIVATE cxx_std_17)

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

FetchContent_MakeAvailable(httplib spdlog)
target_link_libraries(
    tileserver-demo
    PRIVATE
        spdlog::spdlog
        httplib::httplib
        PostgreSQL::PostgreSQL
)