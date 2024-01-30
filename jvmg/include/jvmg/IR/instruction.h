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
            NULL_VAL = 0,
            M1,
            ZERO,
            ONE,
            TWO,
            THREE,
            FOUR,
            FIVE
        };

        Instruction() : opcodeByte(0x0), type(NoTy) {}
        explicit Instruction(std::uint8_t opcodeByte) : opcodeByte(opcodeByte), type(NoTy) {}
        Instruction(std::uint8_t opcodeByte, Type type) : opcodeByte(opcodeByte), type(type) {}
        Instruction(std::uint8_t opcodeByte, Type type, std::optional<ImplicitValue> value)
                : opcodeByte(opcodeByte), type(type), value(value) {}
        Instruction(std::uint8_t opcodeByte, std::vector<std::uint8_t> operands)
                : opcodeByte(opcodeByte), operands(std::move(operands)) , type(NoTy) {}
        Instruction(std::uint8_t opcodeByte, std::vector<std::uint8_t> operands, Type type)
                : opcodeByte(opcodeByte), operands(std::move(operands)), type(type) {}
        Instruction(std::uint8_t opcodeByte, std::vector<std::uint8_t> operands, Type type, std::optional<ImplicitValue> value)
            : opcodeByte(opcodeByte), operands(std::move(operands)), type(type), value(value) {}

        static Opcode getOpcodeFromOpcodeByte(std::uint8_t opcodeByte);

        size_t getSizeInBytes() { return 1 + operands.size(); }

        [[nodiscard]] std::uint8_t getOpcodeByte() const { return opcodeByte; }
        [[nodiscard]] Type getType() const { return type; }
        [[nodiscard]] std::optional<ImplicitValue> getImplicitValue() const { return value; }
    private:
        void _serialize() override;

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
        InstructionShortOperand(std::uint8_t opcodeByte, std::uint16_t operand) : Instruction(opcodeByte) {
            operands.push_back((operand & 0xFF00) >> 8);
            operands.push_back(operand & 0xFF);
        }

        InstructionShortOperand(std::uint8_t opcodeByte, std::uint16_t operand, Type type) : InstructionShortOperand(opcodeByte, operand) {
            type = type;
        }
    };

    struct InstructionIntOperand : public Instruction {
        InstructionIntOperand(std::uint8_t opcodeByte, std::uint32_t operand) : Instruction(opcodeByte) {
            operands.push_back((operand & 0xFF000000) >> 24);
            operands.push_back((operand & 0xFF0000) >> 16);
            operands.push_back((operand & 0xFF00) >> 8);
            operands.push_back(operand & 0xFF);
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
    };

    struct Ldc : public InstructionByteOperand {
        explicit Ldc(std::uint8_t operand) : InstructionByteOperand(0x12, operand) {}
    };

    struct LdcW : public InstructionShortOperand {
        explicit LdcW(std::uint16_t operand) : InstructionShortOperand(0x13, operand) {}
    };

    struct Ldc2W : public InstructionShortOperand {
        explicit Ldc2W(std::uint16_t operand) : InstructionShortOperand(0x14, operand) {}
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

    // fstore_<n>
    static const Instruction FStore0(0x43, Instruction::FloatTy);
    static const Instruction FStore1(0x44, Instruction::FloatTy);
    static const Instruction FStore2(0x45, Instruction::FloatTy);
    static const Instruction FStore3(0x46, Instruction::FloatTy);

    // dstore_<n>
    static const Instruction DStore0(0x47, Instruction::DoubleTy);
    static const Instruction DStore1(0x48, Instruction::DoubleTy);
    static const Instruction DStore2(0x49, Instruction::DoubleTy);
    static const Instruction DStore3(0x4A, Instruction::DoubleTy);

    // astore_<n>
    static const Instruction AStore0(0x4B, Instruction::ReferenceTy);
    static const Instruction AStore1(0x4C, Instruction::ReferenceTy);
    static const Instruction AStore2(0x4D, Instruction::ReferenceTy);
    static const Instruction AStore3(0x4E, Instruction::ReferenceTy);

    // Tastore_<n>
    static const Instruction IAStore(0x4F, Instruction::IntTy);
    static const Instruction LAStore(0x50, Instruction::LongTy);
    static const Instruction FAStore(0x51, Instruction::FloatTy);
    static const Instruction DAStore(0x52, Instruction::DoubleTy);
    static const Instruction AAStore(0x53, Instruction::ReferenceTy);
    static const Instruction BAStore(0x54, Instruction::ByteTy);
    static const Instruction CAStore(0x55, Instruction::CharTy);
    static const Instruction SAStore(0x56, Instruction::ShortTy);

    static const Instruction Pop(0x57);
    static const Instruction Pop2(0x58);
    static const Instruction Dup(0x59);
    static const Instruction DupX1(0x5A);
    static const Instruction DupX2(0x5B);
    static const Instruction Dup2(0x5C);
    static const Instruction Dup2X1(0x5D);
    static const Instruction Dup2X2(0x5E);
    static const Instruction Swap(0x5F);

    // Tadd
    static const Instruction IAdd(0x60);
    static const Instruction LAdd(0x61);
    static const Instruction FAdd(0x62);
    static const Instruction DAdd(0x63);

    // Tsub
    static const Instruction ISub(0x64);
    static const Instruction LSub(0x65);
    static const Instruction FSub(0x66);
    static const Instruction DSub(0x67);

    // Tmul
    static const Instruction IMul(0x68);
    static const Instruction LMul(0x69);
    static const Instruction FMul(0x6A);
    static const Instruction DMul(0x6B);

    // Tdiv
    static const Instruction IDiv(0x6C);
    static const Instruction LDiv(0x6D);
    static const Instruction FDiv(0x6E);
    static const Instruction DDiv(0x6F);

    // Trem
    static const Instruction IRem(0x70);
    static const Instruction LRem(0x71);
    static const Instruction FRem(0x72);
    static const Instruction DRem(0x73);

    // Tneg
    static const Instruction INeg(0x74);
    static const Instruction LNeg(0x75);
    static const Instruction FNeg(0x76);
    static const Instruction DNeg(0x77);

    // Tshl
    static const Instruction IShl(0x78);
    static const Instruction LShl(0x79);

    // Tshr
    static const Instruction IShr(0x7A);
    static const Instruction LShr(0x7B);

    // Tushr
    static const Instruction IUshr(0x7C);
    static const Instruction LUshr(0x7D);

    // Tand
    static const Instruction IAnd(0x7E);
    static const Instruction LAnd(0x7F);

    // Tor
    static const Instruction IOr(0x80);
    static const Instruction LOr(0x81);

    // Txor
    static const Instruction IXor(0x82);
    static const Instruction LXor(0x83);

    struct Iinc : InstructionShortOperand {
        explicit Iinc(std::uint16_t operand) : InstructionShortOperand(0x84, operand) {}
    };

    // Conversion
    static const Instruction I2l(0x85);
    static const Instruction I2f(0x86);
    static const Instruction I2d(0x87);
    static const Instruction L2i(0x88);
    static const Instruction L2f(0x89);
    static const Instruction L2d(0x8A);
    static const Instruction F2i(0x8B);
    static const Instruction F2l(0x8C);
    static const Instruction F2d(0x8D);
    static const Instruction D2i(0x8E);
    static const Instruction D2l(0x8F);
    static const Instruction D2f(0x90);
    static const Instruction I2b(0x91);
    static const Instruction I2c(0x92);
    static const Instruction I2s(0x93);

    // Compare
    static const Instruction LCmp(0x94);
    static const Instruction FCmpL(0x95);
    static const Instruction FCmpG(0x96);
    static const Instruction DCmpL(0x97);
    static const Instruction DCmpG(0x98);


    struct IfEq : InstructionShortOperand {
        explicit IfEq(std::uint16_t operand) : InstructionShortOperand(0x99, operand) {}
    };

    struct IfNe : InstructionShortOperand {
        explicit IfNe(std::uint16_t operand) : InstructionShortOperand(0x9A, operand) {}
    };

    struct IfLt : InstructionShortOperand {
        explicit IfLt(std::uint16_t operand) : InstructionShortOperand(0x9B, operand) {}
    };

    struct IfGe : InstructionShortOperand {
        explicit IfGe(std::uint16_t operand) : InstructionShortOperand(0x9C, operand) {}
    };

    struct IfGt : InstructionShortOperand {
        explicit IfGt(std::uint16_t operand) : InstructionShortOperand(0x9D, operand) {}
    };

    struct IfLe : InstructionShortOperand {
        explicit IfLe(std::uint16_t operand) : InstructionShortOperand(0x9E, operand) {}
    };

    struct IfICmpEq : InstructionShortOperand {
        explicit IfICmpEq(std::uint16_t operand) : InstructionShortOperand(0x9F, operand) {}
    };

    struct IfICmpNe : InstructionShortOperand {
        explicit IfICmpNe(std::uint16_t operand) : InstructionShortOperand(0xA0, operand) {}
    };

    struct IfICmpLt : InstructionShortOperand {
        explicit IfICmpLt(std::uint16_t operand) : InstructionShortOperand(0xA1, operand) {}
    };

    struct IfICmpGe : InstructionShortOperand {
        explicit IfICmpGe(std::uint16_t operand) : InstructionShortOperand(0xA2, operand) {}
    };

    struct IfICmpGt : InstructionShortOperand {
        explicit IfICmpGt(std::uint16_t operand) : InstructionShortOperand(0xA3, operand) {}
    };

    struct IfICmpLe : InstructionShortOperand {
        explicit IfICmpLe(std::uint16_t operand) : InstructionShortOperand(0xA4, operand) {}
    };

    struct IfACmpEq : InstructionShortOperand {
        explicit IfACmpEq(std::uint16_t operand) : InstructionShortOperand(0xA5, operand) {}
    };

    struct IfACmpNe : InstructionShortOperand {
        explicit IfACmpNe(std::uint16_t operand) : InstructionShortOperand(0xA6, operand) {}
    };

    struct Goto : InstructionShortOperand {
        explicit Goto(std::uint16_t operand) : InstructionShortOperand(0xA7, operand) {}
    };

    struct Jsr : InstructionShortOperand {
        explicit Jsr(std::uint16_t operand) : InstructionShortOperand(0xA8, operand) {}
    };

    struct Ret : InstructionShortOperand {
        explicit Ret(std::uint8_t operand) : InstructionShortOperand(0xA9, operand) {}
    };

    class Tableswitch : public Instruction {
    public:
        Tableswitch(std::uint8_t numPadding, std::uint32_t defaultValue, std::uint32_t lowValue, std::uint32_t highValue, const std::vector<std::int32_t>& indices)
            : Instruction(0xAA) {
            for (int i = 0; i < numPadding; i++) {
                operands.push_back(0x0);
            }

            operands.push_back((defaultValue & 0xFF000000) >> 24);
            operands.push_back((defaultValue & 0xFF0000) >> 16);
            operands.push_back((defaultValue & 0xFF00) >> 8);
            operands.push_back(defaultValue & 0xFF);

            operands.push_back((lowValue & 0xFF000000) >> 24);
            operands.push_back((lowValue & 0xFF0000) >> 16);
            operands.push_back((lowValue & 0xFF00) >> 8);
            operands.push_back(lowValue & 0xFF);

            operands.push_back((highValue & 0xFF000000) >> 24);
            operands.push_back((highValue & 0xFF0000) >> 16);
            operands.push_back((highValue & 0xFF00) >> 8);
            operands.push_back(highValue & 0xFF);

            for (int index : indices) {
                operands.push_back((index & 0xFF000000) >> 24);
                operands.push_back((index & 0xFF0000) >> 16);
                operands.push_back((index & 0xFF00) >> 8);
                operands.push_back(index & 0xFF);
            }
        }
    };

    class Lookupswitch : public Instruction {
    public:
        Lookupswitch(std::uint8_t numPadding, std::uint32_t defaultValue, std::uint32_t nPairs, const std::vector<std::pair<std::int32_t, std::int32_t>>& pairs)
        : Instruction(0xAB) {

            for (int i = 0; i < numPadding; i++) {
                operands.push_back(0x0);
            }

            operands.push_back((defaultValue & 0xFF000000) >> 24);
            operands.push_back((defaultValue & 0xFF0000) >> 16);
            operands.push_back((defaultValue & 0xFF00) >> 8);
            operands.push_back(defaultValue & 0xFF);

            operands.push_back((nPairs & 0xFF000000) >> 24);
            operands.push_back((nPairs & 0xFF0000) >> 16);
            operands.push_back((nPairs & 0xFF00) >> 8);
            operands.push_back(nPairs & 0xFF);

            for (auto pair : pairs) {
                auto match = pair.first;
                auto offset = pair.first;

                operands.push_back((match & 0xFF000000) >> 24);
                operands.push_back((match & 0xFF0000) >> 16);
                operands.push_back((match & 0xFF00) >> 8);
                operands.push_back(match & 0xFF);

                operands.push_back((offset & 0xFF000000) >> 24);
                operands.push_back((offset & 0xFF0000) >> 16);
                operands.push_back((offset & 0xFF00) >> 8);
                operands.push_back(offset & 0xFF);
            }
        }
    };

    // Treturn
    static const Instruction IReturn(0xAC, Instruction::IntTy);
    static const Instruction LReturn(0xAD, Instruction::LongTy);
    static const Instruction FReturn(0xAE, Instruction::FloatTy);
    static const Instruction DReturn(0xAF, Instruction::DoubleTy);
    static const Instruction AReturn(0xB0, Instruction::ReferenceTy);

    static const Instruction Return(0xB1);

    struct GetStatic : InstructionShortOperand {
        explicit GetStatic(std::uint16_t operand) : InstructionShortOperand(0xB2, operand) {}
    };

    struct PutStatic : InstructionShortOperand {
        explicit PutStatic(std::uint16_t operand) : InstructionShortOperand(0xB3, operand) {}
    };

    struct GetField : InstructionShortOperand {
        explicit GetField(std::uint16_t operand) : InstructionShortOperand(0xB4, operand) {}
    };

    struct PutField : InstructionShortOperand {
        explicit PutField(std::uint16_t operand) : InstructionShortOperand(0xB5, operand) {}
    };

    struct InvokeVirtual : InstructionShortOperand {
        explicit InvokeVirtual(std::uint16_t operand) : InstructionShortOperand(0xB6, operand) {}
    };

    struct InvokeSpecial : InstructionShortOperand {
        explicit InvokeSpecial(std::uint16_t operand) : InstructionShortOperand(0xB7, operand) {}
    };

    struct InvokeStatic : InstructionShortOperand {
        explicit InvokeStatic(std::uint16_t operand) : InstructionShortOperand(0xB8, operand) {}
    };

    struct InvokeInterface : InstructionIntOperand {
        explicit InvokeInterface(std::uint32_t operand) : InstructionIntOperand(0xB9, operand) {}
    };

    struct InvokeDynamic : InstructionIntOperand {
        explicit InvokeDynamic(std::uint32_t operand) : InstructionIntOperand(0xBA, operand) {}
    };

    struct New : InstructionShortOperand {
        explicit New(std::uint16_t operand) : InstructionShortOperand(0xBB, operand) {}
    };

    struct NewArray : InstructionByteOperand {
        explicit NewArray(std::uint8_t operand) : InstructionByteOperand(0xBC, operand) {}
    };

    struct ANewArray : InstructionShortOperand {
        explicit ANewArray(std::uint16_t operand) : InstructionShortOperand(0xBD, operand) {}
    };

    static const Instruction ArrayLength(0xBE);
    static const Instruction AThrow(0xBF);

    struct CheckCast : InstructionShortOperand {
        explicit CheckCast(std::uint16_t operand) : InstructionShortOperand(0xC0, operand) {}
    };

    struct InstanceOf : InstructionShortOperand {
        explicit InstanceOf(std::uint16_t operand) : InstructionShortOperand(0xC1, operand) {}
    };

    static const Instruction MonitorEnter(0xC2);
    static const Instruction MonitorExit(0xC3);

    struct Wide : Instruction {
        Wide(std::uint8_t opcode, std::uint8_t indexByte1, std::uint8_t indexByte2) : Instruction(0xC4, {opcode, indexByte1, indexByte2}) {}

        // Wide - execute iinc
        Wide(std::uint8_t indexByte1, std::uint8_t indexByte2, std::uint8_t countByte1, std::uint8_t countByte2) : Instruction(0xC4, {0x84, indexByte1, indexByte2, countByte1, countByte2}) {}
    };

    struct MultiANewArray : Instruction {
        MultiANewArray(std::uint8_t indexByte1, std::uint8_t indexByte2, std::uint8_t dimensions) : Instruction(0xC5, {indexByte1, indexByte2, dimensions}) {}
        MultiANewArray(std::uint16_t index, std::uint8_t dimensions) : Instruction(0xC5) {
            operands.push_back((index & 0xFF00) >> 8);
            operands.push_back(index & 0xFF);
           operands.push_back(dimensions);
        }
    };

    struct IfNull : InstructionShortOperand {
        explicit IfNull(std::uint8_t operand) : InstructionShortOperand(0xC6, operand) {}
    };

    struct IfNonNull : InstructionShortOperand {
        explicit IfNonNull(std::uint8_t operand) : InstructionShortOperand(0xC7, operand) {}
    };

    struct GotoW : InstructionIntOperand {
        explicit GotoW(std::uint32_t operand) : InstructionIntOperand(0xC8, operand) {}
    };

    struct JsrW : InstructionIntOperand {
        explicit JsrW(std::uint32_t operand) : InstructionIntOperand(0xC9, operand) {}
    };

    static const Instruction Breakpoint(0xCA);

    static const Instruction ImpDep1(0xFE);
    static const Instruction ImpDep2(0xFF);
}

#endif //_INSTRUCTION_H
