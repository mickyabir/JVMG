#include "jvmg/bytecode/parser/parser.h"
#include "jvmg/IR/ConstantPool/constantPoolInfo.h"

using namespace jvmg;

std::string ParserContext::getConstantUTF8(int idx) const {
    // Constant pool indices start at 1, so subtract 1
    int constantIdx = idx - 1;
    assert(constantIdx <= constantPool.size() - 1);

    auto cpInfo = constantPool.at(constantIdx);
    assert(cpInfo.tag == CPInfo::CONSTANT_Utf8);
    ConstUTF8Info *utf8Info = cpInfo.asUTF8Info();

    std::stringstream s;

    for (int i = 0; i < utf8Info->getLength(); i++) {
        s << utf8Info->getByte(i);
    }

    return s.str();
}

CPInfo ParserContext::getConstant(int idx) const {
    return constantPool.at(idx - 1);
}

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
    std::vector<AttributeInfo*> attributes;
    AttributeInfo *attributesInfo;
    for (int i = 0; i < attributesCount; i++) {
        attributesInfo = consumeAttributesInfo();
        attributes.push_back(attributesInfo);
    }

    return {minorVersion, majorVersion, constantPoolCount, constantPool, accessFlags, thisClass, superClass, interfacesCount, interfaces, fieldsCount, fields, methodsCount, methods, attributesCount, attributes};
}

void Parser::consumeMagic() {
    assert(consumeOneByte() == 0xCA);
    assert(consumeOneByte() == 0xFE);
    assert(consumeOneByte() == 0xBA);
    assert(consumeOneByte() == 0xBE);
}

CPInfo Parser::consumeConstantPoolInfo() {
    std::uint8_t tag = consumeOneByte();

    switch(tag) {
        case CPInfo::ConstantType::CONSTANT_Class: {
            auto nameIndex = consumeTwoBytes();
            return ConstClassInfo(nameIndex);
        }
        case CPInfo::ConstantType::CONSTANT_String: {
            auto stringIndex = consumeTwoBytes();
            return ConstStringInfo(stringIndex);
        }
        case CPInfo::ConstantType::CONSTANT_MethodType: {
            auto descriptorIndex = consumeTwoBytes();
            return ConstMethodTypeInfo(descriptorIndex);
        }
        case CPInfo::ConstantType::CONSTANT_MethodHandle: {
            auto referenceKind = consumeOneByte();
            auto referenceIndex = consumeTwoBytes();
            return ConstMethodHandleInfo(referenceKind, referenceIndex);
        }
        case CPInfo::ConstantType::CONSTANT_Fieldref: {
            auto classIndex = consumeTwoBytes();
            auto nameAndTypeIndex = consumeTwoBytes();
            return ConstFieldRefInfo(classIndex, nameAndTypeIndex);
        }
        case CPInfo::ConstantType::CONSTANT_Methodref: {
            auto classIndex = consumeTwoBytes();
            auto nameAndTypeIndex = consumeTwoBytes();
            return ConstMethodRefInfo(classIndex, nameAndTypeIndex);
        }
        case CPInfo::ConstantType::CONSTANT_InterfaceMethodref: {
            auto classIndex = consumeTwoBytes();
            auto nameAndTypeIndex = consumeTwoBytes();
            return ConstInterfaceMethodRefInfo(classIndex, nameAndTypeIndex);
        }
        case CPInfo::ConstantType::CONSTANT_NameAndType: {
            auto nameIndex = consumeTwoBytes();
            auto descriptorIndex = consumeTwoBytes();
            return ConstNameAndType(nameIndex, descriptorIndex);
        }
        case CPInfo::ConstantType::CONSTANT_Integer: {
            auto bytes = consumeFourBytes();
            return ConstIntegerInfo(bytes);
        }
        case CPInfo::ConstantType::CONSTANT_Float: {
            auto bytes = consumeFourBytes();
            return ConstFloatInfo(bytes);
        }
        case CPInfo::ConstantType::CONSTANT_InvokeDynamic: {
            auto bootstrapMethodAttrIndex = consumeTwoBytes();
            auto nameAndTypeIndex = consumeTwoBytes();
            return ConstInvokeDynamicInfo(bootstrapMethodAttrIndex, nameAndTypeIndex);
        }
        case CPInfo::ConstantType::CONSTANT_Utf8:
        {
            std::uint16_t length = consumeTwoBytes();
            std::vector<std::uint8_t> bytes;
            for (int i = 0; i < length; i++) {
                bytes.push_back(consumeOneByte());
            }
            return ConstUTF8Info(length, bytes);
        }
        case CPInfo::ConstantType::CONSTANT_Long: {
            auto highBytes = consumeFourBytes();
            auto lowBytes = consumeFourBytes();
            return ConstLongInfo(highBytes, lowBytes);
        }
        case CPInfo::ConstantType::CONSTANT_Double: {
            auto highBytes = consumeFourBytes();
            auto lowBytes = consumeFourBytes();
            return ConstDoubleInfo(highBytes, lowBytes);
        }
        default:
            throw std::invalid_argument("Constant pool info tag invalid.");
    }
}

