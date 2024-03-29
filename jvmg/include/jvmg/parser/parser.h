//
// Created by Micky on 1/22/2024.
//

#ifndef _PARSER_H
#define _PARSER_H

#include "jvmg/reader.h"
#include "jvmg/IR/instruction.h"
#include "jvmg/IR/ConstantPool/constantPoolInfo.h"

#include <cassert>
#include <sstream>

namespace jvmg {
    class ParserContext {
    public:
        ParserContext() : byteOffset(0), codeStartOffset(0) {}

        void incrementByteOffset() { byteOffset += 1; }
        [[nodiscard]] long long getByteOffset() const { return byteOffset; }

        void setCodeStartOffset(long long offset) { codeStartOffset = offset; }
        [[nodiscard]] long long getCodeStartOffset() const { return codeStartOffset; }

        void addConstantToPool(const CPInfo& constant) { constantPool.push_back(constant); }
        [[nodiscard]] std::string getConstantUTF8(int idx) const;

        [[nodiscard]] CPInfo getConstant(int idx) const;
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

        [[nodiscard]] ParserContext *getContext() const { return context; }

    private:
        std::uint8_t consumeOneByte();
        std::uint16_t consumeTwoBytes();
        std::uint32_t consumeFourBytes();

        Reader *reader;
        ParserContext *context;
    };
}

#endif //_PARSER_H
