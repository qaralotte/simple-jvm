#include "include/runtime/metaspace/clazz.h"

using namespace runtime;

Clazz::Clazz(classfile::ClassFile clazz) {
    access_flags = clazz.access_flags;
    this_name = clazz.this_class;
    super_name = clazz.super_class;
    interfaces_name = clazz.interfaces;
    fields = Field::arrayOf(this, clazz.fields);
    methods = Method::arrayOf(this, clazz.methods);
    constant_pool = ConstantPool(this, clazz.constant_pool);
}

bool Clazz::haveAccess(uint16 flag) {
    return (access_flags & flag) != 0;
}

