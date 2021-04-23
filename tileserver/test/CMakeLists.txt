include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY  https://github.com/google/googletest.git
    GIT_TAG         release-1.10.0
)
FetchContent_MakeAvailable(googletest)

set(TARGET ${PROJECT_NAME}-test)
add_executable(${TARGET} test_main.cpp)
target_compile_features(${TARGET} PRIVATE cxx_std_20)
target_link_libraries(${TARGET}
PRIVATE
    gtest
    gtest_main
)
add_test(NAME ${TARGET} COMMAND ${TARGET})
