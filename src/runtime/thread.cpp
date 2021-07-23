#include <exception>

#include "include/runtime/thread.h"

#include "include/cmd.h"
#include "include/log.h"

using namespace runtime;

ulong JVMThread::current_stack_memory = 0;

/* 栈帧 */
JVMFrame::JVMFrame(shared_ptr<JVMThread> _thread, shared_ptr<Method> _method) {
    locals = VariableTable(_method -> max_locals);
    stack = OperandStack(_method -> max_stack);
    method = _method;
    thread = _thread;
    pc = 0;
}

void JVMFrame::setPC(uint _pc) {
    pc = _pc;
}

uint JVMFrame::getPC() {
    return pc;
}

/* 线程 */
JVMThread::JVMThread() {
    size = 0;
}

shared_ptr<JVMThread> JVMThread::init() {
    return make_shared<JVMThread>();
}

void JVMThread::setPC(uint _pc) {
    top().pc = _pc;
}

uint JVMThread::getPC() {
    return top().pc;
}

void JVMThread::push(JVMFrame frame) {
    current_stack_memory += sizeof(frame);
    JVMThread::checkStackOverflow();
    frames.push(frame);
    size += 1;
}

JVMFrame JVMThread::pop() {
    if (size <= 0) {
        ERROR("OutOfMemoryError: size < 0");
        exit(0);
    }
    auto frame = frames.top();
    frames.pop();
    size -= 1;
    return frame;
}

JVMFrame &JVMThread::top() {
    return frames.top();
}

bool JVMThread::isEmpty() const {
    return frames.empty();
}

void JVMThread::checkStackOverflow() {
    if (current_stack_memory > Cmd::Xss) {
        ERROR("StackOverflowError: current=%llu, Xss=%llu", current_stack_memory, Cmd::Xss);
        exit(0);
    }
}