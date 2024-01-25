//
// Created by Micky on 1/21/2024.
//

#ifndef _INSTRUCTION_H
#define _INSTRUCTION_H

#include <cassert>
#include <utility>
#include <iostream>
#include <vector>
#include <map>
#include <cstdint>
#include <optional>

namespace jvmg {
    class Instruction {
    public:
        enum Opcode {
            NOP,
            CONST,
            BI_PUSH,
            SI_PUSH,
            LOAD,
            STORE,
            POP,
            DUP,
            SWAP,
            ADD,
            SUB,
            MUL,
            DIV,
            RETURN,
            GET_FIELD,
            PUT_FIELD,
            INVOKE_SPECIAL,
            INVALID_INSTRUCTION_OPCODE
        };

        enum Type {
            ByteTy,
            ShortTy,
            IntTy,
            LongTy,
            CharTy,
            FloatTy,
            DoubleTy,
            ReferenceTy,
            NoTy
        };

        enum ImplicitValue {
            NULL_VAL,
            M1,
            ZERO,
            ONE,
            TWO,
            THREE,
            FOUR,
            FIVE
        };

        explicit Instruction(Opcode opcode) : opcode(opcode), type(NoTy) {}

        Instruction(Opcode opcode, std::uint8_t operand1)
            : opcode(opcode), operand1(operand1), type(NoTy) {}

        Instruction(Opcode opcode, std::uint8_t operand1, std::uint8_t operand2)
                : opcode(opcode), operand1(operand1), operand2(operand2), type(NoTy) {}

        static Opcode getOpcodeFromOpcodeByte(std::uint16_t opcodeByte);

        int getSizeInBytes() { return 1 + (operand1.has_value() ? 1 : 0) + (operand2.has_value() ? 1 : 0); }
    protected:
        Opcode opcode;
        Type type;
        std::optional<std::uint8_t> operand1;
        std::optional<std::uint8_t> operand2;
        std::optional<ImplicitValue> value;
    };

    class ConstInst : public Instruction {
    public:
        explicit ConstInst(std::uint8_t opcodeByte) : Instruction(CONST) {
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
    };

    class LoadInst : public Instruction {
    public:
        LoadInst(std::uint16_t opcodeByte) : Instruction(LOAD) {
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
    };

    class StoreInst : public Instruction {
    public:
        StoreInst(std::uint16_t opcodeByte) : Instruction(STORE) {
            switch (opcodeByte) {
                case 0x3C: {
                    type = IntTy;
                    break;
                }
                default:
                    throw std::invalid_argument("Invalid store instruction opcode: " + std::to_string(opcodeByte));
            }
        }
    };


    class BiPushInst : public Instruction {
    public:
        explicit BiPushInst(std::uint8_t operand) : Instruction(BI_PUSH, operand) {
            type = ByteTy;
        }
    };
    class SiPushInst : public Instruction {
    public:
        SiPushInst(std::uint8_t operand1, std::uint8_t operand2) : Instruction(SI_PUSH, operand1, operand2) {
            type = ShortTy;
        }
    };

    class ReturnInst : public Instruction {
    public:
        explicit ReturnInst(std::uint16_t opcodeByte) : Instruction(RETURN) {
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
    };

    class IndexInstruction: public Instruction {
    public:
        IndexInstruction(Opcode opcode, std::uint8_t indexByte1, std::uint8_t indexByte2) : Instruction(opcode, indexByte1, indexByte2) {
            index = (indexByte1 << 8) | indexByte2;
        }

        void setIndex(std::uint16_t index) {
            this->index = index;
            operand1 = (index & 0xFF00) >> 8;
            operand2 = index & 0xFF;
        }

        std::uint16_t getIndex() {
            return index;
        }
    private:
        std::uint16_t index;
    };

    class GetFieldInst: public IndexInstruction {
    public:
        GetFieldInst(std::uint8_t indexByte1, std::uint8_t indexByte2) : IndexInstruction(GET_FIELD, indexByte1, indexByte2) {}
    };

    class PutFieldInst: public IndexInstruction {
    public:
        PutFieldInst(std::uint8_t indexByte1, std::uint8_t indexByte2) : IndexInstruction(PUT_FIELD, indexByte1, indexByte2) {}
    };

    class InvokeSpecialInst : public IndexInstruction {
    public:
        InvokeSpecialInst( std::uint8_t indexByte1, std::uint8_t indexByte2) : IndexInstruction(INVOKE_SPECIAL, indexByte1, indexByte2) {}
    };
}

#endif //_INSTRUCTION_H
