//
// Created by Micky on 1/22/2024.
//

#ifndef _OPERAND_H
#define _OPERAND_H

#include "jvmg/IR/type.h"
#include "jvmg/IR/value.h"

namespace jvmg {
    class operand {
    public:
        enum Category{
            OneUnit = 0,
            TwoUnits,
        };

        explicit operand(PrimitiveType *ty, Value *value);

        int getOffsetSize() { return units == OneUnit ? 1 : 2; }

    private:
        PrimitiveType *ty;
        Category units;
        Value *value;
    };
}
#endif //_OPERAND_H
