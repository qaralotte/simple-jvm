#include "include/runtime/metaspace/object.h"
#include "include/runtime/metaspace/clazz.h"

using namespace runtime;

bool Object::isInstanceOf(Clazz d) {
    return d.isAssignFrom(clazz);
}

bool Object::operator==(const Object &other) const {
    if (clazz != other.clazz) return false;
    if (field.getSlots() != other.field.getSlots()) return false;
    return true;
}

bool Object::operator!=(const Object &other) const {
    return !operator==(other);
}
