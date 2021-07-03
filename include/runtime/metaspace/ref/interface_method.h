#ifndef JVM_INTERFACE_METHOD_H
#define JVM_INTERFACE_METHOD_H

#include <string>
#include <vector>

#include "include/std.h"

#include "include/runtime/metaspace/clazz.h"
#include "include/runtime/metaspace/constant.h"
#include "include/runtime/metaspace/method.h"

namespace runtime {
    struct InterfaceMethodRef : Constant {
    public:
        jstring class_name;
        jstring name;
        jstring descriptor;
        ConstantPool *constant_pool;
        Method *method;
        Clazz *clazz;
    public:
        InterfaceMethodRef(ConstantPool *, const vector<shared_ptr<classfile::cp_info>> &, shared_ptr<classfile::InterfaceMethodref>);
    };
}

#endif //JVM_INTERFACE_METHOD_H
