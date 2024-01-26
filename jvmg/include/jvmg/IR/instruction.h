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
        void _serialize() override;

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
        explicit ConstInst(std::uint8_t opcodeByte);
    };

    class LoadInst : public Instruction {
    public:
        explicit LoadInst(std::uint16_t opcodeByte);
    };

    class StoreInst : public Instruction {
    public:
        explicit StoreInst(std::uint16_t opcodeByte);
    };

    class BiPushInst : public Instruction {
    public:
        explicit BiPushInst(std::uint8_t opcodeByte, std::uint8_t operand);
    };

    class SiPushInst : public Instruction {
    public:
        SiPushInst(std::uint8_t opcodeByte, std::uint8_t operand1, std::uint8_t operand2);
    };

    class ReturnInst : public Instruction {
    public:
        explicit ReturnInst(std::uint16_t opcodeByte);
    };

    class IndexInstruction: public Instruction {
    public:
        IndexInstruction(std::uint8_t opcodeByte, std::uint8_t indexByte1, std::uint8_t indexByte2);

        void setIndex(std::uint16_t index) {
            this->index = index;
            operand1 = (index & 0xFF00) >> 8;
            operand2 = index & 0xFF;
        }

        std::uint16_t getIndex() { return index; }

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
