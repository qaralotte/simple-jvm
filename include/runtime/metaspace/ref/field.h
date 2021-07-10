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
        shared_ptr<ConstantPool> constant_pool; // !ref
        shared_ptr<Field> field; // !ref
        shared_ptr<Clazz> clazz; // !ref
    private:
        shared_ptr<Field> lookupField(shared_ptr<Clazz>, string, string);
    public:
        FieldRef(shared_ptr<ConstantPool>, const vector<shared_ptr<classfile::cp_info>> &, shared_ptr<classfile::Field>);
        shared_ptr<Clazz> resolvedClass();
        shared_ptr<Field> resolvedField();
    };
}

#endif //JVM_RUNTIME_REF_FIELD_H
