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
            LOAD,
            STORE,
            POP,
            DUP,
            SWAP,
            ADD,
            SUB,
            MUL,
            INVOKE_SPECIAL,
            INVALID_INSTRUCTION_OPCODE
        };

        enum Type {
            ByteTy,
            ShortTy,
            IntTy,
            LongTy,
            CharTy,
            Float,
            Double,
            ReferenceTy
        };

        explicit Instruction(Opcode opcode) : opcode(opcode) {}

        Instruction(Opcode opcode, std::uint8_t operand1)
            : opcode(opcode), operand1(operand1) {}

        Instruction(Opcode opcode, std::uint8_t operand1, std::uint8_t operand2)
                : opcode(opcode), operand1(operand1), operand2(operand2) {}

        static Opcode getOpcodeFromOpcodeByte(std::uint16_t opcodeByte);
    protected:
        Opcode opcode;
        Type type;
        std::optional<int> value;
        std::optional<std::uint8_t> operand1;
        std::optional<std::uint8_t> operand2;
    };

    class ConstInst : public Instruction {
    public:
        enum Value {
            NULL_VAL,
            M1,
            ZERO,
            ONE,
            TWO,
            THREE,
            FOUR,
            FIVE
        };

        explicit ConstInst(std::uint8_t opcodeByte) : Instruction(CONST) {
            switch (opcodeByte) {
                case 0x01: {
                    type = ReferenceTy;
                    value = NULL_VAL;
                }
                case 0x02: {
                    type = IntTy;
                    value = M1;
                }
                case 0x03: {
                    type = IntTy;
                    value = ZERO;
                }
                case 0x04: {
                    type = IntTy;
                    value = ONE;
                }
                case 0x05: {
                    type = IntTy;
                    value = TWO;
                }
                case 0x06: {
                    type = IntTy;
                    value = THREE;
                }
                case 0x07: {
                    type = IntTy;
                    value = FOUR;
                }
                case 0x08: {
                    type = IntTy;
                    value = FIVE;
                }
            }
        }
    private:
        Value value;
    };

    class LoadInst : public Instruction {
    public:
        LoadInst(std::uint16_t opcodeByte) : Instruction(Instruction::Opcode::LOAD) {
            switch (opcodeByte) {
                case 0x2A: {
                    type = ReferenceTy;
                }
            }
        }
    };

    class InvokeSpecial : public Instruction {
    public:
        InvokeSpecial( std::uint8_t indexByte1, std::uint8_t indexByte2) : Instruction(Instruction::Opcode::INVOKE_SPECIAL, indexByte1, indexByte2) {
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
}

#endif //_INSTRUCTION_H
