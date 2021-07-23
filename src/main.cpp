#include <vector>
#include <ctime>

#include "include/cmd.h"
#include "include/log.h"
#include "include/finder.h"
#include "include/libzip/zip.h"

#include "include/opcode/interpreter.h"
#include "include/runtime/thread.h"
#include "include/runtime/loader.h"
#include "include/runtime/metaspace/method.h"

void loop(shared_ptr<runtime::JVMThread> thread) {

    while (true) {
        auto &frame = thread -> top();
        auto reader = opcode::Reader(thread, frame.method -> code, frame.getPC());
        auto interpreter = opcode::Interpreter(frame, reader);

        auto opcode = reader.readByte();
        interpreter.execute(opcode);
        frame.setPC(reader.getPC());

        if (thread -> isEmpty()) break;
    }
}

void interpret(shared_ptr<runtime::Method> method) {
    auto thread = runtime::JVMThread().init();
    runtime::JVMFrame frame(thread, method);
    frame.locals.set(0, Cmd::handleJargs()); // args
    thread -> push(frame);
    loop(thread);
}

void showHelp() {

}

/* 启动虚拟机 */
void startJVM() {
    DEBUG("启动 JVM");
    if (!Cmd::isSet("main")) {
        showHelp();
        return;
    }
    auto clazz = runtime::ClassLoader(Cmd::main).loadClass();
    auto main_method = clazz -> getMainMethod();
    if (main_method != nullptr) {
        interpret(main_method);
    } else {
        ERROR("在%s 未找到main方法", clazz -> this_name.c_str());
        exit(0);
    }
}

/* 卸载 JVM */
void destroyJVM() {
    DEBUG("卸载 JVM");
    for (auto uzf : classfinder::ClassFinder::unz_jars) {
        zip_close(uzf.second);
    }
}


int main(int argc, char *argv[]) {

    /* 处理命令行参数 */
    vector<string> cmd_vec;
    for (int i = 1; i < argc; ++i) {
        cmd_vec.push_back(string(argv[i]));
    }
    Cmd::config(cmd_vec);

    clock_t startTime, endTime;
    startTime = clock();

    /* 启动 JVM */
    startJVM();

    /* 卸载 JVM */
    destroyJVM();

    endTime = clock();
    DEBUG("运行时长: %lfms", (double)(endTime - startTime) / CLOCKS_PER_SEC * 1000);

    return 0;
}
