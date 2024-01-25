#include "jvmg/bytecode/parser/parser.h"

using namespace jvmg;

Instruction::Opcode Instruction::getOpcodeFromOpcodeByte(std::uint16_t opcodeByte) {
    if (opcodeByte == 0x0) {
        return Instruction::NOP;
    } else if (0x01 <= opcodeByte && opcodeByte <= 0x0F) {
        return Instruction::CONST;
    } else if (0x15 <= opcodeByte && opcodeByte <= 0x35) {
        return Instruction::LOAD;
    } else if (0x36 <= opcodeByte && opcodeByte <= 0x56) {
        return Instruction::STORE;
    } else if (0x57 <= opcodeByte && opcodeByte <= 0x58) {
        return Instruction::POP;
    } else if (0x59 <= opcodeByte && opcodeByte <= 0x5E) {
        return Instruction::DUP;
    } else if (opcodeByte == 0x5F) {
        return Instruction::SWAP;
    } else if (0x60 <= opcodeByte && opcodeByte <= 0x63) {
        return Instruction::ADD;
    } else if (0x64 <= opcodeByte && opcodeByte <= 0x67) {
        return Instruction::SUB;
    } else if (0x68 <= opcodeByte && opcodeByte <= 0x6B) {
        return Instruction::MUL;
    } else if (0x6C <= opcodeByte && opcodeByte <= 0x6F) {
        return Instruction::DIV;
    } else if (0xAC <= opcodeByte && opcodeByte <= 0xB1) {
        return Instruction::RETURN;
    } else if (opcodeByte == 0xB4) {
        return Instruction::GET_FIELD;
    } else if (opcodeByte == 0xB5) {
        return Instruction::PUT_FIELD;
    } else if (opcodeByte == 0xB7) {
        return Instruction::INVOKE_SPECIAL;
    } else {
        return Instruction::INVALID_INSTRUCTION_OPCODE;
    }
}

Instruction Parser::consumeInstruction() {
    std::uint8_t opcodeByte = consumeOneByte();
    auto opcode = Instruction::getOpcodeFromOpcodeByte(opcodeByte);

    switch (opcode) {
        case Instruction::CONST: {
            return ConstInst(opcodeByte);
        }
        case Instruction::LOAD: {
            return LoadInst(opcodeByte);
        }
        case Instruction::RETURN: {
            return ReturnInst(opcodeByte);
        }
        case Instruction::GET_FIELD: {
            auto operand1 = consumeOneByte();
            auto operand2 = consumeOneByte();
            return GetFieldInst(operand1, operand2);
        }
        case Instruction::PUT_FIELD: {
            auto operand1 = consumeOneByte();
            auto operand2 = consumeOneByte();
            return PutFieldInst(operand1, operand2);
        }
        case Instruction::INVOKE_SPECIAL: {
            auto operand1 = consumeOneByte();
            auto operand2 = consumeOneByte();
            return InvokeSpecialInst(operand1, operand2);
        }
        case Instruction::INVALID_INSTRUCTION_OPCODE:
        default:
            throw std::invalid_argument("Opcode not implemented: " + std::to_string(opcodeByte));
    }
}