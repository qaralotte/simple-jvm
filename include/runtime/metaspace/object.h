#ifndef JVM_RUNTIME_OBJECT_H
#define JVM_RUNTIME_OBJECT_H

#include <vector>

#include "include/std.h"

#include "include/runtime/metaspace/clazz.h"
#include "include/runtime/vartable_table.h"

namespace runtime {

    struct Slot;
    class Object {
    public:
        Clazz clazz;
        VariableTable field;
    public:
        Object(Clazz _clazz, VariableTable _field) : clazz(_clazz), field(_field) {};
        bool isInstanceOf(Clazz);
    public:
        bool operator==(const Object &) const;
        bool operator!=(const Object &) const;
    };

    typedef shared_ptr<Object> jobject;
    inline jobject make_jobject(runtime::Object obj) {return make_shared<runtime::Object>(obj);}
}


#endif //JVM_RUNTIME_OBJECT_H
