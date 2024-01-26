#include "jvmg/bytecode/reader.h"
#include "jvmg/IR/instruction.h"
#include "jvmg/bytecode/parser/parser.h"
#include "jvmg/codegen/codegen.h"

using namespace jvmg;

int main() {
    Reader reader = Reader("Main.class");

    Parser parser = Parser(&reader);
    auto classFile = parser.consumeClassFile();

    outputBinaryFromClassfile(classFile, "test.class");

    return 0;
}
