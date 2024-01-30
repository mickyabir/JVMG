#include "jvmg/bytecode/reader.h"
#include "jvmg/IR/instruction.h"
#include "jvmg/bytecode/parser/parser.h"

using namespace jvmg;

int main() {
    Reader reader = Reader("test/data/classFiles/Minimum.class");

    Parser parser = Parser(&reader);
    auto classFile = parser.consumeClassFile();
    classFile.outputToFile("test/data/classFiles/Switch.class");


    std::uint16_t minorVersion = 0;
    std::uint16_t majorVersion = 65;

    std::uint16_t constantPoolCount = 13;
    std::vector<CPInfo> constantPool;

    constantPool.push_back(ConstMethodRefInfo(0x0002, 0x0003));
    constantPool.push_back(ConstClassInfo(0x0004));
    constantPool.push_back(ConstNameAndType(0x0005, 0x0006));
    constantPool.push_back(ConstUTF8Info("java/lang/Object"));
    constantPool.push_back(ConstUTF8Info("<init>"));
    constantPool.push_back(ConstUTF8Info("()V"));
    constantPool.push_back(ConstClassInfo(0x0008));
    constantPool.push_back(ConstUTF8Info("Minimum"));
    constantPool.push_back(ConstUTF8Info("Code"));
    constantPool.push_back(ConstUTF8Info("LineNumberTable"));
    constantPool.push_back(ConstUTF8Info("SourceFile"));
    constantPool.push_back(ConstUTF8Info("Minimum.java"));

    std::uint16_t accessFlags = ClassFile::ACC_PUBLIC | ClassFile::ACC_SUPER;
    std::uint16_t thisClass = 0x0007;
    std::uint16_t superClass = 0x0002;
    std::uint16_t interfaceCount = 0;
    std::vector<std::uint16_t> interfaces;
    std::uint16_t fieldsCount = 0;
    std::vector<ClassFile::FieldInfo> fields;

    std::uint16_t methodsCount = 0;
    std::vector<ClassFile::MethodInfo> methods;

    std::uint16_t methodAccessFlags = 0x0001;
    std::uint16_t methodNameIndex = 0x0005;
    std::uint16_t methodDescriptorIndex = 0x0006;
    std::uint16_t methodAttributesCount = 0x0001;
    std::vector<Instruction> code = {
            ALoad0,
            InvokeSpecial(0x0001),
            Return
    };
    std::vector<LineNumberAttribute::LineNumberTableEntry> lineNumberTable = {
            {0, 1}
    };
    auto lineNumberTableAttribute = LineNumberAttribute(1, lineNumberTable);

    auto lineNumberTableAttributeInfo = new AttributeInfo(0x000A, 0x0006, &lineNumberTableAttribute);
    lineNumberTableAttributeInfo->setAttributeName("LineNumberTable");
    auto codeAttribute = new CodeAttribute(1, 1, 5, code, 0, {}, 1, {lineNumberTableAttributeInfo});
    auto methodAttribute = new AttributeInfo(0x0009, 0x001D, codeAttribute);
    methodAttribute->setAttributeName("Code");
    auto method = ClassFile::MethodInfo(methodAccessFlags, methodNameIndex, methodDescriptorIndex, methodAttributesCount, {methodAttribute});
    methods.push_back(method);


    auto sourceFileAttribute = SourceFileAttribute(0x000C, "SourceFile");
    auto classAttribute = AttributeInfo(0x000B, 0x0002, &sourceFileAttribute);

    auto attributesCount = 1;
    std::vector<AttributeInfo *> attributes = {&classAttribute};
    auto newClassFile = new ClassFile(minorVersion, majorVersion, constantPoolCount, constantPool, accessFlags, thisClass, superClass, interfaceCount, interfaces, fieldsCount, fields, methodsCount, methods, attributesCount, attributes);
    newClassFile->outputToFile("test/data/classFiles/Handwritten.class");

    auto methodDeleted = newClassFile->getMethods()[0];
    delete newClassFile;

    return 0;
}
