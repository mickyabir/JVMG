#include "jvmg/IR/instruction.h"

using namespace jvmg;

IPushInst::IPushInst(Type *ty, operand *op) : Instruction(ty, IPUSH, op) {
    auto *intTy = dynamic_cast<IntegerType*>(ty);
    assert(intTy != nullptr && (intTy->isByte() || intTy->isShort()));
}