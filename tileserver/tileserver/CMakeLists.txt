add_executable(tileserver main.cpp)
target_compile_features(tileserver PRIVATE cxx_std_20)

target_link_libraries(
    tileserver
PRIVATE
    httplib::httplib
    spdlog::spdlog
    PostgreSQL::PostgreSQL
    ioc
)