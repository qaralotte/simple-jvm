//
// Created by qaralotte on 2020/12/12.
//

#ifndef SIMPLE_JVM_FIELD_H
#define SIMPLE_JVM_FIELD_H

#include "include/type.h"

namespace classfile {
    struct field_info {
        ushort access_flags;
        ushort name_index;
        ushort descriptor_index;
        ushort attributes_length;
        // todo attributes
    };
}

#endif //SIMPLE_JVM_FIELD_H
