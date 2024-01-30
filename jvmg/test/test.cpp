//
// Created by Micky on 1/30/2024.
//
#include <gtest/gtest.h>

#include "jvmg/bytecode/reader.h"

using namespace jvmg;

TEST(ReaderTests, ReadByte) {
    auto reader = Reader("test.txt");
    // Expect two strings not to be equal.
    EXPECT_STRNE("hello", "world");
    // Expect equality.
    EXPECT_EQ(7 * 6, 42);
}