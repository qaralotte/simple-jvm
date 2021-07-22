#ifndef JVM_CLASSFILE_CONSTANT_H
#define JVM_CLASSFILE_CONSTANT_H

#include <vector>
#include <memory>

#include "std.h"
#include "include/std.h"

namespace classfile {
    enum Constant {
        UTF8 = 1,
        INTEGER = 3,
        FLOAT,
        LONG,
        DOUBLE,
        CLASS,
        STRING,
        FIELDREF,
        METHODREF,
        INTERFACE_METHODREF,
        NAME_AND_TYPE,
        METHOD_HANDLE = 15,
        METHOD_TYPE,
        INVOKE_DYNAMIC = 18
    };

    struct cp_info {
        u1 tag;
    };

    struct Class : cp_info {
        u2 name_index;
    };

    struct Field : cp_info {
        u2 class_index;
        u2 name_and_type_index;
    };

    struct Method : cp_info {
        u2 class_index;
        u2 name_and_type_index;
    };

    struct InterfaceMethodref : cp_info {
        u2 class_index;
        u2 name_and_type_index;
    };

    struct String : cp_info {
        u2 string_index;
    };

    struct Integer : cp_info {
        jint value;
    };

    struct Float : cp_info {
        jfloat value;
    };

    struct Long : cp_info {
        jlong value;
    };

    struct Double : cp_info {
        jdouble value;
    };

    struct NameAndType : cp_info {
        u2 name_index;
        u2 descriptor_index;
    };

    struct Utf8 : cp_info {
        string value;
    };

    struct MethodHandle : cp_info {
        u1 reference_kind;
        u2 reference_index;
    };

    struct MethodType : cp_info {
        u2 descriptor_index;
    };

    struct InvokeDynamic : cp_info {
        u2 bootstrap_method_attr_index;
        u2 name_and_type_index;
    };

    class ConstantUtils {
    public:
        // 转换不同的constant类型
        template<class T>
        static shared_ptr<T> getConstant(const vector<shared_ptr<cp_info>> &pool, uint index) {
            return static_pointer_cast<T>(pool[index]);
        }
    };
}


#endif //JVM_CLASSFILE_CONSTANT_H
