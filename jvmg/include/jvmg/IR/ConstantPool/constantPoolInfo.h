//
// Created by Micky on 1/22/2024.
//

#ifndef _CONSTANT_POOL_INFO_H
#define _CONSTANT_POOL_INFO_H

#include "jvmg/IR/attribute.h"

#include <cstdint>
#include <utility>
#include <vector>

namespace jvmg {
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

        explicit CPInfo(ConstantType tag) : tag(tag) {}
        CPInfo(ConstantType tag, std::vector<std::uint8_t> info) : tag(tag), info(std::move(info)) {}

        ConstUTF8Info *asUTF8Info() { return tag == CONSTANT_Utf8 ? (ConstUTF8Info*)this : nullptr; }

        ConstantType tag;
        std::vector<std::uint8_t> info;

    private:
        void _serialize() override;
    };

    class ConstClassInfo : public CPInfo {
    public:
        explicit ConstClassInfo(std::uint16_t nameIndex) : CPInfo(CONSTANT_Class) {
            info.push_back((nameIndex & 0xFF00) >> 8);
            info.push_back(nameIndex & 0xFF);
        }
    };

    class ConstFieldRefInfo : public CPInfo {
    public:
        ConstFieldRefInfo(std::uint16_t classIndex, std::uint16_t nameAndTypeIndex) : CPInfo(CONSTANT_Fieldref) {
            info.push_back((classIndex & 0xFF00) >> 8);
            info.push_back(classIndex & 0xFF);
            info.push_back((nameAndTypeIndex & 0xFF00) >> 8);
            info.push_back(nameAndTypeIndex & 0xFF);
        }
    };

    class ConstMethodRefInfo : public CPInfo {
    public:
        ConstMethodRefInfo(std::uint16_t classIndex, std::uint16_t nameAndTypeIndex) : CPInfo(CONSTANT_Methodref) {
            info.push_back((classIndex & 0xFF00) >> 8);
            info.push_back(classIndex & 0xFF);
            info.push_back((nameAndTypeIndex & 0xFF00) >> 8);
            info.push_back(nameAndTypeIndex & 0xFF);
        }
    };

    class ConstInterfaceMethodRefInfo : public CPInfo {
    public:
        ConstInterfaceMethodRefInfo(std::uint16_t classIndex, std::uint16_t nameAndTypeIndex) : CPInfo(CONSTANT_InterfaceMethodref) {
            info.push_back((classIndex & 0xFF00) >> 8);
            info.push_back(classIndex & 0xFF);
            info.push_back((nameAndTypeIndex & 0xFF00) >> 8);
            info.push_back(nameAndTypeIndex & 0xFF);
        }
    };

    class ConstStringInfo : public CPInfo {
    public:
        explicit ConstStringInfo(std::uint16_t stringIndex) : CPInfo(CONSTANT_String) {
            info.push_back((stringIndex & 0xFF00) >> 8);
            info.push_back(stringIndex & 0xFF);
        }
    };

    class ConstIntegerInfo : public CPInfo {
    public:
        explicit ConstIntegerInfo(std::uint32_t bytes) : CPInfo(CONSTANT_Integer) {
            info.push_back((bytes & 0xFF000000) >> 24);
            info.push_back((bytes & 0xFF0000) >> 16);
            info.push_back((bytes & 0xFF00) >> 8);
            info.push_back(bytes & 0xFF);
        }
    };

    class ConstFloatInfo : public CPInfo {
    public:
        explicit ConstFloatInfo(std::uint32_t bytes) : CPInfo(CONSTANT_Float) {
            info.push_back((bytes & 0xFF000000) >> 24);
            info.push_back((bytes & 0xFF0000) >> 16);
            info.push_back((bytes & 0xFF00) >> 8);
            info.push_back(bytes & 0xFF);
        }
    };

    class ConstLongInfo : public CPInfo {
    public:
        ConstLongInfo(std::uint32_t highBytes, std::uint32_t lowBytes) : CPInfo(CONSTANT_Long) {
            info.push_back((highBytes & 0xFF000000) >> 24);
            info.push_back((highBytes & 0xFF0000) >> 16);
            info.push_back((highBytes & 0xFF00) >> 8);
            info.push_back(highBytes & 0xFF);

            info.push_back((lowBytes & 0xFF000000) >> 24);
            info.push_back((lowBytes & 0xFF0000) >> 16);
            info.push_back((lowBytes & 0xFF00) >> 8);
            info.push_back(lowBytes & 0xFF);
        }
    };

    class ConstDoubleInfo : public CPInfo {
    public:
        ConstDoubleInfo(std::uint32_t highBytes, std::uint32_t lowBytes) : CPInfo(CONSTANT_Double) {
            info.push_back((highBytes & 0xFF000000) >> 24);
            info.push_back((highBytes & 0xFF0000) >> 16);
            info.push_back((highBytes & 0xFF00) >> 8);
            info.push_back(highBytes & 0xFF);

            info.push_back((lowBytes & 0xFF000000) >> 24);
            info.push_back((lowBytes & 0xFF0000) >> 16);
            info.push_back((lowBytes & 0xFF00) >> 8);
            info.push_back(lowBytes & 0xFF);
        }
    };

    class ConstNameAndType : public CPInfo {
    public:
        ConstNameAndType(std::uint16_t nameIndex, std::uint16_t descriptorIndex) : CPInfo(CONSTANT_NameAndType) {
            info.push_back((nameIndex & 0xFF00) >> 8);
            info.push_back(nameIndex & 0xFF);
            info.push_back((descriptorIndex & 0xFF00) >> 8);
            info.push_back(descriptorIndex & 0xFF);
        }
    };

    class ConstUTF8Info : public CPInfo {
    public:
        ConstUTF8Info(std::uint16_t length, std::vector<std::uint8_t> bytes) : CPInfo(CONSTANT_Utf8) {
            info.push_back((length & 0xFF00) >> 8);
            info.push_back(length & 0xFF);
            info.insert(info.end(), bytes.begin(), bytes.end());
        }
        explicit ConstUTF8Info(const std::string& str) : CPInfo(CONSTANT_Utf8) {
            auto length = str.length();
            info.push_back((length & 0xFF00) >> 8);
            info.push_back(length & 0xFF);
            for (auto c : str) {
                info.push_back(c);
            }
        }
        std::uint16_t getLength();
        std::uint8_t getByte(int idx);
    };

    class ConstMethodHandleInfo : public CPInfo {
    public:
        explicit ConstMethodHandleInfo(std::uint8_t referenceKind, std::uint16_t referenceIndex) : CPInfo(CONSTANT_MethodHandle) {
            info.push_back(referenceKind);
            info.push_back((referenceIndex & 0xFF00) >> 8);
            info.push_back(referenceIndex & 0xFF);
        }
    };

    class ConstMethodTypeInfo : public CPInfo {
    public:
        explicit ConstMethodTypeInfo(std::uint16_t descriptorIndex) : CPInfo(CONSTANT_MethodType) {
            info.push_back((descriptorIndex & 0xFF00) >> 8);
            info.push_back(descriptorIndex & 0xFF);
        }
    };

    class ConstInvokeDynamicInfo : public CPInfo {
    public:
        explicit ConstInvokeDynamicInfo(std::uint16_t bootstrapMethodAttrIndex, std::uint16_t nameAndTypeIndex) : CPInfo(CONSTANT_InvokeDynamic) {
            info.push_back((bootstrapMethodAttrIndex & 0xFF00) >> 8);
            info.push_back(bootstrapMethodAttrIndex & 0xFF);
            info.push_back((nameAndTypeIndex & 0xFF00) >> 8);
            info.push_back(nameAndTypeIndex & 0xFF);
        }
    };
}

#endif //_CONSTANT_POOL_INFO_H
