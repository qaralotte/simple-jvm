#ifndef JVM_RUNTIME_SLOT_H
#define JVM_RUNTIME_SLOT_H

#include "heap/object.h"

namespace runtime {
    struct Slot {
        int value;
        shared_ptr<heap::Object> obj;
    };
}

#endif //JVM_RUNTIME_SLOT_H
