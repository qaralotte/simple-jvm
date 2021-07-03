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
        jstring class_name;
        ConstantPool *constant_pool;
        Clazz *clazz;
    public:
        ClassRef(ConstantPool *, const vector<shared_ptr<classfile::cp_info>> &, shared_ptr<classfile::Class>);
    };
}

#endif //JVM_RUNTIME_REF_CLAZZ_H
