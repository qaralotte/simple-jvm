#ifndef JVM_RUNTIME_LOADER_H
#define JVM_RUNTIME_LOADER_H

#include <string>
#include <map>

#include "include/std.h"
#include "include/classpath.h"

namespace runtime {

    class Clazz;
    class ClassLoader {
    private:
        static Clazz loadNonArrayClass(string);
    public:
        static map<string, Clazz> class_map;
        static Clazz loadClass(string);
    };
}

#endif //JVM_RUNTIME_LOADER_H
