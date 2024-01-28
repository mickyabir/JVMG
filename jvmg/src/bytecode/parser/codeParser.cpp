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




        {Instruction::IRETURN, IReturn},

        {Instruction::RETURN, Return},

};

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
            return SiPush(operand1, operand2);
        }
        case Instruction::LDC: {
            auto operand = consumeOneByte();
            return Ldc(operand);
        }
        case Instruction::LDC_W: {
            auto operand1 = consumeOneByte();
            auto operand2 = consumeOneByte();
            return LdcW(operand1, operand2);
        }
        case Instruction::LDC2_W: {
            auto operand1 = consumeOneByte();
            auto operand2 = consumeOneByte();
            return Ldc2W(operand1, operand2);
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

        case Instruction::GETFIELD: {
            auto operand1 = consumeOneByte();
            auto operand2 = consumeOneByte();
            return GetField(operand1, operand2);
        }
        case Instruction::PUTFIELD: {
            auto operand1 = consumeOneByte();
            auto operand2 = consumeOneByte();
            return PutField(operand1, operand2);
        }

        case Instruction::INVOKESPECIAL: {
            auto operand1 = consumeOneByte();
            auto operand2 = consumeOneByte();
            return InvokeSpecial(operand1, operand2);
        }



        case Instruction::INVALID_INSTRUCTION_OPCODE:
        default:
            throw std::invalid_argument("Opcode not implemented: " + std::to_string(opcodeByte));
    }
}