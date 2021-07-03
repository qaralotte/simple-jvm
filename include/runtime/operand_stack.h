#ifndef JVM_RUNTIME_OPERAND_STACK_H
#define JVM_RUNTIME_OPERAND_STACK_H

#include <memory>
#include <stack>

#include "include/std.h"

#include "slot.h"
#include "heap/object.h"

namespace runtime {
    class OperandStack {
    private:
        uint capacity;
        stack<Slot> stack;
    public:
        OperandStack() = default;
        explicit OperandStack(uint _capacity) : capacity(_capacity) {};
        uint getCapacity();
    public:
        template<typename T> void push(T);
        template<typename T> T pop();
    };
}

#endif //JVM_RUNTIME_OPERAND_STACK_H
