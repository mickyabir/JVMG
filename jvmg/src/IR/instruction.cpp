#include "jvmg/IR/instruction.h"

using namespace jvmg;

void Instruction::_serialize() {
    serializeBytes(opcodeByte);
    if (operand1.has_value()) {
        serializeBytes(operand1.value());

        // Nested because operand2 should not have a value if operand1 does not
        if (operand2.has_value()) {
            serializeBytes(operand2.value());
        }
    }
}

ConstInst::ConstInst(std::uint8_t opcodeByte) : Instruction(opcodeByte) {
    switch (opcodeByte) {
        case 0x01: {
            type = ReferenceTy;
            value = NULL_VAL;
            break;
        }
        case 0x02: {
            type = IntTy;
            value = M1;
            break;
        }
        case 0x03: {
            type = IntTy;
            value = ZERO;
            break;
        }
        case 0x04: {
            type = IntTy;
            value = ONE;
            break;
        }
        case 0x05: {
            type = IntTy;
            value = TWO;
            break;
        }
        case 0x06: {
            type = IntTy;
            value = THREE;
            break;
        }
        case 0x07: {
            type = IntTy;
            value = FOUR;
            break;
        }
        case 0x08: {
            type = IntTy;
            value = FIVE;
            break;
        }
        default:
            throw std::invalid_argument("Invalid const instruction opcode: " + std::to_string(opcodeByte));
    }
}

LoadInst::LoadInst(std::uint16_t opcodeByte) : Instruction(opcodeByte) {
    switch (opcodeByte) {
        case 0x2A: {
            type = ReferenceTy;
            value = ZERO;
            break;
        }
        default:
            throw std::invalid_argument("Invalid load instruction opcode: " + std::to_string(opcodeByte));
    }
}

StoreInst::StoreInst(std::uint16_t opcodeByte) : Instruction(opcodeByte) {
    switch (opcodeByte) {
        case 0x3C: {
            type = IntTy;
            break;
        }
        default:
            throw std::invalid_argument("Invalid store instruction opcode: " + std::to_string(opcodeByte));
    }
}

BiPushInst::BiPushInst(std::uint8_t opcodeByte, std::uint8_t operand) : Instruction(opcodeByte, operand) {
    type = ByteTy;
}

SiPushInst::SiPushInst(std::uint8_t opcodeByte, std::uint8_t operand1, std::uint8_t operand2) : Instruction(opcodeByte, operand1, operand2) {
    type = ShortTy;
}

ReturnInst::ReturnInst(std::uint16_t opcodeByte) : Instruction(opcodeByte) {
    switch (opcodeByte) {
        case 0xAC: {
            type = IntTy;
            break;
        }
        case 0xAD: {
            type = LongTy;
            break;
        }
        case 0xAE: {
            type = FloatTy;
            break;
        }
        case 0xAF: {
            type = DoubleTy;
            break;
        }
        case 0xB0: {
            type = ReferenceTy;
            break;
        }
        case 0xB1: {
            break;
        }
        default:
            throw std::invalid_argument("Invalid return instruction opcode: " + std::to_string(opcodeByte));
    }
}

IndexInstruction::IndexInstruction(std::uint8_t opcodeByte, std::uint8_t indexByte1, std::uint8_t indexByte2) : Instruction(opcodeByte, indexByte1, indexByte2) {
    index = (indexByte1 << 8) | indexByte2;
}