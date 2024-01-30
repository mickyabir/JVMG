add_library(parser parser.cpp codeParser.cpp)
target_include_directories(parser
        PUBLIC
        "${PROJECT_SOURCE_DIR}/include"
)
