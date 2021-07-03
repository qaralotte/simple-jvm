#include <exception>

#include "include/runtime/thread.h"

#include "include/cmd.h"
#include "include/log.h"

using namespace runtime;

/* 栈帧 */
JVMFrame::JVMFrame(JVMThread *thread, uint max_locals, uint max_stack) {
    self_thread = thread;
    locals = LocalVariableTable(max_locals);
    stack = OperandStack(max_stack);
}

/* 栈 */
JVMStack::JVMStack(uint _capacity) {
    capaciry = (_capacity == 0 ? UINT32_MAX : _capacity);
    size = 0;
}

void JVMStack::push(JVMFrame frame) {
    if (size > capaciry) {
        ERROR("StackOverflowError: size=%u, capacity=%u", size, capaciry);
        exit(0);
    }
    frames.push(frame);
    size += 1;
}

JVMFrame JVMStack::pop() {
    if (size <= 0) {
        ERROR("OutOfMemoryError: size < 0");
        exit(0);
    }
    auto frame = frames.top();
    frames.pop();
    size -= 1;
    return frame;
}

/* 线程 */
JVMThread::JVMThread() {
    pc = 0;
    stack = JVMStack(Cmd::Xss);
}

void JVMThread::setPC(uint _pc) {
    pc = _pc;
}

uint JVMThread::getPC() {
    return pc;
}

JVMStack &JVMThread::getStack() {
    return stack;
}