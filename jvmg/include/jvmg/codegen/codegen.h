//
// Created by Micky on 1/25/2024.
//

#ifndef _CODEGEN_H
#define _CODEGEN_H

#include "jvmg/IR/classfile.h"

#include <sstream>

using namespace jvmg;

std::uint16_t reverseTwoByteOrder(std::uint16_t bytes) {
    return ((bytes & 0xFF) << 8) | ((bytes & 0xFF00) >> 8);
}

std::uint32_t reverseFourByteOrder(std::uint32_t bytes) {
    return ((bytes & 0xFF) << 24) | ((bytes & 0xFF00) << 8) | ((bytes & 0xFF0000) >> 8) | ((bytes & 0xFF000000) >> 24);
}

void outputBytes(std::ofstream &outputStream, std::uint8_t bytes) {
    outputStream.write((char *) &bytes, sizeof(std::uint8_t));
}

void outputBytes(std::ofstream &outputStream, std::uint16_t bytes) {
    auto reversedBytes = reverseTwoByteOrder(bytes);
    outputStream.write((char *) &reversedBytes, sizeof(std::uint16_t));
}

void outputBytes(std::ofstream &outputStream, std::uint32_t bytes) {
    auto reversedBytes = reverseFourByteOrder(bytes);
    outputStream.write((char *) &reversedBytes, sizeof(std::uint32_t));
}

void outputInfo(std::ofstream &outputStream, const Info *info) {

}

void outputAttribute(std::ofstream &outputStream, const AttributeInfo *attribute) {
    outputBytes(outputStream, attribute->attributeNameIndex);
    outputBytes(outputStream, attribute->attributeNameIndex);
}

void outputCPInfo(std::ofstream &outputStream, const ClassFile::CPInfo &cpInfo) {
    outputBytes(outputStream, cpInfo.tag);
    for (auto &infoByte : cpInfo.info) {
        outputBytes(outputStream, infoByte);
    }
}

void outputFieldInfo(std::ofstream &outputStream, const ClassFile::FieldInfo &fieldInfo) {
    outputBytes(outputStream, fieldInfo.accessFlags);
    outputBytes(outputStream, fieldInfo.nameIndex);
    outputBytes(outputStream, fieldInfo.descriptorIndex);
    outputBytes(outputStream, fieldInfo.attributesCount);
    for (auto &attribute : fieldInfo.attributes) {
        outputAttribute(outputStream, attribute);
    }
}

void outputBinaryFromClassfile(ClassFile &classFile, const std::string& filename) {
    std::ofstream outputStream(filename, std::ios::out | std::ios::binary);

    std::vector<std::uint8_t> bytes = classFile.serialize();

    for (auto byte : bytes) {
        outputStream.write((char *) &byte, sizeof(std::uint8_t));
    }
}

#endif //_CODEGEN_H
