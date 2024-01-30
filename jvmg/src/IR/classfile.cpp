//
// Created by Micky on 1/25/2024.
//

#include "jvmg/IR/classfile.h"

using namespace jvmg;

void ClassFile::FieldInfo::_serialize() {
    serializeBytes(accessFlags);
    serializeBytes(nameIndex);
    serializeBytes(descriptorIndex);
    serializeBytes(attributesCount);

    for (auto& attribute : attributes) {
        insertBytes(attribute->serialize());
    }
}

void ClassFile::MethodInfo::_serialize() {
    serializeBytes(accessFlags);
    serializeBytes(nameIndex);
    serializeBytes(descriptorIndex);
    serializeBytes(attributesCount);
    for (auto& attributeInfo : attributes) {
        insertBytes(attributeInfo->serialize());
    }
}

void ClassFile::_serialize() {
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

    serializeBytes(methodsCount);
    for (auto& method : methods) {
        insertBytes((method.serialize()));
    }

    serializeBytes(attributesCount);
    for (auto& attribute : attributes) {
        insertBytes(attribute->serialize());
    }
}