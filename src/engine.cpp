#include <vector>

#include "include/classfile/reader.h"

#include "include/cmd.h"
#include "include/log.h"

/* 启动虚拟机 */
void start() {
    DEBUG("开始启动 JVM");
    classfile::ClassReader reader("Main");
    reader.read();
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
