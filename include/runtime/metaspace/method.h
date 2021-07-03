#ifndef JVM_RUNTIME_METHOD_H
#define JVM_RUNTIME_METHOD_H

#include <string>
#include <vector>

#include "include/std.h"
#include "include/classfile/method.h"

namespace runtime {
    class Clazz;
    class Method {
    public:
        uint16 access_flags;
        jstring name;
        jstring descriptor;
        uint16 max_stack;
        uint16 max_locals;
        vector<uint8> code;
        Clazz *clazz;
    private:
        Method(Clazz *, classfile::method_info);
    public:
        static vector<Method> arrayOf(Clazz *, vector<classfile::method_info>);
    };
}


#endif //JVM_RUNTIME_METHOD_H
