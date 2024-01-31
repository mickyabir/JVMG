add_subdirectory(IR)
add_subdirectory(parser)
add_subdirectory(util)

add_library(JVMGLib jvmg.cpp)

target_link_libraries(JVMGLib
        PUBLIC
        parser
        IR
        util
)