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
        shared_ptr<ConstantPool> constant_pool;
        shared_ptr<Method> method;
        shared_ptr<Clazz> clazz;
    public:
        MethodRef(shared_ptr<ConstantPool>, const vector<shared_ptr<classfile::cp_info>> &, shared_ptr<classfile::Method>);
        shared_ptr<Clazz> resolvedClass();
    };
}

#endif //JVM_RUNTIME_REF_METHOD_H
