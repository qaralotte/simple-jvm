#ifndef JVM_CLASSFILE_CLASSFILE_H
#define JVM_CLASSFILE_CLASSFILE_H

#include <vector>
#include <memory>

#include "std.h"
#include "include/std.h"

#include "constant.h"
#include "field.h"
#include "method.h"
#include "attribute.h"

namespace classfile {
    struct ClassFile {
        vector<shared_ptr<cp_info>> constant_pool;
        u2 access_flags;
        string this_class;
        string super_class;
        vector<string> interfaces;
        vector<field_info> fields;
        vector<method_info> methods;
        vector<shared_ptr<attribute_info>> attributes;
    };
}

#endif //JVM_CLASSFILE_CLASSFILE_H
