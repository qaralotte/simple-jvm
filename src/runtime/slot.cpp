#include "include/runtime/slot.h"
#include "include/runtime/metaspace/object.h"

using namespace runtime;

bool Slot::operator==(const Slot &other) const {
    if (value != other.value) return false;
    if (*obj != *other.obj) return false;
    return true;
}

bool Slot::operator!=(const Slot &other) const {
    return !operator==(other);
}