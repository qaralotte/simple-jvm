//
// Created by qaralotte on 2020/12/12.
//

#ifndef SIMPLE_JVM_CLASSFILE_H
#define SIMPLE_JVM_CLASSFILE_H

#include <vector>

#include "include/type.h"
#include "include/jvm/engine/loader.h"

#include "constant.h"

#define JVM_SUPPORT_VERSION 52

namespace classfile {
    struct ClassFile {
        ulong magic;
        ushort minor_version;
        ushort major_version;
        ushort constant_pool_count;
        vector<constant::cp_info *> constant_pool;
    };
}

#endif //SIMPLE_JVM_CLASSFILE_H
