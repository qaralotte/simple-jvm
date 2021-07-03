#ifndef JVM_RUNTIME_FIELD_H
#define JVM_RUNTIME_FIELD_H

#include <string>
#include <vector>

#include "include/std.h"
#include "include/classfile/field.h"

namespace runtime {
    class Clazz;
    class Field {
    public:
        uint16 access_flags;
        jstring name;
        jstring descriptor;
        Clazz *clazz;
    private:
        Field(Clazz *, classfile::field_info);
    public:
        static vector<Field> arrayOf(Clazz *, vector<classfile::field_info>);
    };
}


#endif //JVM_RUNTIME_FIELD_H
