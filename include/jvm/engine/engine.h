//
// Created by qaralotte on 2020/12/12.
//

#ifndef SIMPLE_JVM_ENGINE_H
#define SIMPLE_JVM_ENGINE_H

#include <string>

#include "include/type.h"
#include "include/jvm/classfile/classfile.h"

class Engine {
private:
    static string jdk_path;
    static string ext_path;
    static string current_path;
public:
    static void init();
    static classfile::ClassFile load(const string &);
};

#endif //SIMPLE_JVM_ENGINE_H
