#ifndef JVM_RUNTIME_OPERAND_STACK_H
#define JVM_RUNTIME_OPERAND_STACK_H

#include <memory>
#include <vector>

#include "include/std.h"

#include "slot.h"
#include "metaspace/object.h"

namespace runtime {
    class OperandStack {
    private:
        uint capacity;
        vector<Slot> stack;
        uint size;
    public:
        OperandStack() = default;
        explicit OperandStack(uint);
        runtime::jobject getRegFromTop(uint);
    public:
        template<typename T> void push(T);
        template<typename T> T pop();
    };
}

#endif //JVM_RUNTIME_OPERAND_STACK_H
