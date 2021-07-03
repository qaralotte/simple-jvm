#ifndef JVM_RUNTIME_REF_METHOD_H
#define JVM_RUNTIME_REF_METHOD_H

#include <string>
#include <vector>

#include "include/std.h"

#include "include/runtime/metaspace/clazz.h"
#include "include/runtime/metaspace/constant.h"
#include "include/runtime/metaspace/method.h"

namespace runtime {
    struct MethodRef : Constant {
    public:
        jstring class_name;
        jstring name;
        jstring descriptor;
        ConstantPool *constant_pool;
        Method *method;
        Clazz *clazz;
    public:
        MethodRef(ConstantPool *, const vector<shared_ptr<classfile::cp_info>> &, shared_ptr<classfile::Method>);
    };
}

#endif //JVM_RUNTIME_REF_METHOD_H
