#ifndef JVM_RUNTIME_REF_FIELD_H
#define JVM_RUNTIME_REF_FIELD_H

#include <string>
#include <vector>

#include "include/std.h"

#include "include/runtime/metaspace/clazz.h"
#include "include/runtime/metaspace/constant.h"
#include "include/runtime/metaspace/field.h"

namespace runtime {
    struct FieldRef : Constant {
    public:
        jstring class_name;
        jstring name;
        jstring descriptor;
        ConstantPool *constant_pool;
        Field *field;
        Clazz *clazz;
    public:
        FieldRef(ConstantPool *, const vector<shared_ptr<classfile::cp_info>> &, shared_ptr<classfile::Field>);
    };
}

#endif //JVM_RUNTIME_REF_FIELD_H
