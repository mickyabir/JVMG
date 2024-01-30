//
// Created by Micky on 1/29/2024.
//

#include "jvmg/IR/ConstantPool/constantPoolInfo.h"

using namespace jvmg;

void CPInfo::_serialize() {
    serializeBytes(tag);
    for (auto& byte : info) {
        serializeBytes(byte);
    }
}
