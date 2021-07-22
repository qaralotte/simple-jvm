#ifndef JVM_CLASSFILE_METHOD_H
#define JVM_CLASSFILE_METHOD_H

#include <vector>

#include "std.h"
#include "include/std.h"

#include "attribute.h"

namespace classfile {
    struct method_info {
        u2 access_flags;
        string name;
        string descriptor;
        vector<shared_ptr<attribute_info>> attributes;
    };
}

#endif //JVM_CLASSFILE_METHOD_H
