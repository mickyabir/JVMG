//
// Created by Micky on 1/22/2024.
//

#ifndef _CLASSFILE_H
#define _CLASSFILE_H

#define CLASS_MAGIC 0xCAFEBABE

#include <cstdint>
#include <vector>

namespace jvmg {
    class ClassFile {
    public:
        struct CPInfo {
            enum ConstantType : std::uint8_t {
                CONSTANT_Class = 7,
                CONSTANT_Fieldref = 9,
                CONSTANT_Methodref  = 10,
                CONSTANT_InterfaceMethodref = 11,
                CONSTANT_String = 8,
                CONSTANT_Integer = 3,
                CONSTANT_Float = 4,
                CONSTANT_Long = 5,
                CONSTANT_Double = 6,
                CONSTANT_NameAndType = 12,
                CONSTANT_Utf8 = 1,
                CONSTANT_MethodHandle = 15,
                CONSTANT_MethodType = 16,
                CONSTANT_InvokeDynamic = 18,
            };

            CPInfo(ConstantType tag, std::vector<std::uint8_t> info) : tag(tag), info(info) {}

            ConstantType tag;
            std::vector<std::uint8_t> info;
        };

        enum ClassAccessFlags : std::uint16_t {
            ACC_PUBLIC = 0x0001,
            ACC_FINAL = 0x0010,
            ACC_SUPER = 0x0020,
            ACC_INTERFACE = 0x0200,
            ACC_ABSTRACT = 0x0400,
            ACC_SYNTHETIC = 0x1000,
            ACC_ANNOTATION = 0x2000,
            ACC_ENUM = 0x4000
        };

        struct AttributeInfo {
            AttributeInfo(
                    std::uint16_t attributeNameIndex,
                    std::uint32_t attributeLength,
                    std::vector<std::uint8_t> info) :
                    attributeNameIndex(attributeNameIndex),
                    attributeLength(attributeLength),
                    info(std::move(info)) {}

            std::uint16_t attributeNameIndex;
            std::uint32_t attributeLength;
            std::vector<std::uint8_t> info;
        };

        struct FieldInfo {
            enum FieldAccessFlag : std::uint16_t {
                ACC_PUBLIC = 0x0001,
                ACC_PRIVATE = 0x0002,
                ACC_PROTECTED = 0x0004,
                ACC_STATIC = 0x0008,
                ACC_FINAL = 0x0010,
                ACC_VOLATILE = 0x0040,
                ACC_TRANSIENT = 0x0080,
                ACC_SYNTHETIC = 0x1000,
                ACC_ENUM = 0x4000
            };

            std::uint16_t accessFlags;
            std::uint16_t nameIndex;
            std::uint16_t descriptorIndex;
            std::uint16_t attributesCount;
            std::vector<AttributeInfo> attributes;
        };

        struct MethodInfo {
            enum MethodAccessFlags : uint16_t {
                ACC_PUBLIC = 0x0001,
                ACC_PRIVATE = 0x0002,
                ACC_PROTECTED = 0x0004,
                ACC_STATIC = 0x0008,
                ACC_FINAL = 0x0010,
                ACC_SYNCHRONIZED = 0x0020,
                ACC_BRIDGE = 0x0040,
                ACC_VARARGS = 0x0080,
                ACC_NATIVE = 0x0100,
                ACC_ABSTRACT = 0x0400,
                ACC_STRICT = 0x0800,
                ACC_SYNTHETIC = 0x1000
            };

            MethodInfo(
                std::uint16_t accessFlags,
                std::uint16_t nameIndex,
                std::uint16_t descriptorIndex,
                std::uint16_t attributesCount,
                std::vector<AttributeInfo> attributes)
                : accessFlags(accessFlags),
                nameIndex(nameIndex),
                descriptorIndex(descriptorIndex),
                attributesCount(attributesCount),
                attributes(std::move(attributes)) {}

            std::uint16_t accessFlags;
            std::uint16_t nameIndex;
            std::uint16_t descriptorIndex;
            std::uint16_t attributesCount;
            std::vector<AttributeInfo> attributes;
        };

        ClassFile(const std::uint16_t &minorVersion, const std::uint16_t &majorVersion,
                  const std::uint16_t &constantPoolCount, std::vector<CPInfo> constantPool, const std::uint16_t &accessFlags,
                  const std::uint16_t &thisClass, const std::uint16_t &superClass, const std::uint16_t &interfaceCount,
                  std::vector<std::uint16_t> interfaces, const std::uint16_t &fieldsCount, std::vector<FieldInfo> fields,
                  const std::uint16_t &methodCount, std::vector<MethodInfo> methods, const std::uint16_t &attributesCount,
                  std::vector<AttributeInfo> attributes) : minorVersion(minorVersion), majorVersion(majorVersion),
                                                           constantPoolCount(constantPoolCount), constantPool(std::move(constantPool)),
                                                           accessFlags(accessFlags), thisClass(thisClass), superClass(superClass),
                                                           interfaceCount(interfaceCount), interfaces(std::move(interfaces)),
                                                           fieldsCount(fieldsCount), fields(std::move(fields)), methodCount(methodCount),
                                                           methods(std::move(methods)), attributesCount(attributesCount),
                                                           attributes(std::move(attributes)) {}

    private:
        const std::uint32_t magic = CLASS_MAGIC;
        std::uint16_t minorVersion;
        std::uint16_t majorVersion;
        std::uint16_t constantPoolCount;
        std::vector<CPInfo> constantPool;
        std::uint16_t accessFlags;
        std::uint16_t thisClass;
        std::uint16_t superClass;
        std::uint16_t interfaceCount;
        std::vector<std::uint16_t> interfaces;
        std::uint16_t fieldsCount;
        std::vector<FieldInfo> fields;
        std::uint16_t methodCount;
        std::vector<MethodInfo> methods;
        std::uint16_t attributesCount;
        std::vector<AttributeInfo> attributes;
    };
}

#endif //_CLASSFILE_H
