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
        // todo 动态连接 Dynamic Linking
        // todo 返回地址 Return Address
        shared_ptr<Method> method; // !ref
        shared_ptr<JVMThread> thread; // !ref
    public:
        JVMFrame(shared_ptr<JVMThread>, shared_ptr<Method>);
    };

    class JVMStack {
    private:
        uint capaciry;
        uint size;
        stack<JVMFrame> frames;
    public:
        JVMStack(uint = 0);
        void push(JVMFrame);
        JVMFrame pop();
    };

    class JVMThread {
    private:
        JVMStack stack;
        uint pc;
    public:
        JVMThread();
        shared_ptr<JVMThread> init();
        void setPC(uint);
        uint getPC();
        JVMStack &getStack();
    };
}

#endif //JVM_RUNTIME_THREAD_H
