#ifndef JVM_STRING_H
#define JVM_STRING_H

#include <map>

#include "include/std.h"

#include "include/runtime/metaspace/object.h"
#include "include/runtime/loader.h"

namespace runtime {
    class JString {
    private:
        string raw_string;
    public:
        static map<string, jobject> pool;
    public:
        JString(string _raw) : raw_string(_raw) {};
        jobject toObject();
    };
}

#endif //JVM_STRING_H
