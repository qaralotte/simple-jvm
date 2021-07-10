#ifndef JVM_RUNTIME_LOADER_H
#define JVM_RUNTIME_LOADER_H

#include <string>
#include <map>

#include "include/std.h"
#include "include/classpath.h"

#include "include/runtime/metaspace/clazz.h"

namespace runtime {
    class ClassLoader {
    private:
        string class_name;
    private:
        void loadNonArrayClass();
        void prepare();
    public:
        static map<string, shared_ptr<Clazz>> class_map;
    public:
        ClassLoader(string _name) : class_name(_name) {};
        shared_ptr<Clazz> loadClass();
    };
}

#endif //JVM_RUNTIME_LOADER_H
