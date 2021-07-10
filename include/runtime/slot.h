#ifndef JVM_RUNTIME_SLOT_H
#define JVM_RUNTIME_SLOT_H

#include "include/std.h"

namespace runtime {

    struct Object;
    struct Slot {
        int value;
        shared_ptr<Object> obj;
        bool operator==(const Slot &other) const;
        bool operator!=(const Slot &other) const;
    };
}

#endif //JVM_RUNTIME_SLOT_H