AttributeInfo *Parser::consumeAttributesInfo() {
    std::uint16_t attributeNameIndex = consumeTwoBytes();
    std::uint32_t attributeLength = consumeFourBytes();
    Attribute *info = nullptr;
    std::string attributeName = context->getConstantUTF8(attributeNameIndex);
    AttributeInfo::AttributeNameTag attributeNameTag = AttributeInfo::getAttributeNameTag(attributeName);

    auto *attributeInfo = new AttributeInfo(attributeNameIndex, attributeLength, info);
    attributeInfo->setAttributeName(attributeName);

    switch(attributeNameTag) {
        case AttributeInfo::CODE: {
            std::uint16_t maxStack = consumeTwoBytes();
            std::uint16_t maxLocals = consumeTwoBytes();

            std::uint32_t codeLength = consumeFourBytes();
            std::vector<Instruction> code;

            // Code length is in bytes, and instructions are variable-length
            // Keep track of bytes consumed
            context->setCodeStartOffset(context->getByteOffset());
            for (size_t i = 0; i < codeLength;) {
                code.push_back((consumeInstruction()));
                i += code.back().getSizeInBytes();
            }

            std::uint16_t exceptionTableLength = consumeTwoBytes();
            std::vector<CodeAttribute::ExceptionTableEntry> exceptionTable;
            for (int i = 0; i < exceptionTableLength; i++) {
                std::uint16_t startPC = consumeTwoBytes();
                std::uint16_t endPC = consumeTwoBytes();
                std::uint16_t handlerPC = consumeTwoBytes();
                std::uint16_t catchType = consumeTwoBytes();
                exceptionTable.push_back({startPC, endPC, handlerPC, catchType});
            }

            std::uint16_t attributesCount = consumeTwoBytes();
            std::vector<AttributeInfo*> attributes;
            for (int i = 0; i < attributesCount; i++) {
                attributes.push_back(consumeAttributesInfo());
            }
            info = new CodeAttribute(maxStack, maxLocals, codeLength, code, exceptionTableLength, exceptionTable, attributesCount, attributes);
            break;
        }
        case AttributeInfo::LINE_NUMBER_TABLE: {
            std::uint16_t lineNumberTableLength = consumeTwoBytes();
            std::vector<LineNumberAttribute::LineNumberTableEntry> lineNumberTable;
            for (int i = 0; i < lineNumberTableLength; i++) {
                std::uint16_t startPC = consumeTwoBytes();
                std::uint16_t lineNumber = consumeTwoBytes();
                lineNumberTable.push_back({startPC, lineNumber});
            }
            info = new LineNumberAttribute(lineNumberTableLength, lineNumberTable);
            break;
        }
        case AttributeInfo::SOURCE_FILE: {
            std::uint16_t sourceFileIndex = consumeTwoBytes();
            info = new SourceFileAttribute(sourceFileIndex, attributeName);
            break;
        }
        default:
            std::cout << "AttributeNameTag: " << attributeNameTag << std::endl;
            throw std::invalid_argument("Invalid argument: " + attributeName);
    }

    attributeInfo->info = info;
    return attributeInfo;
}

ClassFile::FieldInfo Parser::consumeFieldInfo() {
    std::uint16_t accessFlags = consumeTwoBytes();
    std::uint16_t nameIndex = consumeTwoBytes();
    std::uint16_t descriptorIndex = consumeTwoBytes();
    std::uint16_t attributesCount = consumeTwoBytes();

    std::vector<AttributeInfo*> attributes;
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
    std::vector<AttributeInfo*> attributes;

    for (int i = 0; i < attributesCount; i++) {
        attributes.push_back(consumeAttributesInfo());
    }

    return {accessFlags, nameIndex, descriptorIndex, attributesCount, attributes};
}

std::uint8_t Parser::consumeOneByte() {
    context->incrementByteOffset();
    return reader->readByte();
}

std::uint16_t Parser::consumeTwoBytes() {
    std::uint8_t hbyte = reader->readByte();
    context->incrementByteOffset();
    std::uint8_t lbyte = reader->readByte();
    context->incrementByteOffset();
    return (hbyte << 8) | lbyte;
}

std::uint32_t Parser::consumeFourBytes() {
    std::uint8_t hhbyte = reader->readByte();
    context->incrementByteOffset();
    std::uint8_t hlbyte = reader->readByte();
    context->incrementByteOffset();
    std::uint8_t lhbyte = reader->readByte();
    context->incrementByteOffset();
    std::uint8_t llbyte = reader->readByte();
    context->incrementByteOffset();
    return (hhbyte << 24) | (hlbyte << 16) | ( lhbyte << 8) | llbyte;
}