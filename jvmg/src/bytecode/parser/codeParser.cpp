#include "jvmg/bytecode/parser/parser.h"

using namespace jvmg;

static std::map<Instruction::Opcode, Instruction> staticInstMap = {
        {Instruction::ACONST_NULL, AConstNull},

        // iconst_<n>
        {Instruction::ICONST_M1, IConstM1},
        {Instruction::ICONST_0, IConst0},
        {Instruction::ICONST_1, IConst1},
        {Instruction::ICONST_2, IConst2},
        {Instruction::ICONST_3, IConst3},
        {Instruction::ICONST_4, IConst4},
        {Instruction::ICONST_5, IConst5},

        // lconst_<n>
        {Instruction::LCONST_0, LConst0},
        {Instruction::LCONST_1, LConst1},

        // fconst_<n>
        {Instruction::FCONST_0, FConst0},
        {Instruction::FCONST_1, FConst1},
        {Instruction::FCONST_2, FConst2},

        // dconst_<n>
        {Instruction::DCONST_0, DConst0},
        {Instruction::DCONST_1, DConst1},

        // iload_<n>
        {Instruction::ILOAD_0, ILoad0},
        {Instruction::ILOAD_1, ILoad1},
        {Instruction::ILOAD_2, ILoad2},
        {Instruction::ILOAD_3, ILoad3},

        // lload_<n>
        {Instruction::LLOAD_0, LLoad0},
        {Instruction::LLOAD_1, LLoad1},
        {Instruction::LLOAD_2, LLoad2},
        {Instruction::LLOAD_3, LLoad3},

        // fload_<n>
        {Instruction::FLOAD_0, FLoad0},
        {Instruction::FLOAD_1, FLoad1},
        {Instruction::FLOAD_2, FLoad2},
        {Instruction::FLOAD_3, FLoad3},

        // dload_<n>
        {Instruction::DLOAD_0, DLoad0},
        {Instruction::DLOAD_1, DLoad1},
        {Instruction::DLOAD_2, DLoad2},
        {Instruction::DLOAD_3, DLoad3},

        // aload_<n>
        {Instruction::ALOAD_0, ALoad0},
        {Instruction::ALOAD_1, ALoad1},
        {Instruction::ALOAD_2, ALoad2},
        {Instruction::ALOAD_3, ALoad3},

        // Taload_<n>
        {Instruction::IALOAD, IALoad},
        {Instruction::LALOAD, LALoad},
        {Instruction::FALOAD, FALoad},
        {Instruction::DALOAD, DALoad},
        {Instruction::AALOAD, AALoad},
        {Instruction::BALOAD, BALoad},
        {Instruction::CALOAD, CALoad},
        {Instruction::SALOAD, SALoad},

        // istore_<n>
        {Instruction::ISTORE_0, IStore0},
        {Instruction::ISTORE_1, IStore1},
        {Instruction::ISTORE_2, IStore2},
        {Instruction::ISTORE_3, IStore3},

        // lstore_<n>
        {Instruction::LSTORE_0, LStore0},
        {Instruction::LSTORE_1, LStore1},
        {Instruction::LSTORE_2, LStore2},
        {Instruction::LSTORE_3, LStore3},

        // fstore_<n>
        {Instruction::FSTORE_0, FStore0},
        {Instruction::FSTORE_1, FStore1},
        {Instruction::FSTORE_2, FStore2},
        {Instruction::FSTORE_3, FStore3},

        // dstore_<n>
        {Instruction::DSTORE_0, DStore0},
        {Instruction::DSTORE_1, DStore1},
        {Instruction::DSTORE_2, DStore2},
        {Instruction::DSTORE_3, DStore3},

        // astore_<n>
        {Instruction::ASTORE_0, AStore0},
        {Instruction::ASTORE_1, AStore1},
        {Instruction::ASTORE_2, AStore2},
        {Instruction::ASTORE_3, AStore3},

        // Tastore_<n>
        {Instruction::IASTORE, IAStore},
        {Instruction::LASTORE, LAStore},
        {Instruction::FASTORE, FAStore},
        {Instruction::DASTORE, DAStore},
        {Instruction::AASTORE, AAStore},
        {Instruction::BASTORE, BAStore},
        {Instruction::CASTORE, CAStore},
        {Instruction::SASTORE, SAStore},

        {Instruction::POP, Pop},
        {Instruction::POP2, Pop2},
        {Instruction::DUP, Dup},
        {Instruction::DUP_X1, DupX1},
        {Instruction::DUP_X2, DupX2},
        {Instruction::DUP2, Dup2},
        {Instruction::DUP2, Dup2},
        {Instruction::DUP2_X1, Dup2X1},
        {Instruction::DUP2_X2, Dup2X2},
        {Instruction::SWAP, Swap},

        // Tadd
        {Instruction::IADD, IAdd},
        {Instruction::LADD, LAdd},
        {Instruction::FADD, FAdd},
        {Instruction::DADD, DAdd},

        // Tsub
        {Instruction::ISUB, ISub},
        {Instruction::LSUB, LSub},
        {Instruction::FSUB, FSub},
        {Instruction::DSUB, DSub},

        // Tmul
        {Instruction::IMUL, IMul},
        {Instruction::LMUL, LMul},
        {Instruction::FMUL, FMul},
        {Instruction::DMUL, DMul},

        // Tdiv
        {Instruction::IDIV, IDiv},
        {Instruction::LDIV, LDiv},
        {Instruction::FDIV, FDiv},
        {Instruction::DDIV, DDiv},

        // Trem
        {Instruction::IREM, IRem},
        {Instruction::LREM, LRem},
        {Instruction::FREM, FRem},
        {Instruction::DREM, DRem},

        // Tneg
        {Instruction::INEG, INeg},
        {Instruction::LNEG, LNeg},
        {Instruction::FNEG, FNeg},
        {Instruction::DNEG, DNeg},

        // Tshl
        {Instruction::ISHL, IShl},
        {Instruction::LSHL, LShl},

        // Tshr
        {Instruction::ISHR, IShr},
        {Instruction::LSHR, LShr},

        // Tushr
        {Instruction::IUSHR, IUshr},
        {Instruction::LUSHR, LUshr},

        // Tand
        {Instruction::IAND, IAnd},
        {Instruction::LAND, LAnd},

        // Tor
        {Instruction::IOR, IOr},
        {Instruction::LOR, LOr},

        // Txor
        {Instruction::IXOR, IXor},
        {Instruction::LXOR, LXor},

        // Conversion
        {Instruction::I2L, I2l},
        {Instruction::I2F, I2f},
        {Instruction::I2D, I2d},
        {Instruction::L2I, L2i},
        {Instruction::L2F, L2f},
        {Instruction::L2D, L2d},
        {Instruction::F2I, F2i},
        {Instruction::F2L, F2l},
        {Instruction::F2D, F2d},
        {Instruction::D2I, D2i},
        {Instruction::D2L, D2l},
        {Instruction::D2F, D2f},
        {Instruction::I2B, I2b},
        {Instruction::I2C, I2c},
        {Instruction::I2S, I2s},

        // Compare
        {Instruction::LCMP, LCmp},
        {Instruction::FCMPL, FCmpL},
        {Instruction::FCMPG, FCmpG},
        {Instruction::DCMPL, DCmpL},
        {Instruction::DCMPG, DCmpG},

        // Treturn
        {Instruction::IRETURN, IReturn},
        {Instruction::LRETURN, LReturn},
        {Instruction::FRETURN, FReturn},
        {Instruction::DRETURN, DReturn},
        {Instruction::ARETURN, AReturn},

        {Instruction::RETURN, Return},

        {Instruction::MONITORENTER, MonitorEnter},
        {Instruction::MONITOREXIT, MonitorExit},

        {Instruction::BREAKPOINT, Breakpoint},

        {Instruction::IMPDEP1, ImpDep1},
        {Instruction::IMPDEP2, ImpDep2},
};

