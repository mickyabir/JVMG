#include "jvmg/bytecode/parser.h"

using namespace jvmg;

using CPInfo = ClassFile::CPInfo;

ClassFile Parser::consumeClassFile() {
    consumeMagic();

    std::uint16_t minorVersion = consumeTwoBytes();
    std::uint16_t majorVersion = consumeTwoBytes();

    std::uint16_t constantPoolCount = consumeTwoBytes();

    std::vector<CPInfo> constantPool;

    // Constant pool count is 1-indexed
    for (int i = 1; i < constantPoolCount; i++) {
        CPInfo cpInfo = consumeConstantPoolInfo();
        constantPool.push_back(cpInfo);
        context->addConstantToPool(cpInfo);
    }

    std::uint16_t accessFlags = consumeTwoBytes();
    std::uint16_t thisClass = consumeTwoBytes();
    std::uint16_t superClass = consumeTwoBytes();

    std::uint16_t interfacesCount = consumeTwoBytes();
    std::vector<std::uint16_t> interfaces;
    for (int i = 0; i < interfacesCount; i++) {
        interfaces.push_back(consumeTwoBytes());
    }

    std::uint16_t fieldsCount = consumeTwoBytes();
    std::vector<ClassFile::FieldInfo> fields;
    for (int i = 0; i < fieldsCount; i++) {
        ClassFile::FieldInfo fieldInfo = consumeFieldInfo();
        fields.push_back(fieldInfo);
    }

    std::uint16_t methodsCount = consumeTwoBytes();
    std::vector<ClassFile::MethodInfo> methods;
    for (int i = 0; i < methodsCount; i++) {
        ClassFile::MethodInfo methodInfo = consumeMethodInfo();
        methods.push_back(methodInfo);
    }

    std::uint16_t attributesCount = consumeTwoBytes();
    std::vector<AttributeInfo> attributes;
    for (int i = 0; i < attributesCount; i++) {
        AttributeInfo attributesInfo = consumeAttributesInfo();
        attributes.push_back(attributesInfo);
    }

    return {minorVersion, majorVersion, constantPoolCount, constantPool, accessFlags, thisClass, superClass, interfacesCount, interfaces, fieldsCount, fields, methodsCount, methods, attributesCount, attributes};
}

void Parser::consumeMagic() {
    assert(reader->readByte() == 0xCA);
    assert(reader->readByte() == 0xFE);
    assert(reader->readByte() == 0xBA);
    assert(reader->readByte() == 0xBE);
}

CPInfo Parser::consumeConstantPoolInfo() {
    std::uint8_t tag = consumeOneByte();
    std::vector<std::uint8_t> info;

    switch(tag) {
        case CPInfo::ConstantType::CONSTANT_Class:
            info.push_back(consumeOneByte());
            info.push_back(consumeOneByte());
            break;
        case CPInfo::ConstantType::CONSTANT_Fieldref:
        case CPInfo::ConstantType::CONSTANT_Methodref:
        case CPInfo::ConstantType::CONSTANT_InterfaceMethodref:
        case CPInfo::ConstantType::CONSTANT_NameAndType:
            info.push_back(consumeOneByte());
            info.push_back(consumeOneByte());
            info.push_back(consumeOneByte());
            info.push_back(consumeOneByte());
            break;
        case CPInfo::ConstantType::CONSTANT_Utf8:
        {
            std::uint16_t length = consumeTwoBytes();
            std::uint8_t lengthHighBits = (length & 0xFF00) >> 8;
            std::uint8_t lengthLowBits = (length & 0x00FF);
            info.push_back(lengthHighBits);
            info.push_back(lengthLowBits);
            for (int i = 0; i < length; i++) {
                info.push_back(consumeOneByte());
            }
            break;
        }
        default:
            throw std::invalid_argument("Constant pool info tag invalid.");
    }

    return {(CPInfo::ConstantType) tag, info};
}

AttributeInfo Parser::consumeAttributesInfo() {
    std::uint16_t attributeNameIndex = consumeTwoBytes();
    std::uint32_t attributeLength = consumeFourBytes();
    std::vector<std::uint8_t> info;

    std::string attributeName = context->getConstantUTF8(attributeNameIndex);

    if (attributeName == "Code") {
        std::uint16_t maxStack = consumeTwoBytes();
        std::uint16_t maxLocals = consumeTwoBytes();

        std::uint32_t codeLength = consumeFourBytes();
        std::vector<std::uint8_t> code;
        for (int i = 0; i < codeLength; i++) {
            code.push_back((consumeOneByte()));
        }

        std::uint16_t exceptionTableLength;
        std::vector<CodeAttribute::ExceptionTableEntry> exceptionTable;
        for (int i = 0; i < exceptionTableLength; i++) {
            std::uint16_t startPC = consumeTwoBytes();
            std::uint16_t endPC = consumeTwoBytes();
            std::uint16_t handlerPC = consumeTwoBytes();
            std::uint16_t catchType = consumeTwoBytes();
            exceptionTable.push_back({startPC, endPC, handlerPC, catchType});
        }

        std::uint16_t attributesCount;
        std::vector<AttributeInfo> attributes;
        for (int i = 0; i < attributesCount; i++) {
            attributes.push_back(consumeAttributesInfo());
        }
        return CodeAttribute(attributeNameIndex, attributeLength, std::vector<std::uint8_t>(), maxStack, maxLocals, codeLength, code, exceptionTableLength, exceptionTable, attributesCount, attributes);
    } else {
        for (int i = 0; i < attributeLength; i++) {
            info.push_back(consumeOneByte());
        }

        return {attributeNameIndex, attributeLength, info};
    }
}

ClassFile::FieldInfo Parser::consumeFieldInfo() {
    std::uint16_t accessFlags = consumeTwoBytes();
    std::uint16_t nameIndex = consumeTwoBytes();
    std::uint16_t descriptorIndex = consumeTwoBytes();
    std::uint16_t attributesCount = consumeTwoBytes();

    std::vector<AttributeInfo> attributes;
    for (int i = 0; i < attributesCount; i++) {
        attributes.push_back(consumeAttributesInfo());
    }

    return {accessFlags, nameIndex, descriptorIndex, attributesCount, attributes};
}

ClassFile::MethodInfo Parser::consumeMethodInfo() {
    std::uint16_t accessFlags = consumeTwoBytes();
    std::uint16_t nameIndex = consumeTwoBytes();
    std::uint16_t descriptorIndex = consumeTwoBytes();
    std::uint16_t attributesCount = consumeTwoBytes();
    std::vector<AttributeInfo> attributes;

    for (int i = 0; i < attributesCount; i++) {
        attributes.push_back(consumeAttributesInfo());
    }

    return {accessFlags, nameIndex, descriptorIndex, attributesCount, attributes};
}

std::uint8_t Parser::consumeOneByte() {
    return reader->readByte();
}

std::uint16_t Parser::consumeTwoBytes() {
    std::uint8_t hbyte = reader->readByte();
    std::uint8_t lbyte = reader->readByte();
    return (hbyte << 8) | lbyte;
}

std::uint32_t Parser::consumeFourBytes() {
    std::uint8_t hhbyte = reader->readByte();
    std::uint8_t hlbyte = reader->readByte();
    std::uint8_t lhbyte = reader->readByte();
    std::uint8_t llbyte = reader->readByte();
    return (hhbyte << 24) | (hlbyte << 16) | ( lhbyte << 8) | llbyte;
}