//
// Created by Micky on 1/22/2024.
//

#ifndef _CLASSFILE_H
#define _CLASSFILE_H

#define CLASS_MAGIC 0xCAFEBABE

#include "jvmg/IR/attribute.h"

#include <cstdint>
#include <vector>

namespace jvmg {
    class ClassFile : public Serializable {
    public:
        struct ConstUTF8Info;

        struct CPInfo : Serializable {
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

            ConstUTF8Info *asUTF8Info() { return tag == CONSTANT_Utf8 ? (ConstUTF8Info*)this : nullptr; }

            ConstantType tag;
            std::vector<std::uint8_t> info;

        private:
            void _serialize() override {
                serializeBytes(tag);
                for (auto& byte : info) {
                    serializeBytes(byte);
                }
            }

        };

        struct ConstUTF8Info : CPInfo {
            std::uint16_t getLength() {
                return (info[0] << 8) | info[1];
            }

            std::uint8_t getByte(int idx) {
                return info[idx + 2];
            }
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
                attributesCount(attributesCount),
                attributes(std::move(attributes)) {}

            std::uint16_t accessFlags;
            std::uint16_t nameIndex;
            std::uint16_t descriptorIndex;
            std::uint16_t attributesCount;
            std::vector<AttributeInfo*> attributes;

        private:
            void _serialize() override {
                serializeBytes(accessFlags);
                serializeBytes(nameIndex);
                serializeBytes(descriptorIndex);
                serializeBytes(attributesCount);

                for (auto& attribute : attributes) {
                    insertBytes(attribute->serialize());
                }
            }
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
            void _serialize() override {
                serializeBytes(accessFlags);
                serializeBytes(nameIndex);
                serializeBytes(descriptorIndex);
                serializeBytes(attributesCount);
                for (auto& attributeInfo : attributes) {
                    insertBytes(attributeInfo->serialize());
                }
            }
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
                                                           fieldsCount(fieldsCount), fields(std::move(fields)), methodCount(methodCount),
                                                           methods(std::move(methods)), attributesCount(attributesCount),
                                                           attributes(std::move(attributes)) {}

        [[nodiscard]] const std::vector<MethodInfo>& getMethods() const { return methods; }
        [[nodiscard]] const std::vector<AttributeInfo*>& getAttributes() const { return attributes; }

        [[nodiscard]] std::uint32_t getMagic() const { return magic; }
        [[nodiscard]] std::uint16_t getMinorVersion() const { return minorVersion; }
        [[nodiscard]] std::uint16_t getMajorVersion() const { return majorVersion; }
        [[nodiscard]] std::uint16_t getConstantPoolCount() const { return constantPoolCount; }
        [[nodiscard]] const std::vector<CPInfo>& getConstantPool() const { return constantPool; }
        [[nodiscard]] std::uint16_t getAccessFlags() const { return accessFlags; }
        [[nodiscard]] std::uint16_t getThisClass() const { return thisClass; }
        [[nodiscard]] std::uint16_t getSuperClass() const { return superClass; }
        [[nodiscard]] std::uint16_t getInterfaceCount() const { return interfaceCount; }
        [[nodiscard]] const std::vector<std::uint16_t>& getInterfaces() const { return interfaces; }
        [[nodiscard]] std::uint16_t getFieldsCount() const { return fieldsCount; }
        [[nodiscard]] const std::vector<FieldInfo>& getFields() const { return fields; }

    private:
        void _serialize() override {
            serializeBytes(magic);
            serializeBytes(minorVersion);
            serializeBytes(majorVersion);

            serializeBytes(constantPoolCount);
            for (auto& constant : constantPool) {
                insertBytes(constant.serialize());
            }

            serializeBytes(accessFlags);
            serializeBytes(thisClass);
            serializeBytes(superClass);

            serializeBytes(interfaceCount);
            for (auto& interface : interfaces) {
                serializeBytes(interface);
            }

            serializeBytes(fieldsCount);
            for (auto& field : fields) {
                insertBytes(field.serialize());
            }

            serializeBytes(methodCount);
            for (auto& method : methods) {
                insertBytes((method.serialize()));
            }

            serializeBytes(attributesCount);
            for (auto& attribute : attributes) {
                insertBytes(attribute->serialize());
            }
        }

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
        std::vector<AttributeInfo*> attributes;
    };
}

#endif //_CLASSFILE_H