std::uint16_t combineTwoBytesReverse(std::uint8_t byte1, std::uint8_t byte2) {
    return (byte1 << 8) | byte2;
}

std::uint16_t combineFourBytesReverse(std::uint8_t byte1, std::uint8_t byte2, std::uint8_t byte3, std::uint8_t byte4) {
    return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
}

Instruction Parser::consumeInstruction() {
    std::uint8_t opcodeByte = consumeOneByte();
    auto opcode = Instruction::getOpcodeFromOpcodeByte(opcodeByte);

    if (staticInstMap.find(opcode) != staticInstMap.end()) {
        return staticInstMap[opcode];
    }

    switch (opcode) {
        case Instruction::BIPUSH: {
            auto operand = consumeOneByte();
            return BiPush(operand);
        }
        case Instruction::SIPUSH: {
            auto operand1 = consumeOneByte();
            auto operand2 = consumeOneByte();
            auto operand = combineTwoBytesReverse(operand1, operand2);
            return SiPush(operand);
        }
        case Instruction::LDC: {
            auto operand = consumeOneByte();
            return Ldc(operand);
        }
        case Instruction::LDC_W: {
            auto operand1 = consumeOneByte();
            auto operand2 = consumeOneByte();
            auto operand = combineTwoBytesReverse(operand1, operand2);
            return LdcW(operand);
        }
        case Instruction::LDC2_W: {
            auto operand1 = consumeOneByte();
            auto operand2 = consumeOneByte();
            auto operand = combineTwoBytesReverse(operand1, operand2);
            return Ldc2W(operand);
        }
        case Instruction::ILOAD: {
            auto operand = consumeOneByte();
            return ILoad(operand);
        }
        case Instruction::LLOAD: {
            auto operand = consumeOneByte();
            return LLoad(operand);
        }
        case Instruction::FLOAD: {
            auto operand = consumeOneByte();
            return FLoad(operand);
        }
        case Instruction::DLOAD: {
            auto operand = consumeOneByte();
            return DLoad(operand);
        }
        case Instruction::ALOAD: {
            auto operand = consumeOneByte();
            return ALoad(operand);
        }
        case Instruction::ISTORE: {
            auto operand = consumeOneByte();
            return IStore(operand);
        }
        case Instruction::LSTORE: {
            auto operand = consumeOneByte();
            return LStore(operand);
        }
        case Instruction::FSTORE: {
            auto operand = consumeOneByte();
            return FStore(operand);
        }
        case Instruction::DSTORE: {
            auto operand = consumeOneByte();
            return DStore(operand);
        }
        case Instruction::ASTORE: {
            auto operand = consumeOneByte();
            return AStore(operand);
        }
        case Instruction::IINC: {
            auto operand1 = consumeOneByte();
            auto operand2 = consumeOneByte();
            auto operand = combineTwoBytesReverse(operand1, operand2);
            return Iinc(operand);
        }
        case Instruction::IFEQ: {
            auto operand1 = consumeOneByte();
            auto operand2 = consumeOneByte();
            auto operand = combineTwoBytesReverse(operand1, operand2);
            return IfEq(operand);
        }
        case Instruction::IFNE: {
            auto operand1 = consumeOneByte();
            auto operand2 = consumeOneByte();
            auto operand = combineTwoBytesReverse(operand1, operand2);
            return IfNe(operand);
        }
        case Instruction::IFLT: {
            auto operand1 = consumeOneByte();
            auto operand2 = consumeOneByte();
            auto operand = combineTwoBytesReverse(operand1, operand2);
            return IfLt(operand);
        }
        case Instruction::IFGE: {
            auto operand1 = consumeOneByte();
            auto operand2 = consumeOneByte();
            auto operand = combineTwoBytesReverse(operand1, operand2);
            return IfGe(operand);
        }
        case Instruction::IFGT: {
            auto operand1 = consumeOneByte();
            auto operand2 = consumeOneByte();
            auto operand = combineTwoBytesReverse(operand1, operand2);
            return IfGt(operand);
        }
        case Instruction::IFLE: {
            auto operand1 = consumeOneByte();
            auto operand2 = consumeOneByte();
            auto operand = combineTwoBytesReverse(operand1, operand2);
            return IfLe(operand);
        }
        case Instruction::IF_ICMPEQ: {
            auto operand1 = consumeOneByte();
            auto operand2 = consumeOneByte();
            auto operand = combineTwoBytesReverse(operand1, operand2);
            return IfICmpEq(operand);
        }
        case Instruction::IF_ICMPNE: {
            auto operand1 = consumeOneByte();
            auto operand2 = consumeOneByte();
            auto operand = combineTwoBytesReverse(operand1, operand2);
            return IfICmpNe(operand);
        }
        case Instruction::IF_ICMPLT: {
            auto operand1 = consumeOneByte();
            auto operand2 = consumeOneByte();
            auto operand = combineTwoBytesReverse(operand1, operand2);
            return IfICmpLt(operand);
        }
        case Instruction::IF_ICMPGE: {
            auto operand1 = consumeOneByte();
            auto operand2 = consumeOneByte();
            auto operand = combineTwoBytesReverse(operand1, operand2);
            return IfICmpGe(operand);
        }
        case Instruction::IF_ICMPGT: {
            auto operand1 = consumeOneByte();
            auto operand2 = consumeOneByte();
            auto operand = combineTwoBytesReverse(operand1, operand2);
            return IfICmpGt(operand);
        }
        case Instruction::IF_ICMPLE: {
            auto operand1 = consumeOneByte();
            auto operand2 = consumeOneByte();
            auto operand = combineTwoBytesReverse(operand1, operand2);
            return IfICmpLe(operand);
        }
        case Instruction::IF_ACMPEQ: {
            auto operand1 = consumeOneByte();
            auto operand2 = consumeOneByte();
            auto operand = combineTwoBytesReverse(operand1, operand2);
            return IfACmpEq(operand);
        }
        case Instruction::IF_ACMPNE: {
            auto operand1 = consumeOneByte();
            auto operand2 = consumeOneByte();
            auto operand = combineTwoBytesReverse(operand1, operand2);
            return IfACmpNe(operand);
        }
        case Instruction::GOTO: {
            auto operand1 = consumeOneByte();
            auto operand2 = consumeOneByte();
            auto operand = combineTwoBytesReverse(operand1, operand2);
            return Goto(operand);
        }
        case Instruction::JSR: {
            auto operand1 = consumeOneByte();
            auto operand2 = consumeOneByte();
            auto operand = combineTwoBytesReverse(operand1, operand2);
            return Jsr(operand);
        }
        case Instruction::RET: {
            auto operand = consumeOneByte();
            return Ret(operand);
        }
        case Instruction::TABLESWITCH: {
            // Consume padding
            std::uint8_t numPadding = 0;
            while ((context->getByteOffset() - context->getCodeStartOffset()) % 4 != 0) {
                consumeOneByte();
                numPadding++;
            }
            auto defaultByte1 = consumeOneByte();
            auto defaultByte2 = consumeOneByte();
            auto defaultByte3 = consumeOneByte();
            auto defaultByte4 = consumeOneByte();
            auto lowByte1 = consumeOneByte();
            auto lowByte2 = consumeOneByte();
            auto lowByte3 = consumeOneByte();
            auto lowByte4 = consumeOneByte();
            auto highByte1 = consumeOneByte();
            auto highByte2 = consumeOneByte();
            auto highByte3 = consumeOneByte();
            auto highByte4 = consumeOneByte();

            auto defaultValue = (defaultByte1 << 24) | (defaultByte2 << 16) | (defaultByte3) | defaultByte4;
            auto lowValue = (lowByte1 << 24) | (lowByte2 << 16) | (lowByte3) | lowByte4;
            auto highValue = (highByte1 << 24) | (highByte2 << 16) | (highByte3) | highByte4;

            std::vector<std::int32_t> indices;
            for (int i = 0; i < (highValue - lowValue + 1); i++) {
                indices.push_back((std::int32_t)consumeFourBytes());
            }

            return Tableswitch(numPadding, defaultValue, lowValue, highValue, indices);
        }
        case Instruction::GETSTATIC: {
            auto operand1 = consumeOneByte();
            auto operand2 = consumeOneByte();
            auto operand = combineTwoBytesReverse(operand1, operand2);
            return GetStatic(operand);
        }
        case Instruction::PUTSTATIC: {
            auto operand1 = consumeOneByte();
            auto operand2 = consumeOneByte();
            auto operand = combineTwoBytesReverse(operand1, operand2);
            return PutStatic(operand);
        }
        case Instruction::GETFIELD: {
            auto operand1 = consumeOneByte();
            auto operand2 = consumeOneByte();
            auto operand = combineTwoBytesReverse(operand1, operand2);
            return GetField(operand);
        }
        case Instruction::PUTFIELD: {
            auto operand1 = consumeOneByte();
            auto operand2 = consumeOneByte();
            auto operand = combineTwoBytesReverse(operand1, operand2);
            return PutField(operand);
        }
        case Instruction::INVOKEVIRTUAL: {
            auto operand1 = consumeOneByte();
            auto operand2 = consumeOneByte();
            auto operand = combineTwoBytesReverse(operand1, operand2);
            return InvokeVirtual(operand);
        }
        case Instruction::INVOKESPECIAL: {
            auto operand1 = consumeOneByte();
            auto operand2 = consumeOneByte();
            auto operand = combineTwoBytesReverse(operand1, operand2);
            return InvokeSpecial(operand);
        }
        case Instruction::INVOKESTATIC: {
            auto operand1 = consumeOneByte();
            auto operand2 = consumeOneByte();
            auto operand = combineTwoBytesReverse(operand1, operand2);
            return InvokeStatic(operand);
        }
        case Instruction::INVOKEINTERFACE: {
            auto operand1 = consumeOneByte();
            auto operand2 = consumeOneByte();
            auto operand3 = consumeOneByte();
            auto operand4 = consumeOneByte();
            auto operand = combineFourBytesReverse(operand1, operand2, operand3, operand4);
            return InvokeInterface(operand);
        }
        case Instruction::INVOKEDYNAMIC: {
            auto operand1 = consumeOneByte();
            auto operand2 = consumeOneByte();
            auto operand3 = consumeOneByte();
            auto operand4 = consumeOneByte();
            auto operand = combineFourBytesReverse(operand1, operand2, operand3, operand4);
            return InvokeDynamic(operand);
        }
        case Instruction::NEW: {
            auto operand1 = consumeOneByte();
            auto operand2 = consumeOneByte();
            auto operand = combineTwoBytesReverse(operand1, operand2);
            return New(operand);
        }
        case Instruction::NEWARRAY: {
            auto operand = consumeOneByte();
            return NewArray(operand);
        }
        case Instruction::ANEWARRAY: {
            auto operand1 = consumeOneByte();
            auto operand2 = consumeOneByte();
            auto operand = combineTwoBytesReverse(operand1, operand2);
            return ANewArray(operand);
        }
        case Instruction::CHECKCAST: {
            auto operand1 = consumeOneByte();
            auto operand2 = consumeOneByte();
            auto operand = combineTwoBytesReverse(operand1, operand2);
            return CheckCast(operand);
        }
        case Instruction::INSTANCEOF: {
            auto operand1 = consumeOneByte();
            auto operand2 = consumeOneByte();
            auto operand = combineTwoBytesReverse(operand1, operand2);
            return InstanceOf(operand);
        }
        case Instruction::WIDE: {
            auto executedOpcode = consumeOneByte();
            if (executedOpcode == 0x84) {
                auto indexByte1 = consumeOneByte();
                auto indexByte2 = consumeOneByte();
                auto countByte1 = consumeOneByte();
                auto countByte2 = consumeOneByte();
                return Wide(indexByte1, indexByte2, countByte1, countByte2);
            } else {
                auto indexByte1 = consumeOneByte();
                auto indexByte2 = consumeOneByte();
                return Wide(opcode, indexByte1, indexByte2);
            }
        }
        case Instruction::MULTIANEWARRAY: {
            auto indexByte1 = consumeOneByte();
            auto indexByte2 = consumeOneByte();
            auto dimensions = consumeOneByte();
            return MultiANewArray(indexByte1, indexByte2, dimensions);
        }
        case Instruction::IFNULL: {
            auto operand = consumeOneByte();
            return IfNull(operand);
        }
        case Instruction::IFNONNULL: {
            auto operand = consumeOneByte();
            return IfNonNull(operand);
        }
        case Instruction::GOTO_W: {
            auto operand1 = consumeOneByte();
            auto operand2 = consumeOneByte();
            auto operand3 = consumeOneByte();
            auto operand4 = consumeOneByte();
            auto operand = combineFourBytesReverse(operand1, operand2, operand3, operand4);
            return GotoW(operand);
        }
        case Instruction::JSR_W: {
            auto operand1 = consumeOneByte();
            auto operand2 = consumeOneByte();
            auto operand3 = consumeOneByte();
            auto operand4 = consumeOneByte();
            auto operand = combineFourBytesReverse(operand1, operand2, operand3, operand4);
            return JsrW(operand);
        }
        case Instruction::INVALID_INSTRUCTION_OPCODE:
        default:
            throw std::invalid_argument("Opcode not implemented: " + std::to_string(opcodeByte));
    }
}