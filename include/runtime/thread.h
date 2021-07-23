#ifndef JVM_RUNTIME_THREAD_H
#define JVM_RUNTIME_THREAD_H

#include <stack>
#include <memory>

#include "include/std.h"

#include "vartable_table.h"
#include "operand_stack.h"

namespace runtime {

    class JVMThread;
    class JVMFrame {
    public:
        VariableTable locals;
        OperandStack stack;
        uint pc;
        shared_ptr<Method> method; // !!ref
        shared_ptr<JVMThread> thread; // !!ref
    public:
        JVMFrame(shared_ptr<JVMThread>, shared_ptr<Method>);
        void setPC(uint);
        uint getPC();
    };

    class JVMThread {
    private:
        uint size;
        stack<JVMFrame> frames;
    public:
        static ulong current_stack_memory;
    public:
        JVMThread();
        shared_ptr<JVMThread> init();
        void setPC(uint);
        uint getPC();
        void push(JVMFrame);
        JVMFrame pop();
        JVMFrame &top();
        bool isEmpty() const;

    public:
        static void checkStackOverflow();
    };
}

#endif //JVM_RUNTIME_THREAD_H
