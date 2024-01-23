//
// Created by Micky on 1/21/2024.
//

#ifndef _INSTRUCTION_H
#define _INSTRUCTION_H

#include "jvmg/IR/value.h"
#include "jvmg/IR/operand.h"

#include <cassert>
#include <utility>
#include <iostream>

namespace jvmg {
    class Instruction {
    public:
        enum OpCode {
            IPUSH,
            CONST,
            LOAD,
            STORE,
            INC,
            ALOAD,
            ASTORE,
            ADD,
            SUB,
            MUL,
            DIV,
            REM,
            NEG,
            SHI,
            SHR,
            AND,
            OR,
            XOR,
            I2,
            L2,
            F2,
            D2,
            CMP,
            CMPL,
            CMPG,
            IF_CMP,
            RETURN
        };
        explicit Instruction(Type *ty, OpCode opcode, operand *op) : ty(ty), opcode(opcode), op(op) {}

        int getOffsetSize() { return op->getOffsetSize() + 1; }
    private:
        Type *ty;
        OpCode opcode;
        operand *op;
    };

    class IPushInst : Instruction {
    public:
        explicit IPushInst(Type *ty, operand *op);
    };
}

#endif //_INSTRUCTION_H
