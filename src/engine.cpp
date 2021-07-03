#include <vector>

#include "include/cmd.h"
#include "include/log.h"

#include "include/classfile/loader.h"
#include "include/runtime/thread.h"
#include "include/opcode/interpreter.h"

#include "include/runtime/loader.h"
#include "include/runtime/metaspace/clazz.h"

__unused shared_ptr<classfile::attribute_info> findAttributeInMethod(classfile::method_info method, string name) {
    for (auto attribute : method.attributes) {
        if (attribute -> name == name) {
            return attribute;
        }
    }
    return nullptr;
}

[[noreturn]] __unused void test(runtime::JVMThread thread, vector<classfile::u1> code) {
    DEBUG("----- 以下为测试数据 -----");
    auto frame = thread.getStack().pop();
    auto reader = opcode::Reader(&thread, code);
    auto interpreter = opcode::Interpreter(frame, reader);
    while (true) {
        auto opcode = reader.readByte();
        interpreter.execute(opcode);
        thread.setPC(reader.getPC());
    }
}

__unused void test0() {
    DEBUG("----- 以下为测试数据 -----");
    auto rclazz = runtime::ClassLoader::loadClass("Main");

    /*
    auto main = loader.getMain();
    if (main != nullopt) {
        shared_ptr<classfile::Code> code_attr = static_pointer_cast<classfile::Code>(findAttributeInMethod(main.value(), "Code"));
        auto max_locals = code_attr -> max_locals;
        auto max_stack = code_attr -> max_stack;
        auto code = code_attr -> code;

        runtime::JVMThread thread;
        auto frame = runtime::JVMFrame(&thread, max_locals, max_stack);
        thread.getStack().push(frame);

        test(thread, code);
    }
     */
}

/* 启动虚拟机 */
void start() {
    DEBUG("开始启动 JVM");
    test0();
}


int main(int argc, char *argv[]) {

    /* 处理命令行参数 */
    vector<string> cmd_vec;
    for (int i = 1; i < argc; ++i) {
        cmd_vec.push_back(string(argv[i]));
    }
    Cmd::config(cmd_vec);

    start();

    return 0;
}
