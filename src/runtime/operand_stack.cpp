#include "include/runtime/operand_stack.h"
#include "include/log.h"

using namespace runtime;

uint OperandStack::getCapacity() {
    return capacity;
}

/* push */
template<typename T> void OperandStack::push(T) {
    ERROR("impossible reach");
    exit(0);
}

template<> void OperandStack::push<jint>(jint value) {
    stack.push(Slot());
    stack.top().value = value;
}

template<> void OperandStack::push<jbyte>(jbyte value) {
    push<jint>((jint) value);
}

template<> void OperandStack::push<jshort>(jshort value) {
    push<jint>((jshort) value);
}

template<> void OperandStack::push<jchar>(jchar value) {
    push<jint>((jint) value);
}

template<> void OperandStack::push<jlong>(jlong value) {
    push<jint>((jint) value);
    push<jint>((jint) (value >> 32));
}


template<> void OperandStack::push<jfloat>(jfloat value) {
    push<jint>(*(jint *)(&value));
}

template<> void OperandStack::push<jdouble>(jdouble value) {
    auto tmp = *(jlong *)(&value);
    push<jlong>(tmp);
}

template<> void OperandStack::push<jobject>(jobject obj) {
    stack.push(Slot());
    stack.top().obj = obj;
}

template<> void OperandStack::push<Slot>(Slot slot) {
    stack.push(slot);
}

/* pop */
template<typename T> T OperandStack::pop() {
    ERROR("impossible reach");
    exit(0);
}

template<> jint OperandStack::pop<jint>() {
    auto value = stack.top().value;
    stack.pop();
    return (jint) value;
}

template<> jbyte OperandStack::pop<jbyte>() {
    return (jbyte) pop<jint>();
}

template<> jshort OperandStack::pop<jshort>() {
    return (jshort) pop<jint>();
}

template<> jchar OperandStack::pop<jchar>() {
    return (jchar) pop<jint>();
}

template<> jlong OperandStack::pop<jlong>() {
    uint high = pop<jint>();
    uint low = pop<jint>();
    return (((jlong) (high) << 32) | (jlong) low);
}

template<> jfloat OperandStack::pop<jfloat>() {
    auto value = pop<jint>();
    return *(jfloat *)(&value);
}

template<> jdouble OperandStack::pop<jdouble>() {
    auto value = pop<jlong>();
    return *(jdouble *)(&value);
}

template<> jobject OperandStack::pop<jobject>() {
    auto obj = stack.top().obj;
    stack.pop();
    return obj;
}

template<> Slot OperandStack::pop<Slot>() {
    auto slot = stack.top();
    stack.pop();
    return slot;
}
