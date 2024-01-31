add_executable(javap javap.cpp)
target_include_directories(javap
        PUBLIC
        "${PROJECT_SOURCE_DIR}/include"
)

target_link_libraries(
        javap
        JVMGLib
)
