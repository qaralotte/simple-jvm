//
// Created by qaralotte on 2020/12/12.
//

#include <cstdlib>

#include "include/jvm/engine/engine.h"
#include "include/log.h"

string Engine::jdk_path;
string Engine::ext_path;
string Engine::current_path;

void Engine::init() {
#ifdef PROJECT_SOURCE_DIR
#define _STR(DIR) _TOSTR(DIR)
#define _TOSTR(DIR) #DIR
    current_path = _STR(PROJECT_SOURCE_DIR);
    jdk_path = current_path + "/jre/lib";
    ext_path = jdk_path + "/ext";
    DEBUG("执行 Engine::init");
#else
    ERROR("找不到PROJECT_SOURCE_DIR环境变量, 请用CMake 3.10及以上版本进行编译");
#endif
}

classfile::ClassFile Engine::load(const string &clazz) {
    ClassLoader loader(clazz);
    return loader.load();
}