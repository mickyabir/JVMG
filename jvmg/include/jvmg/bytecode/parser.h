//
// Created by Micky on 1/22/2024.
//

#ifndef _PARSER_H
#define _PARSER_H

#include "jvmg/bytecode/reader.h"

#include <cassert>

namespace jvmg {
    class Parser {
    public:
        Parser(Reader *reader) {
            this->reader = reader;
        }

        ClassFile consumeClassFile();
        void consumeMagic();
        ClassFile::CPInfo consumeConstantPoolInfo();
        ClassFile::FieldInfo consumeFieldInfo();
        ClassFile::MethodInfo consumeMethodInfo();
        ClassFile::AttributeInfo consumeAttributesInfo();

    private:
        std::uint8_t consumeOneByte();
        std::uint16_t consumeTwoBytes();
        std::uint32_t consumeFourBytes();

        Reader *reader;
    };
}

#endif //_PARSER_H
