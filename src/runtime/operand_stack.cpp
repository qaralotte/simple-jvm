#include "include/runtime/operand_stack.h"
#include "include/log.h"

using namespace runtime;

OperandStack::OperandStack(uint _capacity) : capacity(_capacity), size(0) {
    stack.resize(_capacity);
}

runtime::jobject OperandStack::getRegFromTop(uint n) {
    return stack[size - 1 - n].obj;
}

/* push */
template<typename T> void OperandStack::push(T) {
    ERROR("impossible reach");
    exit(0);
}

template<> void OperandStack::push<jint>(jint value) {
    if (size < 0 && size >= capacity) {
        ERROR("size (%u) > OperandStack.capacity (%u)", size, capacity);
        exit(0);
    }
    Slot slot;
    slot.value = value;
    stack[size] = slot;
    size += 1;
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
    if (size < 0 && size >= capacity) {
        ERROR("size (%u) > OperandStack.capacity (%u)", size, capacity);
        exit(0);
    }
    Slot slot;
    slot.obj = obj;
    stack[size] = slot;
    size += 1;
}

template<> void OperandStack::push<Slot>(Slot slot) {
    if (size < 0 && size >= capacity) {
        ERROR("size (%u) > OperandStack.capacity (%u)", size, capacity);
        exit(0);
    }
    stack[size] = slot;
    size += 1;
}

/* pop */
template<typename T> T OperandStack::pop() {
    ERROR("impossible reach");
    exit(0);
}

template<> jint OperandStack::pop<jint>() {
    if (size < 0 && size >= capacity) {
        ERROR("size (%u) > OperandStack.capacity (%u)", size, capacity);
        exit(0);
    }
    size -= 1;
    auto value = stack[size].value;
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
    if (size < 0 && size >= capacity) {
        ERROR("size (%u) > OperandStack.capacity (%u)", size, capacity);
        exit(0);
    }
    size -= 1;
    auto obj = stack[size].obj;
    return obj;
}

template<> Slot OperandStack::pop<Slot>() {
    if (size < 0 && size >= capacity) {
        ERROR("size (%u) > OperandStack.capacity (%u)", size, capacity);
        exit(0);
    }
    size -= 1;
    auto slot = stack[size];
    return slot;
}
