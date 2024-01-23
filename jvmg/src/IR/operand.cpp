#include "jvmg/IR/operand.h"

using namespace jvmg;

operand::operand(PrimitiveType *ty, Value *value) : ty(ty), value(value) {
    this->ty = ty;
    this->value = value;

    auto intType = dynamic_cast<IntegerType*>(ty);
    auto floatType = dynamic_cast<FloatingPointType*>(ty);

    if ((intType != nullptr && intType->isLong()) || (floatType != nullptr && floatType->isDouble())) {
        units = TwoUnits;
    } else {
        units = OneUnit;
    }
}