//
// Created by Micky on 1/30/2024.
//
#include <gtest/gtest.h>

#include "jvmg/bytecode/reader.h"

using namespace jvmg;

class InstructionTestFixture : public ::testing::Test {
public:
    InstructionTestFixture() {
    }

    void SetUp() override {

    }

    void TearDown() override {

    }

    ~InstructionTestFixture() override = default;
};

TEST_F(InstructionTestFixture, AllInstructionsTagTypeValue) {
    EXPECT_EQ(Nop.getOpcodeFromOpcodeByte(Nop.getOpcodeByte()), Instruction::NOP);
    EXPECT_EQ(AConstNull.getOpcodeFromOpcodeByte(AConstNull.getOpcodeByte()), Instruction::ACONST_NULL);
    EXPECT_EQ(AConstNull.getType(), Instruction::ReferenceTy);
    EXPECT_EQ(AConstNull.getImplicitValue(), Instruction::NULL_VAL);
}