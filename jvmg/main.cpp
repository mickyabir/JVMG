#include "jvmg/bytecode/reader.h"
#include "jvmg/IR/instruction.h"
#include "jvmg/bytecode/parser/parser.h"

using namespace jvmg;

int main() {
    Reader reader = Reader("Main.class");

    Parser parser = Parser(&reader);
    auto classFile = parser.consumeClassFile();

    auto method1 = (CodeInfo*)classFile.getMethods()[0].attributes[0]->info;
    auto method2 = (CodeInfo*)classFile.getMethods()[1].attributes[0]->info;
    auto method3 = (CodeInfo*)classFile.getMethods()[2].attributes[0]->info;

    return 0;
}
