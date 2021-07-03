#include "include/runtime/local_vartable.h"
#include "include/log.h"

using namespace runtime;

uint LocalVariableTable::getCapacity() {
    return capacity;
}

/* getter */
template<typename T> T LocalVariableTable::get(uint) {
    ERROR("impossible reach");
    exit(0);
}

template<> jint LocalVariableTable::get<jint>(uint index) {
    return (jint) slots[index].value;
}

template<> jlong LocalVariableTable::get<jlong>(uint index) {
    uint low = get<jint>(index);
    uint high = get<jint>(index + 1);
    return (((jlong) (high) << 32) | (jlong) low);
}

template<> jfloat LocalVariableTable::get<jfloat>(uint index) {
    return *(jfloat *)(&slots[index].value);
}

template<> jdouble LocalVariableTable::get<jdouble>(uint index) {
    auto tmp = get<jlong>(index);
    return *(jdouble *)(&tmp);
}

template<> shared_ptr<heap::Object> LocalVariableTable::get<shared_ptr<heap::Object>>(uint index) {
    return slots[index].obj;
}

/* setter */
template<> void LocalVariableTable::set<jint>(uint index, jint value) {
    slots[index].value = value;
}

template<> void LocalVariableTable::set<jlong>(uint index, jlong value) {
    set<jint>(index, (jint) value);
    set<jint>(index + 1, (jint) (value >> 32));
}

template<> void LocalVariableTable::set<jfloat>(uint index, jfloat value) {
    set<jint>(index, *(jint *)(&value));
}

template<> void LocalVariableTable::set<jdouble>(uint index, jdouble value) {
    auto tmp = *(jlong *)(&value);
    set<jlong>(index, tmp);
}

template<> void LocalVariableTable::set<shared_ptr<heap::Object>>(uint index, shared_ptr<heap::Object> obj) {
    slots[index].obj = obj;
}