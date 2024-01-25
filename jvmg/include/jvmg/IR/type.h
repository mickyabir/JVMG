#ifndef JVMG_TYPES_H
#define JVMG_TYPES_H

#define MAX_JVM_BYTE 127;
#define MAX_JVM_SHORT 32767;
#define MAX_JVM_INT 2147483647;
#define MAX_JVM_LONG 9223372036854775807;
#define MAX_JVM_CHAR 65535;

#define MIN_JVM_BYTE -128;
#define MIN_JVM_SHORT -32768;
#define MIN_JVM_INT -2147483648;
#define MIN_JVM_LONG -9223372036854775808;
#define MIN_JVM_CHAR 0;

namespace jvmg {
    class Type {
    public:
        enum TypeID {
            ByteTyID = 0,
            ShortTyID,
            IntTyID,
            LongTyID,
            CharTyID,
            FloatTy,
            DoubleTy,
            ReturnAddrTyID,
            BooleanTyID,
            NullTyID,
            ClassTyID,
            ArrayTyID,
            InterfaceTyID,
            ReferenceTyID
        };
        explicit Type(TypeID tid) : tid(tid) {}

    private:
        TypeID tid;
    };
}
#endif //JVMG_TYPES_H