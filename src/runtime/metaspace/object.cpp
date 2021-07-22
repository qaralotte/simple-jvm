#include "include/runtime/metaspace/object.h"
#include "include/runtime/metaspace/clazz.h"
#include "include/accessflags.h"

using namespace runtime;

shared_ptr<Object> Object::newMultiDimensionArray(vector<jint> lengths, shared_ptr<Clazz> clazz) {
    auto length = lengths[0];
    auto array = clazz -> initArray(length);
    if (lengths.size() > 1) {
        auto &data = array -> array_data;
        for (int i = 0; i < data.size(); ++i) {
            data[i] = newMultiDimensionArray(vector<jint>(lengths.begin() + 1, lengths.end()), clazz -> getComponentClass());
        }
    }
    return array;
}

bool Object::isInstanceOf(Clazz d) {
    return d.isAssignFrom(*clazz);
}

void Object::setRefVal(string name, string descriptor, shared_ptr<Object> ref) {
    auto field = clazz -> findField(name, descriptor);
    single_data.set<runtime::jobject>(field -> slot_id, ref);
}

shared_ptr<Object> Object::getRefVal(string name, string descriptor) {
    auto field = clazz -> findField(name, descriptor);
    return single_data.get<runtime::jobject>(field -> slot_id);
}

#pragma clang diagnostic ignored "-Wtautological-undefined-compare"
bool Object::operator==(const Object &other) const {
    if (this == nullptr && &other == nullptr) return true;
    if (this == nullptr || &other == nullptr) return false;
    if (*clazz != *other.clazz) return false;
    if (single_data.getSlots() != other.single_data.getSlots()) return false;
    return true;
}

bool Object::operator!=(const Object &other) const {
    return !operator==(other);
}
