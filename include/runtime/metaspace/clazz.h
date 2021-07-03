#ifndef JVM_RUNTIME_CLAZZ_H
#define JVM_RUNTIME_CLAZZ_H

#include <string>
#include <vector>
#include <memory>

#include "include/std.h"
#include "include/classfile/classfile.h"

#include "field.h"
#include "method.h"
#include "constant.h"

#include "include/runtime/loader.h"
#include "include/runtime/slot.h"

namespace runtime {
    class Clazz {
    public:
        uint16 access_flags;
        jstring this_name;
        jstring super_name;
        vector<jstring> interfaces_name;
        vector<Field> fields;
        vector<Method> methods;
        ConstantPool constant_pool;
        ClassLoader *loader;
        shared_ptr<Clazz> super_class;
        vector<Clazz> interfaces;
        uint instance_slot_count;
        uint static_slot_count;
        Slot static_vars;
    public:
        Clazz() = default;
        Clazz(classfile::ClassFile);
        bool haveAccess(uint16);
    };
}


#endif //JVM_RUNTIME_CLAZZ_H
