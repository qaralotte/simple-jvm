#ifndef JVM_CLASSFILE_LOADER_H
#define JVM_CLASSFILE_LOADER_H

#include <memory>
#include <optional>

#include "classfile.h"
#include "constant.h"
#include "field.h"
#include "method.h"
#include "attribute.h"

#include "include/finder.h"

namespace classfile {
    class ClassLoader {
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
        ClassLoader(ClassPath);
        ClassFile load();
        optional<method_info> getMain();
    };
}

#endif //JVM_CLASSFILE_LOADER_H
