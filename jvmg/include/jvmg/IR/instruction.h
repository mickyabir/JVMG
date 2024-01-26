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
#include "jvmg/util/util.h"

namespace jvmg {
    class Instruction : public Serializable {
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

        explicit Instruction(std::uint8_t opcodeByte) : opcodeByte(opcodeByte), type(NoTy), opcode(getOpcodeFromOpcodeByte(opcodeByte)) {}

        Instruction(std::uint8_t opcodeByte, std::uint8_t operand1)
            : opcodeByte(opcodeByte), operand1(operand1), type(NoTy), opcode(getOpcodeFromOpcodeByte(opcodeByte)) {}

        Instruction(std::uint8_t opcodeByte, std::uint8_t operand1, std::uint8_t operand2)
                : opcodeByte(opcodeByte), operand1(operand1), operand2(operand2), type(NoTy), opcode(getOpcodeFromOpcodeByte(opcodeByte)){}

        static Opcode getOpcodeFromOpcodeByte(std::uint16_t opcodeByte);

        int getSizeInBytes() { return 1 + (operand1.has_value() ? 1 : 0) + (operand2.has_value() ? 1 : 0); }

    private:
        void _serialize() override {
        }
        std::uint8_t opcodeByte;
    protected:
        Opcode opcode;
        Type type;
        std::optional<std::uint8_t> operand1;
        std::optional<std::uint8_t> operand2;
        std::optional<ImplicitValue> value;
    };

    class ConstInst : public Instruction {
    public:
        explicit ConstInst(std::uint8_t opcodeByte) : Instruction(opcodeByte) {
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
        LoadInst(std::uint16_t opcodeByte) : Instruction(opcodeByte) {
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
        StoreInst(std::uint16_t opcodeByte) : Instruction(opcodeByte) {
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
        explicit BiPushInst(std::uint8_t opcodeByte, std::uint8_t operand) : Instruction(opcodeByte, operand) {
            type = ByteTy;
        }
    };
    class SiPushInst : public Instruction {
    public:
        SiPushInst(std::uint8_t opcodeByte, std::uint8_t operand1, std::uint8_t operand2) : Instruction(opcodeByte, operand1, operand2) {
            type = ShortTy;
        }
    };

    class ReturnInst : public Instruction {
    public:
        explicit ReturnInst(std::uint16_t opcodeByte) : Instruction(opcodeByte) {
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
        IndexInstruction(std::uint8_t opcodeByte, std::uint8_t indexByte1, std::uint8_t indexByte2) : Instruction(opcodeByte, indexByte1, indexByte2) {
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
        GetFieldInst(std::uint8_t opcodeByte, std::uint8_t indexByte1, std::uint8_t indexByte2) : IndexInstruction(opcodeByte, indexByte1, indexByte2) {}
    };

    class PutFieldInst: public IndexInstruction {
    public:
        PutFieldInst(std::uint8_t opcodeByte, std::uint8_t indexByte1, std::uint8_t indexByte2) : IndexInstruction(opcodeByte, indexByte1, indexByte2) {}
    };

    class InvokeSpecialInst : public IndexInstruction {
    public:
        InvokeSpecialInst(std::uint8_t opcodeByte, std::uint8_t indexByte1, std::uint8_t indexByte2) : IndexInstruction(opcodeByte, indexByte1, indexByte2) {}
    };
}

#endif //_INSTRUCTION_H
