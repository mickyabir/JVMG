//
// Created by Micky on 1/22/2024.
//

#ifndef _CLASSFILE_H
#define _CLASSFILE_H

#define CLASS_MAGIC 0xCAFEBABE

#include "jvmg/IR/attribute.h"
#include "jvmg/IR/ConstantPool/constantPoolInfo.h"

#include <cstdint>
#include <utility>
#include <vector>

namespace jvmg {
    class ClassFile : public Serializable {
    public:
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

        struct FieldInfo : Serializable {
        public:
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

            FieldInfo(std::uint16_t accessFlags,
                std::uint16_t nameIndex,
                std::uint16_t descriptorIndex,
                std::uint16_t attributesCount,
                std::vector<AttributeInfo*> attributes)
                : accessFlags(accessFlags),
                nameIndex(nameIndex),
                descriptorIndex(descriptorIndex),
                attributesCount(attributesCount),
                attributes(std::move(attributes)) {}

            std::uint16_t accessFlags;
            std::uint16_t nameIndex;
            std::uint16_t descriptorIndex;
            std::uint16_t attributesCount;
            std::vector<AttributeInfo*> attributes;
        private:
            void _serialize() override;
        };

        struct MethodInfo : public Serializable {
        public:
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
                std::vector<AttributeInfo*> attributes)
                : accessFlags(accessFlags),
                nameIndex(nameIndex),
                descriptorIndex(descriptorIndex),
                attributesCount(attributesCount),
                attributes(std::move(attributes)) {}

            std::uint16_t accessFlags;
            std::uint16_t nameIndex;
            std::uint16_t descriptorIndex;
            std::uint16_t attributesCount;
            std::vector<AttributeInfo*> attributes;

        private:
            void _serialize() override;
        };

        ClassFile(const std::uint16_t &minorVersion, const std::uint16_t &majorVersion,
                  const std::uint16_t &constantPoolCount, std::vector<CPInfo> constantPool, const std::uint16_t &accessFlags,
                  const std::uint16_t &thisClass, const std::uint16_t &superClass, const std::uint16_t &interfaceCount,
                  std::vector<std::uint16_t> interfaces, const std::uint16_t &fieldsCount, std::vector<FieldInfo> fields,
                  const std::uint16_t &methodCount, std::vector<MethodInfo> methods, const std::uint16_t &attributesCount,
                  std::vector<AttributeInfo*> attributes) : minorVersion(minorVersion), majorVersion(majorVersion),
                                                            constantPoolCount(constantPoolCount), constantPool(std::move(constantPool)),
                                                            accessFlags(accessFlags), thisClass(thisClass), superClass(superClass),
                                                            interfaceCount(interfaceCount), interfaces(std::move(interfaces)),
                                                            fieldsCount(fieldsCount), fields(std::move(fields)), methodsCount(methodCount),
                                                            methods(std::move(methods)), attributesCount(attributesCount),
                                                            attributes(std::move(attributes)) {}

        ~ClassFile() {
            for (auto attribute : attributes) {
                delete attribute;
            }
            attributes.clear();
        }

        [[nodiscard]] std::uint16_t getMinorVersion() const { return minorVersion; }
        [[nodiscard]] std::uint16_t getMajorVersion() const { return majorVersion; }
        [[nodiscard]] std::uint16_t getConstantPoolCount() const { return constantPoolCount; }
        [[nodiscard]] const std::vector<CPInfo>& getConstantPool() const { return constantPool; }
        [[nodiscard]] std::uint16_t getAccessFlags() const { return accessFlags; }
        [[nodiscard]] std::uint16_t getThisClass() const { return thisClass; }
        [[nodiscard]] std::uint16_t getSuperClass() const { return superClass; }
        [[nodiscard]] std::uint16_t getInterfaceCount() const { return interfaceCount; }
        [[nodiscard]] const std::vector<std::uint16_t>& getInterfaces() const { return interfaces; }
        [[nodiscard]] std::uint16_t getMethodsCount() const { return methodsCount; }
        [[nodiscard]] const std::vector<MethodInfo>& getMethods() const { return methods; }
        [[nodiscard]] std::uint16_t getFieldsCount() const { return fieldsCount; }
        [[nodiscard]] const std::vector<FieldInfo>& getFields() const { return fields; }
        [[nodiscard]] std::uint16_t getAttributesCount() const { return attributesCount; }
        [[nodiscard]] const std::vector<AttributeInfo*>& getAttributes() const { return attributes; }

        const std::uint32_t magic = CLASS_MAGIC;
    private:
        void _serialize() override;

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
        std::uint16_t methodsCount;
        std::vector<MethodInfo> methods;
        std::uint16_t attributesCount;
        std::vector<AttributeInfo*> attributes;
    };
}

#endif //_CLASSFILE_H
