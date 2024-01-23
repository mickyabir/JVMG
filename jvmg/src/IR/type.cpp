#include "jvmg/IR/type.h"

using namespace jvmg;

long long IntegerType::getMaxValue() {
    switch (iid) {
        case ByteTyID:
            return MAX_JVM_BYTE;
        case ShortTyID:
            return MAX_JVM_SHORT;
        case IntTyID:
            return MAX_JVM_INT;
        case LongTyID:
            return MAX_JVM_LONG;
        case CharTyID:
            return MAX_JVM_CHAR;
    }
}

long long IntegerType::getMinValue() {
    switch (iid) {
        case ByteTyID:
            return MIN_JVM_BYTE;
        case ShortTyID:
            return MIN_JVM_SHORT;
        case IntTyID:
            return MIN_JVM_INT;
        case LongTyID:
            return MIN_JVM_LONG;
        case CharTyID:
            return MIN_JVM_CHAR;
    }
}