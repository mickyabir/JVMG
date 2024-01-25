//
// Created by Micky on 1/22/2024.
//

#ifndef _PARSER_H
#define _PARSER_H

#include "jvmg/bytecode/reader.h"
#include "jvmg/IR/instruction.h"

#include <cassert>
#include <sstream>

namespace jvmg {
    class ParserContext {
    public:
        void addConstantToPool(ClassFile::CPInfo constant) { constantPool.push_back(constant); }
        std::string getConstantUTF8(int idx) {
            // Constant pool indices start at 1, so subtract 1
            int constantIdx = idx - 1;
            assert(constantIdx < constantPool.size() - 1);

            auto cpInfo = constantPool.at(constantIdx);
            assert(cpInfo.tag == ClassFile::CPInfo::CONSTANT_Utf8);
            ClassFile::ConstUTF8Info *utf8Info = cpInfo.asUTF8Info();

            std::stringstream s;

            for (int i = 0; i < utf8Info->getLength(); i++) {
                s << utf8Info->getByte(i);
            }

            return s.str();
        }
    private:
        std::vector<ClassFile::CPInfo> constantPool;
    };

    class Parser {
    public:
        Parser(Reader *reader) {
            this->reader = reader;
            this->context = new ParserContext();
        }

        ~Parser() {
            delete context;
        }

        ClassFile consumeClassFile();
        void consumeMagic();
        ClassFile::CPInfo consumeConstantPoolInfo();
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
