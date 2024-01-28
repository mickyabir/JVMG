//

// Created by Micky on 1/21/2024.
//

#ifndef _INSTRUCTION_H
#define _INSTRUCTION_H

#include <array>
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
        enum Opcode : std::uint8_t {
            NOP = 0,
            ACONST_NULL,
            ICONST_M1,
            ICONST_0,
            ICONST_1,
            ICONST_2,
            ICONST_3,
            ICONST_4,
            ICONST_5,
            LCONST_0,
            LCONST_1,
            FCONST_0,
            FCONST_1,
            FCONST_2,
            DCONST_0,
            DCONST_1,
            BIPUSH,
            SIPUSH,
            LDC,
            LDC_W,
            LDC2_W,
            ILOAD,
            LLOAD,
            FLOAD,
            DLOAD,
            ALOAD,
            ILOAD_0,
            ILOAD_1,
            ILOAD_2,
            ILOAD_3,
            LLOAD_0,
            LLOAD_1,
            LLOAD_2,
            LLOAD_3,
            FLOAD_0,
            FLOAD_1,
            FLOAD_2,
            FLOAD_3,
            DLOAD_0,
            DLOAD_1,
            DLOAD_2,
            DLOAD_3,
            ALOAD_0,
            ALOAD_1,
            ALOAD_2,
            ALOAD_3,
            IALOAD,
            LALOAD,
            FALOAD,
            DALOAD,
            AALOAD,
            BALOAD,
            CALOAD,
            SALOAD,
            ISTORE,
            LSTORE,
            FSTORE,
            DSTORE,
            ASTORE,
            ISTORE_0,
            ISTORE_1,
            ISTORE_2,
            ISTORE_3,
            LSTORE_0,
            LSTORE_1,
            LSTORE_2,
            LSTORE_3,
            FSTORE_0,
            FSTORE_1,
            FSTORE_2,
            FSTORE_3,
            DSTORE_0,
            DSTORE_1,
            DSTORE_2,
            DSTORE_3,
            ASTORE_0,
            ASTORE_1,
            ASTORE_2,
            ASTORE_3,
            IASTORE,
            LASTORE,
            FASTORE,
            DASTORE,
            AASTORE,
            BASTORE,
            CASTORE,
            SASTORE,
            POP,
            POP2,
            DUP,
            DUP_X1,
            DUP_X2,
            DUP2,
            DUP2_X1,
            DUP2_X2,
            SWAP,
            IADD,
            LADD,
            FADD,
            DADD,
            ISUB,
            LSUB,
            FSUB,
            DSUB,
            IMUL,
            LMUL,
            FMUL,
            DMUL,
            IDIV,
            LDIV,
            FDIV,
            DDIV,
            IREM,
            LREM,
            FREM,
            DREM,
            INEG,
            LNEG,
            FNEG,
            DNEG,
            ISHL,
            LSHL,
            ISHR,
            LSHR,
            IUSHR,
            LUSHR,
            IAND,
            LAND,
            IOR,
            LOR,
            IXOR,
            LXOR,
            IINC,
            I2L,
            I2F,
            I2D,
            L2I,
            L2F,
            L2D,
            F2I,
            F2L,
            F2D,
            D2I,
            D2L,
            D2F,
            I2B,
            I2C,
            I2S,
            LCMP,
            FCMPL,
            FCMPG,
            DCMPL,
            DCMPG,
            IFEQ,
            IFNE,
            IFLT,
            IFGE,
            IFGT,
            IFLE,
            IF_ICMPEQ,
            IF_ICMPNE,
            IF_ICMPLT,
            IF_ICMPGE,
            IF_ICMPGT,
            IF_ICMPLE,
            IF_ACMPEQ,
            IF_ACMPNE,
            GOTO,
            JSR,
            RET,
            TABLESWITCH,
            LOOKUPSWITCH,
            IRETURN,
            LRETURN,
            FRETURN,
            DRETURN,
            ARETURN,
            RETURN,
            GETSTATIC,
            PUTSTATIC,
            GETFIELD,
            PUTFIELD,
            INVOKEVIRTUAL,
            INVOKESPECIAL,
            INVOKESTATIC,
            INVOKEINTERFACE,
            INVOKEDYNAMIC,
            NEW,
            NEWARRAY,
            ANEWARRAY,
            ARRAYLENGTH,
            ATHROW,
            CHECKCAST,
            INSTANCEOF,
            MONITORENTER,
            MONITOREXIT,
            WIDE,
            MULTIANEWARRAY,
            IFNULL,
            IFNONNULL,
            GOTO_W,
            JSR_W,
            BREAKPOINT,
            IMPDEP1,
            IMPDEP2,
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
            M1 = -1,
            ZERO = 0,
            ONE = 1,
            TWO = 2,
            THREE = 3,
            FOUR = 4,
            FIVE = 5
        };

        Instruction() {}
        explicit Instruction(std::uint8_t opcodeByte) : opcodeByte(opcodeByte), type(NoTy) {}
        Instruction(std::uint8_t opcodeByte, Type type) : opcodeByte(opcodeByte), type(type) {}
        Instruction(std::uint8_t opcodeByte, Type type, std::optional<ImplicitValue> value)
                : opcodeByte(opcodeByte), type(type), value(value) {}
        Instruction(std::uint8_t opcodeByte, std::vector<std::uint8_t> operands)
                : opcodeByte(opcodeByte), operands(std::move(operands)) {}
        Instruction(std::uint8_t opcodeByte, std::vector<std::uint8_t> operands, Type type)
                : opcodeByte(opcodeByte), operands(std::move(operands)), type(type) {}
        Instruction(std::uint8_t opcodeByte, std::vector<std::uint8_t> operands, Type type, std::optional<ImplicitValue> value)
            : opcodeByte(opcodeByte), operands(std::move(operands)), type(type), value(value) {}

        static Opcode getOpcodeFromOpcodeByte(std::uint8_t opcodeByte);

        // Return the number of argument bytes this opcode takes
        // Only use for non-variable arg counts - does not work on instructions like tableswitch
        static int getUniformArgCountFromOpcodeByte(std::uint8_t opcodeByte);

        int getSizeInBytes() { return 1 + operands.size(); }

        static std::array<Instruction::Opcode, 256> opcodeLookup;
    private:
        void _serialize() override;

        // Only stores non-uniform arg counts
        static std::map<std::uint8_t, int> opcodeArgCountMap;
        std::uint8_t opcodeByte;
    protected:
        Type type;
        std::vector<std::uint8_t> operands;
        std::optional<ImplicitValue> value;
    };

    struct InstructionByteOperand : public Instruction {
        InstructionByteOperand(std::uint8_t opcodeByte, std::uint8_t operand) : Instruction(opcodeByte, {operand}) {}
        InstructionByteOperand(std::uint8_t opcodeByte, std::uint8_t operand, Type type) : Instruction(opcodeByte, {operand}, type) {}
    };

    struct InstructionShortOperand : public Instruction {
        InstructionShortOperand(std::uint8_t opcodeByte, std::uint8_t operand1, std::uint8_t operand2) : Instruction(opcodeByte, {operand1, operand2}) {}
        InstructionShortOperand(std::uint8_t opcodeByte, std::uint8_t operand1, std::uint8_t operand2, Type type) : Instruction(opcodeByte, {operand1, operand2}, type) {}

        InstructionShortOperand(std::uint8_t opcodeByte, std::uint16_t operand) : Instruction(opcodeByte) {
            operands.push_back((operand & 0xFF00) >> 8);
            operands.push_back(operand & 0xFF);
        }

        InstructionShortOperand(std::uint8_t opcodeByte, std::uint16_t operand, Type type) : InstructionShortOperand(opcodeByte, operand) {
            type = type;
        }
    };

    static const Instruction Nop(0x00);
    static const Instruction AConstNull(0x01, Instruction::ReferenceTy, Instruction::NULL_VAL);

    // iconst_<n>
    static const Instruction IConstM1(0x02, Instruction::IntTy, Instruction::M1);
    static const Instruction IConst0(0x03, Instruction::IntTy, Instruction::ZERO);
    static const Instruction IConst1(0x04, Instruction::IntTy, Instruction::ONE);
    static const Instruction IConst2(0x05, Instruction::IntTy, Instruction::TWO);
    static const Instruction IConst3(0x06, Instruction::IntTy, Instruction::THREE);
    static const Instruction IConst4(0x07, Instruction::IntTy, Instruction::FOUR);
    static const Instruction IConst5(0x08, Instruction::IntTy, Instruction::FIVE);

    // lconst_<n>
    static const Instruction LConst0(0x09, Instruction::LongTy, Instruction::ZERO);
    static const Instruction LConst1(0x0A, Instruction::LongTy, Instruction::ONE);

    // fconst_<n>
    static const Instruction FConst0(0x0B, Instruction::FloatTy, Instruction::ZERO);
    static const Instruction FConst1(0x0C, Instruction::FloatTy, Instruction::ONE);
    static const Instruction FConst2(0x0D, Instruction::FloatTy, Instruction::TWO);

    // dconst_<n>
    static const Instruction DConst0(0x0E, Instruction::DoubleTy, Instruction::ZERO);
    static const Instruction DConst1(0x0F, Instruction::DoubleTy, Instruction::ONE);

    struct BiPush : public InstructionByteOperand {
        explicit BiPush(std::uint8_t operand) : InstructionByteOperand(0x10, operand, ByteTy) {}
    };

    struct SiPush : public InstructionShortOperand {
        explicit SiPush(std::uint16_t operand) : InstructionShortOperand(0x11, operand, ByteTy) {}
        SiPush(std::uint8_t operand1, std::uint8_t operand2) : InstructionShortOperand(0x11, operand1, operand2, ByteTy) {}
    };

    struct Ldc : public InstructionByteOperand {
        explicit Ldc(std::uint8_t operand) : InstructionByteOperand(0x12, operand) {}
    };

    struct LdcW : public InstructionShortOperand {
        explicit LdcW(std::uint16_t operand) : InstructionShortOperand(0x13, operand) {}
        LdcW(std::uint8_t operand1, std::uint8_t operand2) : InstructionShortOperand(0x13, operand1, operand2) {}
    };

    struct Ldc2W : public InstructionShortOperand {
        explicit Ldc2W(std::uint16_t operand) : InstructionShortOperand(0x14, operand) {}
        Ldc2W(std::uint8_t operand1, std::uint8_t operand2) : InstructionShortOperand(0x14, operand1, operand2) {}
    };

    struct ILoad : public InstructionByteOperand {
        explicit ILoad(std::uint8_t operand) : InstructionByteOperand(0x15, operand, IntTy) {}
    };

    struct LLoad : public InstructionByteOperand {
        explicit LLoad(std::uint8_t operand) : InstructionByteOperand(0x16, operand, LongTy) {}
    };

    struct FLoad : public InstructionByteOperand {
        explicit FLoad(std::uint8_t operand) : InstructionByteOperand(0x17, operand, FloatTy) {}
    };

    struct DLoad : public InstructionByteOperand {
        explicit DLoad(std::uint8_t operand) : InstructionByteOperand(0x18, operand, DoubleTy) {}
    };

    struct ALoad : public InstructionByteOperand {
        explicit ALoad(std::uint8_t operand) : InstructionByteOperand(0x19, operand, ReferenceTy) {}
    };

    // iload_<n>
    static const Instruction ILoad0(0x1A, Instruction::IntTy, Instruction::ZERO);
    static const Instruction ILoad1(0x1B, Instruction::IntTy, Instruction::ONE);
    static const Instruction ILoad2(0x1C, Instruction::IntTy, Instruction::TWO);
    static const Instruction ILoad3(0x1D, Instruction::IntTy, Instruction::THREE);

    // lload_<n>
    static const Instruction LLoad0(0x1E, Instruction::LongTy, Instruction::ZERO);
    static const Instruction LLoad1(0x1F, Instruction::LongTy, Instruction::ONE);
    static const Instruction LLoad2(0x20, Instruction::LongTy, Instruction::TWO);
    static const Instruction LLoad3(0x21, Instruction::LongTy, Instruction::THREE);

    // fload_<n>
    static const Instruction FLoad0(0x22, Instruction::FloatTy, Instruction::ZERO);
    static const Instruction FLoad1(0x23, Instruction::FloatTy, Instruction::ONE);
    static const Instruction FLoad2(0x24, Instruction::FloatTy, Instruction::TWO);
    static const Instruction FLoad3(0x25, Instruction::FloatTy, Instruction::THREE);

    // dload_<n>
    static const Instruction DLoad0(0x26, Instruction::DoubleTy, Instruction::ZERO);
    static const Instruction DLoad1(0x27, Instruction::DoubleTy, Instruction::ONE);
    static const Instruction DLoad2(0x28, Instruction::DoubleTy, Instruction::TWO);
    static const Instruction DLoad3(0x29, Instruction::DoubleTy, Instruction::THREE);

    // aload_<n>
    static const Instruction ALoad0(0x2A, Instruction::ReferenceTy, Instruction::ZERO);
    static const Instruction ALoad1(0x2B, Instruction::ReferenceTy, Instruction::ONE);
    static const Instruction ALoad2(0x2C, Instruction::ReferenceTy, Instruction::TWO);
    static const Instruction ALoad3(0x2D, Instruction::ReferenceTy, Instruction::TWO);

    // Taload_<n>
    static const Instruction IALoad(0x2E, Instruction::IntTy);
    static const Instruction LALoad(0x2F, Instruction::LongTy);
    static const Instruction FALoad(0x30, Instruction::FloatTy);
    static const Instruction DALoad(0x31, Instruction::DoubleTy);
    static const Instruction AALoad(0x32, Instruction::ReferenceTy);
    static const Instruction BALoad(0x33, Instruction::ByteTy);
    static const Instruction CALoad(0x34, Instruction::CharTy);
    static const Instruction SALoad(0x35, Instruction::ShortTy);

    struct IStore : InstructionByteOperand {
        explicit IStore(std::uint8_t operand) : InstructionByteOperand(0x36, operand, IntTy) {}
    };

    struct LStore : InstructionByteOperand {
        explicit LStore(std::uint8_t operand) : InstructionByteOperand(0x37, operand, LongTy) {}
    };

    struct FStore : InstructionByteOperand {
        explicit FStore(std::uint8_t operand) : InstructionByteOperand(0x38, operand, FloatTy) {}
    };

    struct DStore : InstructionByteOperand {
        explicit DStore(std::uint8_t operand) : InstructionByteOperand(0x39, operand, DoubleTy) {}
    };

    struct AStore : InstructionByteOperand {
        explicit AStore(std::uint8_t operand) : InstructionByteOperand(0x3A, operand, ReferenceTy) {}
    };

    // istore_<n>
    static const Instruction IStore0(0x3B, Instruction::IntTy);
    static const Instruction IStore1(0x3C, Instruction::IntTy);
    static const Instruction IStore2(0x3D, Instruction::IntTy);
    static const Instruction IStore3(0x3E, Instruction::IntTy);

    // lstore_<n>
    static const Instruction LStore0(0x3F, Instruction::LongTy);
    static const Instruction LStore1(0x40, Instruction::LongTy);
    static const Instruction LStore2(0x41, Instruction::LongTy);
    static const Instruction LStore3(0x42, Instruction::LongTy);



    static const Instruction IReturn(0xAC, Instruction::IntTy);

    static const Instruction Return(0xB1);

    struct GetField : InstructionShortOperand {
        explicit GetField(std::uint16_t operand) : InstructionShortOperand(0xB4, operand) {}
        GetField(std::uint8_t operand1, std::uint8_t operand2) : InstructionShortOperand(0xB4, operand1, operand2) {}
    };

    struct PutField : InstructionShortOperand {
        explicit PutField(std::uint16_t operand) : InstructionShortOperand(0xB5, operand) {}
        PutField(std::uint8_t operand1, std::uint8_t operand2) : InstructionShortOperand(0xB5, operand1, operand2) {}
    };

    struct InvokeSpecial : InstructionShortOperand {
        explicit InvokeSpecial(std::uint16_t operand) : InstructionShortOperand(0xB7, operand) {}
        InvokeSpecial(std::uint8_t operand1, std::uint8_t operand2) : InstructionShortOperand(0xB7, operand1, operand2) {}
    };
}

#endif //_INSTRUCTION_H
