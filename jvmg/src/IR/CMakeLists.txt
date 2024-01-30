add_subdirectory(ConstantPool)

add_library(IR
        attribute.cpp
        classfile.cpp
        instruction.cpp
)
target_link_libraries(IR
        PUBLIC
        ConstantPool
)
target_include_directories(IR
        PUBLIC
        "${PROJECT_SOURCE_DIR}/include"
)
