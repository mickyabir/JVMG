//
// Created by Micky on 1/22/2024.
//

#ifndef _READER_H
#define _READER_H

#include "jvmg/IR/classfile.h"

#include <cstdint>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <bitset>

namespace jvmg {
    class Reader {
    public:
        explicit Reader(const char *filename) {
            this->filename = filename;
            srcFile = std::ifstream(filename, std::ios_base::binary);
        }

        std::uint8_t readByte() {
            std::uint8_t byte;
            srcFile.read((char *)&byte, 1);
            return byte;
        }

    private:
        const char *filename;
        std::ifstream srcFile;
    };
}

#endif //_READER_H
