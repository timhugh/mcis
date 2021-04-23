add_executable(${PROJECT_NAME} main.cpp)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)

include(FetchContent)

FetchContent_Declare(
    spdlog
    GIT_REPOSITORY  https://github.com/gabime/spdlog.git
    GIT_TAG         v1.8.5
)
FetchContent_MakeAvailable(spdlog)

FetchContent_Declare(
    httplib
    GIT_REPOSITORY  https://github.com/yhirose/cpp-httplib.git
    GIT_TAG         v0.8.7
)
FetchContent_MakeAvailable(httplib)

target_link_libraries(
    ${PROJECT_NAME}
PRIVATE
    httplib::httplib
    spdlog::spdlog
    postgis
)
