cmake_minimum_required(VERSION 3.27)
project(.)

set(CMAKE_CXX_STANDARD 23)

add_executable(. main.cpp
        src/IR/type.cpp
        include/jvmg/IR/type.h
        include/jvmg/IR/value.h
        include/jvmg/IR/instruction.h
        include/jvmg/IR/operand.h
        src/IR/operand.cpp
        src/IR/instruction.cpp
        include/jvmg/IR/classfile.h)

target_include_directories(. PRIVATE "include")
