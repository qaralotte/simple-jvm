#include <vector>
#include <ctime>

#include "include/cmd.h"
#include "include/log.h"

#include "include/opcode/interpreter.h"
#include "include/runtime/thread.h"
#include "include/runtime/loader.h"
#include "include/runtime/metaspace/field.h"
#include "include/runtime/metaspace/method.h"

[[noreturn]] __unused void loop(shared_ptr<runtime::JVMThread> thread, vector<classfile::u1> code) {
    auto frame = thread -> getStack().pop();
    auto reader = opcode::Reader(thread, code);
    auto interpreter = opcode::Interpreter(frame, reader);
    while (true) {
        auto opcode = reader.readByte();
        interpreter.execute(opcode);
        thread -> setPC(reader.getPC());
    }
}

__unused void interpret(shared_ptr<runtime::Method> main_method) {
    auto thread = runtime::JVMThread().init();
    runtime::JVMFrame frame(thread, main_method);
    thread -> getStack().push(frame);
    loop(thread, main_method -> code);
}

/* 启动虚拟机 */
void startJVM() {
    DEBUG("开始启动 JVM");
    auto clazz = runtime::ClassLoader("Main").loadClass();
    auto main_method = clazz -> getMainMethod();
    if (main_method != nullptr) {
        interpret(main_method);
    } else {
        ERROR("在%s 未找到main方法", clazz -> this_name.c_str());
        exit(0);
    }
}


int main(int argc, char *argv[]) {

    /* 处理命令行参数 */
    vector<string> cmd_vec;
    for (int i = 1; i < argc; ++i) {
        cmd_vec.push_back(string(argv[i]));
    }
    Cmd::config(cmd_vec);

    /* 开始启动 JVM */
    startJVM();

    return 0;
}
