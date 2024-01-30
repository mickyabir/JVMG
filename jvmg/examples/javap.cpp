//
// Created by Micky on 1/30/2024.
//
// This example program shows how to use this library to implement the javap -verbose program
// This is a set example targeting Minimum.class

#include "jvmg/bytecode/parser/parser.h"

using namespace jvmg;

int main() {
    std::string filename = "../tests/data/classFiles/Minimum.class";
    Reader reader = Reader(filename);
    Parser parser = Parser(&reader);

    auto classFile = parser.consumeClassFile();

    auto thisClassIndex = classFile.getThisClass();
    auto thisClassCPInfo = parser.getContext()->getConstant(thisClassIndex);
    auto thisClass = thisClassCPInfo.asConstClassInfo();
    auto thisClassNameIdx = thisClass->getIndex();

    auto thisClassName = parser.getContext()->getConstantUTF8(thisClassNameIdx);

    std::cout << "public class " << thisClassName << std::endl;
    std::cout << "  minor version: " << classFile.getMinorVersion() << std::endl;
    std::cout << "  major version: " << classFile.getMajorVersion() << std::endl;

    // Access flags
    auto accessFlags = classFile.getAccessFlags();
    std::cout << "  flags: (0x" << std::setfill('0') << std::setw(4) << std::hex << accessFlags << ") ";
    std::cout.copyfmt(std::ios(nullptr));

    if (accessFlags & ClassFile::ACC_PUBLIC) {
        std::cout << "ACC_PUBLIC, ";
    }
    if (accessFlags & ClassFile::ACC_SUPER) {
        std::cout << "ACC_SUPER";
    }

    std::cout << std::endl;

    std::cout << "  this_class: #" << thisClassIndex << std::endl;
    std::cout << "  super_class: #" << classFile.getSuperClass() << std::endl;

    // Counts
    std::cout << "  interfaces: " << classFile.getInterfaceCount() << ", ";
    std::cout << "fields: " << classFile.getFieldsCount() << ", ";
    std::cout << "methods: " << classFile.getMethodsCount() << ", ";
    std::cout << "attributes: " << classFile.getAttributesCount() << std::endl;

    // Constant pool
    std::cout << "Constant pool:" << std::endl;

    int constantPoolIdx = 1;
    for (auto constant : classFile.getConstantPool()) {
        if (constantPoolIdx < 10){
            std::cout << " ";
        }
        std::cout << "  #" << constantPoolIdx << " = ";
        if (constant.tag == jvmg::CPInfo::CONSTANT_Methodref) {
            std::cout << "Methodref         ";
            auto classIndex = (constant.info[0] << 8) | constant.info[1];
            auto nameAndTypeIndex = (constant.info[2] << 8) | constant.info[3];
            std::cout << "#" << classIndex << ".#" << nameAndTypeIndex;
        } else if (constant.tag == jvmg::CPInfo::CONSTANT_Class) {
            std::cout << "Class             ";
            auto nameIndex = (constant.info[0] << 8) | constant.info[1];
            std::cout << "#" << nameIndex;
        } else if (constant.tag == jvmg::CPInfo::CONSTANT_NameAndType) {
            std::cout << "NameAndType       ";
            auto nameIndex = (constant.info[0] << 8) | constant.info[1];
            auto typeIndex = (constant.info[2] << 8) | constant.info[3];
            std::cout << "#" << nameIndex << ":#" << typeIndex;
        } else if (constant.tag == jvmg::CPInfo::CONSTANT_Utf8) {
            std::cout << "Utf8              ";
            auto utf8Const = constant.asUTF8Info();
            auto length = utf8Const->getLength();
            for (auto i = 0; i < length; i++) {
                std::cout << utf8Const->getByte(i);
            }
        }
        std::cout << std::endl;
        constantPoolIdx++;
    }

    std::cout << "{" << std::endl;

    auto method = classFile.getMethods()[0];

    auto name = parser.getContext()->getConstantUTF8(method.nameIndex);
    auto descriptor = parser.getContext()->getConstantUTF8(method.descriptorIndex);
    if (name == "<init>" and descriptor == "()V" and (method.accessFlags & ClassFile::MethodInfo::ACC_PUBLIC)) {
        std::cout << "  public Minimum();" << std::endl;
    }

    std::cout << "    descriptor: " << descriptor << std::endl;

    auto flags = method.accessFlags;
    std::cout << "    flags: (0x" << std::setfill('0') << std::setw(4) << std::hex << method.accessFlags << ") ";
    std::cout.copyfmt(std::ios(nullptr));
    if (flags & ClassFile::MethodInfo::ACC_PUBLIC) {
        std::cout << "ACC_PUBLIC" << std::endl;
    }

    auto code = (CodeAttribute*) method.attributes[0]->info;
    std::cout << "    Code:" << std::endl;
    std::cout << "      stack=" << code->maxStack << ", locals=" << code->maxLocals << std::endl;

    for (auto instruction : code->code) {
        std::cout << "        ";
        if (instruction.getOpcodeByte() == 0x2A) {
            std::cout << "aload_0";
        } else if (instruction.getOpcodeByte() == 0xB7) {
            std::cout << "invokespecial #" << ((instruction.getOperands()[0] << 8) | instruction.getOperands()[1]);
        } else if (instruction.getOpcodeByte() == 0xB1) {
            std::cout << "return";
        }

        std::cout << std::endl;
    }

    std::cout << "      LineNumberTable:" << std::endl;
    auto lineNumberTable = ((LineNumberAttribute*) code->attributes[0]->info)->lineNumberTable;

    std::cout << "        line " << lineNumberTable[0].lineNumber << ": " << lineNumberTable[0].startPC << std::endl;

    std::cout << "}" << std::endl;

    auto sourceFile = (SourceFileAttribute*) classFile.getAttributes()[0]->info;
    auto sourceFileIndex = sourceFile->sourceFileIndex;
    auto sourceFileName = parser.getContext()->getConstantUTF8(sourceFileIndex);
    std::cout << "SourceFile: \"" << sourceFileName << "\"" << std::endl;

    return 0;
}