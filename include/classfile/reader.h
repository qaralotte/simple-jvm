#ifndef JVM_CLASSFILE_READER_H
#define JVM_CLASSFILE_READER_H

#include <memory>

#include "classfile.h"
#include "constant.h"
#include "field.h"
#include "method.h"
#include "attribute.h"

#include "include/finder.h"

namespace classfile {
    class ClassReader {
    private:
        string class_name;
        vector<u1> bytes;
        uint offset = 0;
        ClassFile clazz;
    private:
        u1 readU1();
        u2 readU2();
        u4 readU4();
    private:
        shared_ptr<cp_info> readConstant(); // impl in constant.cpp
        jstring readClass();
        field_info readField();
        method_info readMethod();
        shared_ptr<attribute_info> readAttribute(); // impl in attribute.cpp
    public:
        static string jdk_path;
    public:
        ClassReader(const string &);
        ClassFile read();
    };
}

#endif //JVM_CLASSFILE_READER_H
