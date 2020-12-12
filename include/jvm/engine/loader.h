//
// Created by qaralotte on 2020/12/12.
//

#ifndef SIMPLE_JVM_LOADER_H
#define SIMPLE_JVM_LOADER_H

#include <string>
#include <vector>

#include "include/type.h"
#include "include/jvm/classfile/constant.h"

namespace classfile {
    struct ClassFile;
}

class ClassLoader {
private:
    vector<ubyte> bytes;
    ulong cursor;
private:
    void next();
    ubyte readU1();
    ushort readU2();
    uint readU4();
private:
    classfile::constant::cp_info *readConstant();
public:
    explicit ClassLoader(const string &);
    classfile::ClassFile load();
public:
    static void release(classfile::ClassFile &);
};


#endif //SIMPLE_JVM_LOADER_H
