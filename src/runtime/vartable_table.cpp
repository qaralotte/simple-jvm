#include "include/runtime/vartable_table.h"
#include "include/log.h"
#include "include/runtime/metaspace/object.h"

using namespace runtime;

uint VariableTable::getCapacity() const {
    return capacity;
}

vector<Slot> VariableTable::getSlots() const {
    return slots;
}

/* getter */
template<typename T> T VariableTable::get(uint) {
    ERROR("impossible reach");
    exit(0);
}

template<> jint VariableTable::get<jint>(uint index) {
    return (jint) slots[index].value;
}

template<> jlong VariableTable::get<jlong>(uint index) {
    uint low = get<jint>(index);
    uint high = get<jint>(index + 1);
    return (((jlong) (high) << 32) | (jlong) low);
}

template<> jfloat VariableTable::get<jfloat>(uint index) {
    return *(jfloat *)(&slots[index].value);
}

template<> jdouble VariableTable::get<jdouble>(uint index) {
    auto tmp = get<jlong>(index);
    return *(jdouble *)(&tmp);
}

template<> jobject VariableTable::get<jobject>(uint index) {
    return slots[index].obj;
}

/* setter */
template<typename T> void VariableTable::set(uint, T) {
    ERROR("impossible reach");
    exit(0);
}

template<> void VariableTable::set<jint>(uint index, jint value) {
    slots[index].value = value;
}

template<> void VariableTable::set<jlong>(uint index, jlong value) {
    set<jint>(index, (jint) value);
    set<jint>(index + 1, (jint) (value >> 32));
}

template<> void VariableTable::set<jfloat>(uint index, jfloat value) {
    set<jint>(index, *(jint *)(&value));
}

template<> void VariableTable::set<jdouble>(uint index, jdouble value) {
    auto tmp = *(jlong *)(&value);
    set<jlong>(index, tmp);
}

template<> void VariableTable::set<jobject>(uint index, jobject obj) {
    slots[index].obj = obj;
}