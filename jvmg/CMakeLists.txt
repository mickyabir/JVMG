cmake_minimum_required(VERSION 3.27)
project(.)

set(CMAKE_CXX_STANDARD 23)

add_executable(. main.cpp
        include/jvmg/IR/type.h
        include/jvmg/IR/value.h
        include/jvmg/IR/instruction.h
        src/IR/instruction.cpp
        include/jvmg/IR/classfile.h
        include/jvmg/bytecode/reader.h
        include/jvmg/bytecode/parser/parser.h
        src/bytecode/parser/parser.cpp
        include/jvmg/IR/attribute.h
        src/IR/attribute.cpp
        src/bytecode/parser/codeParser.cpp)

target_include_directories(. PRIVATE "include")
