//
// Created by Micky on 1/21/2024.
//

#ifndef _VALUE_H
#define _VALUE_H

#include "jvmg/IR/type.h"

#include <cstdint>

namespace jvmg {
    class Value {
    public:
        union Data {
            std::int32_t d1;
            std::int32_t d2[2];
        };

        explicit Value(Data data) : data(data) {}
    private:
        Data data;
    };
}

#endif //_VALUE_H
