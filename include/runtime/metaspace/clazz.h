#ifndef JVM_RUNTIME_CLAZZ_H
#define JVM_RUNTIME_CLAZZ_H

#include <string>
#include <vector>
#include <memory>
#include <optional>

#include "include/std.h"
#include "include/classfile/classfile.h"

#include "field.h"
#include "method.h"
#include "constant.h"

#include "include/runtime/vartable_table.h"

namespace runtime {

    class ClassLoader;
    class Clazz : public enable_shared_from_this<Clazz> {
    public:
        uint16 access_flags;
        jstring this_name;
        jstring super_name;
        vector<jstring> interfaces_name;
        vector<shared_ptr<Field>> fields;
        vector<shared_ptr<Method>> methods;
        shared_ptr<ConstantPool> constant_pool;
        shared_ptr<ClassLoader> loader; // !ref
        shared_ptr<Clazz> super_class; // !ref
        vector<shared_ptr<Clazz>> interfaces;
        uint instance_slot_count;
        uint static_slot_count;
        VariableTable static_vars;
    public:
        Clazz() = default;
        shared_ptr<Clazz> init(classfile::ClassFile);
        bool haveAccess(uint16);
        string getPackageName();
        shared_ptr<Method> getStaticMethod(string, string);
        shared_ptr<Method> getMainMethod();
    public:
        bool isAccessTo(Clazz);
        bool isAssignFrom(Clazz);
        bool isSubClassOf(Clazz);
        bool isImplementOf(Clazz);
        bool isSubInterfaceOf(Clazz);
    public:
        bool operator==(const Clazz &) const;
        bool operator!=(const Clazz &) const;
    };
}


#endif //JVM_RUNTIME_CLAZZ_H
