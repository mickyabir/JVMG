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
#include "jvmg/util/util.h"

namespace jvmg {
    struct AttributeInfo;
    struct CodeInfo;

    struct Info : public Serializable {
    };

    struct CodeInfo : public Info {
        struct ExceptionTableEntry : public Serializable {
        public:
            ExceptionTableEntry(std::uint16_t startPC,
                std::uint16_t endPC,
                std::uint16_t handlerPC,
                std::uint16_t catchType)
                : startPC(startPC),
                endPC(endPC),
                handlerPC(handlerPC),
                catchType(catchType) {}

            std::uint16_t startPC;
            std::uint16_t endPC;
            std::uint16_t handlerPC;
            std::uint16_t catchType;
        private:
            void _serialize() override {
                serializeBytes(startPC);
                serializeBytes(endPC);
                serializeBytes(handlerPC);
                serializeBytes(catchType);
            }
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

    private:
        void _serialize() override {
            serializeBytes(maxStack);
            serializeBytes(maxStack);
            serializeBytes(maxLocals);
            serializeBytes(codeLength);
            for (auto& inst : code) {
                insertBytes(inst.serialize());
            }
            serializeBytes(exceptionTableLength);
            for (auto& exception : exceptionTable) {
                insertBytes(exception.serialize());
            }
        }
    };

    struct LineNumberInfo : Info {
        struct LineNumberTableEntry : public Serializable {
        public:
            LineNumberTableEntry(std::uint16_t startPC,
                std::uint16_t lineNumber)
                : startPC(startPC),
                lineNumber(lineNumber) {}

            std::uint16_t startPC;
            std::uint16_t lineNumber;
        private:
            void _serialize() override {
                serializeBytes(startPC);
                serializeBytes(lineNumber);
            }
        };

        LineNumberInfo(std::uint16_t lineNumberTableLength,
                std::vector<LineNumberTableEntry> lineNumberTable)
                : lineNumberTableLength(lineNumberTableLength),
                lineNumberTable(std::move(lineNumberTable)) {}

        std::uint16_t lineNumberTableLength;
        std::vector<LineNumberTableEntry> lineNumberTable;

    private:
        void _serialize() override {
            serializeBytes(lineNumberTableLength);
            for (auto& lineNumberTableEntry : lineNumberTable) {
                insertBytes(lineNumberTableEntry.serialize());
            }
        }
    };

    struct SourceFileInfo : Info {
        SourceFileInfo(std::uint16_t sourceFileIndex, std::string sourceFileName) : sourceFileIndex(sourceFileIndex), sourceFileName(sourceFileName) {}

        std::uint16_t sourceFileIndex;
    private:
        void _serialize() override {
            serializeBytes(sourceFileIndex);
        }

        std::string sourceFileName;
    };

    struct AttributeInfo : Serializable {
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
        void _serialize() {
            serializeBytes(attributeNameIndex);
            serializeBytes(attributeLength);
            insertBytes(info->serialize());
        }
        std::string attributeName;
    };
}
#endif //_ATTRIBUTE_H
