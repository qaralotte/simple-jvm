#ifndef JVM_RUNTIME_METHOD_H
#define JVM_RUNTIME_METHOD_H

#include <string>
#include <vector>

#include "include/std.h"
#include "include/classfile/method.h"

namespace runtime {
    class Clazz;
    class Method : public enable_shared_from_this<Method> {
    public:
        uint16 access_flags;
        jstring name;
        jstring descriptor;
        uint16 max_stack;
        uint16 max_locals;
        vector<uint8> code;
        shared_ptr<Clazz>clazz = nullptr;
    private:
        shared_ptr<Method> init(shared_ptr<Clazz>, classfile::method_info);
    public:
        static vector<shared_ptr<Method>> arrayOf(shared_ptr<Clazz>, vector<classfile::method_info>);
    public:
        Method() = default;
        bool haveAccess(uint16);
        bool isAccessTo(Clazz);
    public:
        bool operator==(const Method &) const;
        bool operator!=(const Method &) const;
    };
}


#endif //JVM_RUNTIME_METHOD_H
