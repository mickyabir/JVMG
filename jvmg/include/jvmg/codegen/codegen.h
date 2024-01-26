//
// Created by Micky on 1/25/2024.
//

#ifndef _CODEGEN_H
#define _CODEGEN_H

#include "jvmg/IR/classfile.h"

#include <sstream>

using namespace jvmg;

void outputBinaryFromClassfile(ClassFile &classFile, const std::string& filename) {
    std::ofstream outputStream(filename, std::ios::out | std::ios::binary);

    std::vector<std::uint8_t> bytes = classFile.serialize();

    for (auto byte : bytes) {
        outputStream.write((char *) &byte, sizeof(std::uint8_t));
    }
}

#endif //_CODEGEN_H
