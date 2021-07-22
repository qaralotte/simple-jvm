#ifndef JVM_RUNTIME_REF_CLAZZ_H
#define JVM_RUNTIME_REF_CLAZZ_H

#include <string>
#include <vector>

#include "include/std.h"

#include "include/runtime/metaspace/clazz.h"
#include "include/runtime/metaspace/constant.h"

namespace runtime {
    struct ClassRef : Constant {
    public:
        string class_name;
        shared_ptr<ConstantPool> constant_pool; // !ref
        shared_ptr<Clazz> clazz; // !ref
    public:
        ClassRef(shared_ptr<ConstantPool>, const vector<shared_ptr<classfile::cp_info>> &, shared_ptr<classfile::Class>);
        shared_ptr<Clazz> resolvedClass();
    };
}

#endif //JVM_RUNTIME_REF_CLAZZ_H
