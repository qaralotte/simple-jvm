#include "include/runtime/vartable_table.h"
#include "include/log.h"
#include "include/runtime/metaspace/object.h"

using namespace runtime;

vector<Slot> VariableTable::getSlots() const {
    return slots;
}

/* getter */
template<typename T> T VariableTable::get(uint) {
    ERROR("impossible reach");
    exit(0);
}

template<> jint VariableTable::get<jint>(uint index) {
    if (index < 0 && index >= slots.size()) {
        ERROR("index (%u) > VariableTable.size (%d)", index, slots.size());
        exit(0);
    }
    return (jint) slots[index].value;
}

template<> jlong VariableTable::get<jlong>(uint index) {
    uint low = get<jint>(index);
    uint high = get<jint>(index + 1);
    return (((jlong) (high) << 32) | (jlong) low);
}

template<> jfloat VariableTable::get<jfloat>(uint index) {
    if (index < 0 && index >= slots.size()) {
        ERROR("index (%u) > VariableTable.size (%d)", index, slots.size());
        exit(0);
    }
    return *(jfloat *)(&slots[index].value);
}

template<> jdouble VariableTable::get<jdouble>(uint index) {
    auto tmp = get<jlong>(index);
    return *(jdouble *)(&tmp);
}

template<> jobject VariableTable::get<jobject>(uint index) {
    if (index < 0 && index >= slots.size()) {
        ERROR("index (%u) > VariableTable.size (%d)", index, slots.size());
        exit(0);
    }
    return slots[index].obj;
}

template<> Slot VariableTable::get<Slot>(uint index) {
    if (index < 0 && index >= slots.size()) {
        ERROR("index (%u) > VariableTable.size (%d)", index, slots.size());
        exit(0);
    }
    return slots[index];
}

/* setter */
template<typename T> void VariableTable::set(uint, T) {
    ERROR("impossible reach");
    exit(0);
}

template<> void VariableTable::set<jint>(uint index, jint value) {
    if (index < 0 && index >= slots.size()) {
        ERROR("index (%u) > VariableTable.size (%d)", index, slots.size());
        exit(0);
    }
    if (mut && capacity <= index) slots.resize(index + 1);
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
    if (mut && capacity <= index) slots.resize(index + 1);
    if (index < 0 && index >= slots.size()) {
        ERROR("index (%u) > VariableTable.size (%d)", index, slots.size());
        exit(0);
    }
    slots[index].obj = obj;
}

template<> void VariableTable::set<Slot>(uint index, Slot slot) {
    if (mut && capacity <= index) slots.resize(index + 1);
    if (index < 0 && index >= slots.size()) {
        ERROR("index (%u) > VariableTable.size (%d)", index, slots.size());
        exit(0);
    }
    slots[index] = slot;
}