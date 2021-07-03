#ifndef JVM_RUNTIME_THREAD_H
#define JVM_RUNTIME_THREAD_H

#include <stack>
#include <memory>

#include "include/std.h"

#include "local_vartable.h"
#include "operand_stack.h"

namespace runtime {

    class JVMThread;
    class JVMFrame {
    public:
        JVMThread *self_thread;
        LocalVariableTable locals;
        OperandStack stack;
        // todo 动态连接 Dynamic Linking
        // todo 返回地址 Return Address
    public:
        JVMFrame(JVMThread *, uint max_locals, uint max_stack);
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
        void setPC(uint);
        uint getPC();
        JVMStack &getStack();
    };
}

#endif //JVM_RUNTIME_THREAD_H
