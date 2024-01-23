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
            PrimitiveTyID = 0,
            ReturnAddrTyID,
            BooleanTyID,
            ReferenceTyID,
        };
        explicit Type(TypeID tid) : tid(tid) {}

        virtual bool isPrimitive() { return tid == PrimitiveTyID; }
        virtual bool getIntegerType() { return false; }
    private:
        TypeID tid;
    };

    class PrimitiveType : public Type {
    public:
        enum PrimitiveTyID {
            IntegerTyID = 0,
            FloatingPointTyID
        };
        explicit PrimitiveType(PrimitiveTyID ptid) : Type(TypeID::PrimitiveTyID), ptid(ptid) {}
    private:
        PrimitiveTyID ptid;
    };

    class IntegerType : public PrimitiveType {
    public:
        enum IntegerTyID {
            ByteTyID = 0,
            ShortTyID,
            IntTyID,
            LongTyID,
            CharTyID
        };
        explicit IntegerType(IntegerTyID iid) : PrimitiveType(PrimitiveTyID::IntegerTyID), iid(iid) {}

        bool isByte() { return iid == ByteTyID; }
        bool isShort() { return iid == ShortTyID; }
        bool isInt() { return iid == IntTyID; }
        bool isLong() { return iid == LongTyID; }
        bool isChar() { return iid == CharTyID; }

        long long getMaxValue();
        long long getMinValue();
    private:
        IntegerTyID iid;
    };

    class FloatingPointType : public PrimitiveType {
    public:
        enum FloatingPointTyID {
            FloatTy = 0,
            DoubleTy
        };
        explicit FloatingPointType(FloatingPointTyID ftid) : PrimitiveType(PrimitiveTyID::FloatingPointTyID) {
            ftid = ftid;
        }

        bool isFloat() { return ftid == FloatTy; }
        bool isDouble() { return ftid == DoubleTy; }
    private:
        FloatingPointTyID ftid;
    };

    class ReturnAddrType : public Type {
    public:
        ReturnAddrType() : Type(ReturnAddrTyID) {}
    };

    class BooleanType : public Type {
    public:
        BooleanType() : Type(BooleanTyID) {}
    };

    class ReferenceType : public Type {
    public:
        enum ReferenceTyID {
            NullTyID = 0,
            ClassTyID,
            ArrayTyID,
            InterfaceTyID
        };
        explicit ReferenceType(ReferenceTyID rid) : Type(TypeID::ReferenceTyID), rid(rid) {}
        virtual bool isNullReference() { return rid == NullTyID; }
    private:
        ReferenceTyID rid;
    };

    class ClassType : public ReferenceType {
    public:
        ClassType() : ReferenceType(ClassTyID) {}
    };

    class ArrayType : public ReferenceType {
    public:
        explicit ArrayType(Type *componentType) : ReferenceType(ArrayTyID), componentType(componentType) {}
    private:
        Type *componentType;
    };

    class InterfaceType : public ReferenceType {
    public:
        InterfaceType() : ReferenceType(InterfaceTyID) {}
    };
}
#endif //JVMG_TYPES_H