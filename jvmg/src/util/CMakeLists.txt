add_library(util util.cpp)
target_include_directories(util
        PUBLIC
        "${PROJECT_SOURCE_DIR}/include"
)