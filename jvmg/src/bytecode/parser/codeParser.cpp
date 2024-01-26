#include "jvmg/bytecode/parser/parser.h"

using namespace jvmg;

Instruction::Opcode Instruction::getOpcodeFromOpcodeByte(std::uint16_t opcodeByte) {
    if (opcodeByte == 0x0) {
        return Instruction::NOP;
    } else if (0x01 <= opcodeByte && opcodeByte <= 0x0F) {
        return Instruction::CONST;
    } else if (opcodeByte == 0x10) {
        return Instruction::BI_PUSH;
    } else if (opcodeByte == 0x11) {
        return Instruction::SI_PUSH;
    } else if (0x12 <= opcodeByte && opcodeByte <= 0x14) {
        return Instruction::LDC;
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
    } else if (0x70 <= opcodeByte && opcodeByte <= 0x73) {
        return Instruction::REM;
    } else if (0x74 <= opcodeByte && opcodeByte <= 0x77) {
        return Instruction::NEG;
    } else if (0x78 <= opcodeByte && opcodeByte <= 0x79) {
        return Instruction::SHL;
    } else if (0x7A <= opcodeByte && opcodeByte <= 0x7D) {
        return Instruction::SHR;
    } else if (0x7E <= opcodeByte && opcodeByte <= 0x7F) {
        return Instruction::AND;
    } else if (0x80 <= opcodeByte && opcodeByte <= 0x81) {
        return Instruction::OR;
    } else if (0x82 <= opcodeByte && opcodeByte <= 0x83) {
        return Instruction::XOR;
    } else if (opcodeByte == 0x84) {
        return Instruction::IINC;
    } else if (0x85 <= opcodeByte && opcodeByte <= 0x93) {
        return Instruction::CONVERT;
    } else if (0x94 <= opcodeByte && opcodeByte <= 0x98) {
        return Instruction::CMP;
    } else if (0x99 <= opcodeByte && opcodeByte <= 0x9E) {
        return Instruction::IF_BRANCH;
    } else if (0x9F <= opcodeByte && opcodeByte <= 0xA6) {
        return Instruction::IF_CMP_BRANCH;
    } else if (opcodeByte == 0xA7) {
        return Instruction::GOTO;
    } else if (opcodeByte == 0xA8) {
        return Instruction::JSR;
    } else if (opcodeByte == 0xA9) {
        return Instruction::RET;
    } else if (opcodeByte == 0xAA) {
        return Instruction::TABLE_SWITCH;
    } else if (opcodeByte == 0xAB) {
        return Instruction::LOOKUP_SWITCH;
    } else if (0xAC <= opcodeByte && opcodeByte <= 0xB1) {
        return Instruction::RETURN;
    } else if (opcodeByte == 0xB2) {
        return Instruction::GET_STATIC;
    } else if (opcodeByte == 0xB3) {
        return Instruction::GET_STATIC;
    } else if (opcodeByte == 0xB4) {
        return Instruction::GET_FIELD;
    } else if (opcodeByte == 0xB5) {
        return Instruction::PUT_FIELD;
    } else if (opcodeByte == 0xB6) {
        return Instruction::INVOKE_VIRTUAL;
    } else if (opcodeByte == 0xB7) {
        return Instruction::INVOKE_SPECIAL;
    } else if (opcodeByte == 0xB8) {
        return Instruction::INVOKE_STATIC;
    } else if (opcodeByte == 0xB9) {
        return Instruction::INVOKE_INTERFACE;
    } else if (opcodeByte == 0xBA) {
        return Instruction::INVOKE_DYNAMIC;
    } else if (opcodeByte == 0xBB) {
        return Instruction::NEW;
    } else if (0xBC <= opcodeByte && opcodeByte <= 0xBD) {
        return Instruction::NEW_ARRAY;
    } else if (opcodeByte == 0xBE) {
        return Instruction::ARRAY_LENGTH;
    } else if (opcodeByte == 0xBF) {
        return Instruction::ATHROW;
    } else if (opcodeByte == 0xC0) {
        return Instruction::CHECK_CAST;
    } else if (opcodeByte == 0xC1) {
        return Instruction::INSTANCE_OF;
    } else if (opcodeByte == 0xC2) {
        return Instruction::MONITOR_ENTER;
    } else if (opcodeByte == 0xC3) {
        return Instruction::MONITOR_EXIT;
    } else if (opcodeByte == 0xC4) {
        return Instruction::WIDE;
    } else if (opcodeByte == 0xC5) {
        return Instruction::MULTI_ANEW_ARRAY;
    } else if (opcodeByte == 0xC6) {
        return Instruction::IF_NULL;
    } else if (opcodeByte == 0xC7) {
        return Instruction::IF_NO_NULL;
    } else if (opcodeByte == 0xC8) {
        return Instruction::GOTO_W;
    } else if (opcodeByte == 0xC9) {
        return Instruction::JSR_W;
    } else if (opcodeByte == 0xCA) {
        return Instruction::BREAK_POINT;
    } else if (opcodeByte == 0xFE) {
        return Instruction::IMP_DEP_1;
    } else if (opcodeByte == 0xFF) {
        return Instruction::IMP_DEP_2;
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
        case Instruction::BI_PUSH: {
            auto operand = consumeOneByte();
            return BiPushInst(opcodeByte, operand);
        }
        case Instruction::SI_PUSH: {
            auto operand1 = consumeOneByte();
            auto operand2 = consumeOneByte();
            return SiPushInst(opcodeByte, operand1, operand2);
        }
        case Instruction::LOAD: {
            return LoadInst(opcodeByte);
        }
        case Instruction::STORE: {
            return StoreInst(opcodeByte);
        }
        case Instruction::RETURN: {
            return ReturnInst(opcodeByte);
        }
        case Instruction::GET_FIELD: {
            auto operand1 = consumeOneByte();
            auto operand2 = consumeOneByte();
            return GetFieldInst(opcodeByte, operand1, operand2);
        }
        case Instruction::PUT_FIELD: {
            auto operand1 = consumeOneByte();
            auto operand2 = consumeOneByte();
            return PutFieldInst(opcodeByte, operand1, operand2);
        }
        case Instruction::INVOKE_SPECIAL: {
            auto operand1 = consumeOneByte();
            auto operand2 = consumeOneByte();
            return InvokeSpecialInst(opcodeByte, operand1, operand2);
        }
        case Instruction::INVALID_INSTRUCTION_OPCODE:
        default:
            throw std::invalid_argument("Opcode not implemented: " + std::to_string(opcodeByte));
    }
}