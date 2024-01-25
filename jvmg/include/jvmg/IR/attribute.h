//
// Created by Micky on 1/23/2024.
//

#ifndef _ATTRIBUTE_H
#define _ATTRIBUTE_H

#include <cstdint>
#include <vector>
#include <string>
#include <map>
#include "instruction.h"

namespace jvmg {
    struct AttributeInfo;
    struct CodeInfo;

    struct Info {
    };

    struct CodeInfo : Info {
        struct ExceptionTableEntry {
            std::uint16_t startPC;
            std::uint16_t endPC;
            std::uint16_t handlerPC;
            std::uint16_t catchType;
        };

        CodeInfo(std::uint16_t maxStack,
                std::uint16_t maxLocals,
                std::uint32_t codeLength,
                std::vector<Instruction> code,
                std::uint16_t exceptionTableLength,
                std::vector<ExceptionTableEntry> exceptionTable,
                std::uint16_t attributesCount,
                std::vector<AttributeInfo*> attributes)
        : maxStack(maxStack),
        maxLocals(maxLocals),
        codeLength(codeLength),
        code(std::move(code)),
        exceptionTableLength(exceptionTableLength),
        exceptionTable(std::move(exceptionTable)),
        attributesCount(attributesCount),
        attributes(std::move(attributes)) {}

        std::uint16_t maxStack;
        std::uint16_t maxLocals;
        std::uint32_t codeLength;
        std::vector<Instruction> code;
        std::uint16_t exceptionTableLength;
        std::vector<ExceptionTableEntry> exceptionTable;
        std::uint16_t attributesCount;
        std::vector<AttributeInfo*> attributes;
    };

    struct LineNumberInfo : Info {
        struct LineNumberTableEntry {
            std::uint16_t startPC;
            std::uint16_t lineNumber;
        };

        LineNumberInfo(std::uint16_t lineNumberTableLength,
                std::vector<LineNumberTableEntry> lineNumberTable)
                : lineNumberTableLength(lineNumberTableLength),
                lineNumberTable(std::move(lineNumberTable)) {}

        std::uint16_t lineNumberTableLength;
        std::vector<LineNumberTableEntry> lineNumberTable;
    };

    struct SourceFileAttribute : Info {
        SourceFileAttribute(std::uint16_t sourceFileIndex) : sourceFileIndex(sourceFileIndex) {}
        std::uint16_t sourceFileIndex;
    };

    struct AttributeInfo {
        enum AttributeNameTag {
            CONSTANT_VALUE = 0,
            CODE,
            STACK_MAP_TABLE,
            EXCEPTIONS,
            INNER_CLASSES,
            ENCLOSING_METHOD,
            SYNTHETIC,
            SIGNATURE,
            SOURCE_FILE,
            SOURCE_DEBUG_EXTENSION,
            LINE_NUMBER_TABLE,
            LOCAL_VARIABLE_TABLE,
            LOCAL_VARIABLE_TYPE_TABLE,
            DEPRECATED,
            RUNTIME_VISIBLE_ANNOTATIONS,
            RUNTIME_INVISIBLE_ANNOTATIONS,
            RUNTIME_VISIBLE_PARAMETER_ANNOTATIONS,
            RUNTIME_INVISIBLE_PARAMETER_ANNOTATIONS,
            ANNOTATION_DEFAULT,
            BOOTSTRAP_METHODS
        };

        AttributeInfo(
                std::uint16_t attributeNameIndex,
                std::uint32_t attributeLength,
                Info *info) :
                attributeNameIndex(attributeNameIndex),
                attributeLength(attributeLength),
                info(info) {}

        ~AttributeInfo() {
            delete info;
        }

        static std::map<std::string, AttributeNameTag> attributeNameTagMap;

        static AttributeNameTag getAttributeNameTag(std::string attributeName) {
            return attributeNameTagMap[attributeName];
        }


        void setAttributeName(std::string attributeName) {
            this->attributeName = attributeName;
        }

        std::string getAttributeName() {
            return this->attributeName;
        }

        std::uint16_t attributeNameIndex;
        std::uint32_t attributeLength;
        Info *info;

    private:
        std::string attributeName;
    };
}
#endif //_ATTRIBUTE_H
