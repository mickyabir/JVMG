//
// Created by Micky on 1/29/2024.
//

#include "jvmg/IR/ConstantPool/constantPoolInfo.h"

using namespace jvmg;

std::uint16_t ConstUTF8Info::getLength() {
    return (info[0] << 8) | info[1];
}

std::uint8_t ConstUTF8Info::getByte(int idx) {
    return info[idx + 2];
}

