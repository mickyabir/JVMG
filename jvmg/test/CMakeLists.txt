add_executable(tests test.cpp)
target_include_directories(tests
        PUBLIC
        "${PROJECT_SOURCE_DIR}/include"
)

target_link_libraries(
        tests
        JVMGLib
        GTest::gtest
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests)
