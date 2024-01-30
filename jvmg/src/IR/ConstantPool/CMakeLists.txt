add_library(ConstantPool constantPoolInfo.cpp ConstUTF8.cpp)
target_include_directories(ConstantPool
        PUBLIC
        "${PROJECT_SOURCE_DIR}/include"
)
