//
// Created by Micky on 1/23/2024.
//

#ifndef _ATTRIBUTE_H
#define _ATTRIBUTE_H

#include <cstdint>
#include <vector>

namespace jvmg {
    struct AttributeInfo {
        AttributeInfo(
                std::uint16_t attributeNameIndex,
                std::uint32_t attributeLength,
                std::vector <std::uint8_t> info) :
                attributeNameIndex(attributeNameIndex),
                attributeLength(attributeLength),
                info(std::move(info)) {}

        std::uint16_t attributeNameIndex;
        std::uint32_t attributeLength;
        std::vector <std::uint8_t> info;
    };

    struct CodeAttribute : AttributeInfo {
        struct ExceptionTableEntry {
            std::uint16_t startPC;
            std::uint16_t endPC;
            std::uint16_t handlerPC;
            std::uint16_t catchType;
        };

        CodeAttribute(std::uint16_t attributeNameIndex, std::uint32_t attributeLength,
                      std::vector<std::uint8_t> info,
                      std::uint16_t maxStack,
                      std::uint16_t maxLocals,
                      std::uint32_t codeLength,
                      std::vector<std::uint8_t> code,
                      std::uint16_t exceptionTableLength,
                      std::vector<ExceptionTableEntry> exceptionTable,
                      std::uint16_t attributesCount,
                      std::vector<AttributeInfo> attributes)
                : AttributeInfo(attributeNameIndex, attributeLength, std::move(info)),
                  maxStack(maxStack),
                  maxLocals(maxLocals),
                  codeLength(codeLength),
                  code(std::move(code)),
                  exceptionTableLength(exceptionTableLength),
                  exceptionTable(std::move(exceptionTable)),
                  attributesCount(attributesCount),
                  attributes(std::move(attributes)){}

        std::uint16_t maxStack;
        std::uint16_t maxLocals;
        std::uint32_t codeLength;
        std::vector<std::uint8_t> code;
        std::uint16_t exceptionTableLength;
        std::vector<ExceptionTableEntry> exceptionTable;
        std::uint16_t attributesCount;
        std::vector<AttributeInfo> attributes;
    };
}
#endif //_ATTRIBUTE_H
