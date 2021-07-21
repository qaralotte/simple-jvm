#include "include/runtime/metaspace/object.h"
#include "include/runtime/metaspace/clazz.h"

using namespace runtime;

shared_ptr<Object> Object::newMultiDimensionArray(vector<jint> lengths, shared_ptr<Clazz> clazz) {
    auto length = lengths[0];
    auto array = clazz -> initArray(length);
    if (lengths.size() > 1) {
        auto &data = array -> data;
        for (int i = 0; i < data.size(); ++i) {
            data[i] = newMultiDimensionArray(vector<jint>(lengths.begin() + 1, lengths.end()), clazz -> getComponentClass());
        }
    }
    return array;
}

bool Object::isInstanceOf(Clazz d) {
    return d.isAssignFrom(*clazz);
}

#pragma clang diagnostic ignored "-Wtautological-undefined-compare"
bool Object::operator==(const Object &other) const {
    if (this == nullptr && &other == nullptr) return true;
    if (this == nullptr || &other == nullptr) return false;
    if (*clazz != *other.clazz) return false;
    if (field.getSlots() != other.field.getSlots()) return false;
    return true;
}

bool Object::operator!=(const Object &other) const {
    return !operator==(other);
}
