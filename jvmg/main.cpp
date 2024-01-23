#include "jvmg/IR/instruction.h"
#include "jvmg/IR/operand.h"
#include "jvmg/IR/type.h"
#include "jvmg/IR/value.h"

using namespace jvmg;

int main() {
    auto shortType = IntegerType(IntegerType::ShortTyID);
    Value::Data val = {16};
    auto shortVal = Value(val);

    operand op = operand(&shortType, &shortVal);
    IPushInst pushInst = IPushInst(&shortType, &op);

    enum Test {
        x = 5,
        y = 8
    };

    std::cout << Test::x << std::endl;

    return 0;
}
