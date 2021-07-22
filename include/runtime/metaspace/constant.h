#ifndef JVM_RUNTIME_CONSTANT_H
#define JVM_RUNTIME_CONSTANT_H

#include <string>
#include <vector>
#include <memory>

#include "include/std.h"
#include "include/classfile/constant.h"

namespace runtime {
    enum ConstantType {
        INTEGER,
        FLOAT,
        LONG,
        DOUBLE,
        STRING,
        CLASSREF,
        FIELDREF,
        METHODREF,
        INTERFACE_METHODREF,
    };

    struct Constant {
        uint8 tag;
    };

    /* 数字 */
    struct IntegerConstant : Constant {
        jint value;
    };

    struct FloatConstant : Constant {
        jfloat value;
    };

    struct LongConstant : Constant {
        jlong value;
    };

    struct DoubleConstant : Constant {
        jdouble value;
    };

    struct StringConstant : Constant {
        string value;
    };

    /* 符号引用 */
    // in ref/*

    class Clazz;
    class ConstantPool : public enable_shared_from_this<ConstantPool> {
    public:
        vector<shared_ptr<Constant>> constants;
        shared_ptr<Clazz> clazz; // !ref
    public:
        ConstantPool() = default;
        shared_ptr<ConstantPool> init(shared_ptr<Clazz>, vector<shared_ptr<classfile::cp_info>>);
    };
}

#endif //JVM_RUNTIME_CONSTANT_H
