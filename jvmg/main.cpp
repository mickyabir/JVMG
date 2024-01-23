#include "jvmg/bytecode/reader.h"
#include "jvmg/IR/instruction.h"
#include "jvmg/IR/operand.h"
#include "jvmg/IR/type.h"
#include "jvmg/IR/value.h"
#include "jvmg/bytecode/parser.h"

using namespace jvmg;

int main() {
    auto shortType = IntegerType(IntegerType::ShortTyID);
    Value::Data val = {16};
    auto shortVal = Value(val);

    operand op = operand(&shortType, &shortVal);
    IPushInst pushInst = IPushInst(&shortType, &op);

    Reader reader = Reader("Main.class");

    Parser parser = Parser(&reader);
    auto classFile = parser.consumeClassFile();

    return 0;
}
