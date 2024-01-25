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

        explicit Instruction(Opcode opcode) : opcode(opcode), type(NoTy), sizeInBytes(1) {}

        Instruction(Opcode opcode, std::uint8_t operand1)
            : opcode(opcode), operand1(operand1), sizeInBytes(2) {}

        Instruction(Opcode opcode, std::uint8_t operand1, std::uint8_t operand2)
                : opcode(opcode), operand1(operand1), operand2(operand2), sizeInBytes(3) {}

        static Opcode getOpcodeFromOpcodeByte(std::uint16_t opcodeByte);

        int getSizeInBytes() { return sizeInBytes; }
    protected:
        Opcode opcode;
        Type type;
        int sizeInBytes;
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

    class ReturnInst : public Instruction {
    public:
        explicit ReturnInst(std::uint16_t opcodeByte) : Instruction(RETURN) {
            switch (opcodeByte) {
                case 0xAC:{
                    type = IntTy;
                }
                case 0xAD:{
                    type = LongTy;
                }
                case 0xAE:{
                    type = FloatTy;
                }
                case 0xAF:{
                    type = DoubleTy;
                }
                case 0xB0:{
                    type = ReferenceTy;
                }
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
