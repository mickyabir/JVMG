//
// Created by Micky on 1/22/2024.
//

#ifndef _PARSER_H
#define _PARSER_H

#include "jvmg/bytecode/reader.h"
#include "jvmg/IR/instruction.h"
#include "jvmg/IR/ConstantPool/constantPoolInfo.h"

#include <cassert>
#include <sstream>

namespace jvmg {
    class ParserContext {
    public:
        ParserContext() : byteOffset(0), codeStartOffset(0) {}

        void incrementByteOffset() { byteOffset += 1; }
        long long getByteOffset() { return byteOffset; }

        void setCodeStartOffset(long long offset) { codeStartOffset = offset; }
        long long getCodeStartOffset() { return codeStartOffset; }

        void addConstantToPool(const CPInfo& constant) { constantPool.push_back(constant); }
        std::string getConstantUTF8(int idx);
    private:
        std::vector<CPInfo> constantPool;
        long long byteOffset;
        long long codeStartOffset;
    };

    class Parser {
    public:
        explicit Parser(Reader *reader) : reader(reader), context(new ParserContext()) {}

        ~Parser() {
            delete context;
        }

        ClassFile consumeClassFile();

        void consumeMagic();
        CPInfo consumeConstantPoolInfo();
        ClassFile::FieldInfo consumeFieldInfo();
        ClassFile::MethodInfo consumeMethodInfo();
        AttributeInfo *consumeAttributesInfo();

        Instruction consumeInstruction();

    private:
        std::uint8_t consumeOneByte();
        std::uint16_t consumeTwoBytes();
        std::uint32_t consumeFourBytes();

        Reader *reader;
        ParserContext *context;
    };
}

#endif //_PARSER_H
