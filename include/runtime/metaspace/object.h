#ifndef JVM_RUNTIME_OBJECT_H
#define JVM_RUNTIME_OBJECT_H

#include <vector>
#include <any>

#include "include/std.h"

#include "include/runtime/metaspace/clazz.h"
#include "include/runtime/vartable_table.h"

namespace runtime {

    struct Slot;
    class Object {
    public:
        shared_ptr<Clazz> clazz;
        struct {
            VariableTable field;
            vector<any> data;
        };
    public:
        Object() = default;
        Object(shared_ptr<Clazz> _clazz, VariableTable _field) : clazz(_clazz), field(_field) {};
        template<typename T> static Object newArray(shared_ptr<Clazz> _clazz, uint length) {
            Object object;
            object.clazz = _clazz;
            object.data.resize(length);
            for (int i = 0; i < length; ++i) {
                object.data[i] = T();
            }
            return object;
        }
        static shared_ptr<Object> newMultiDimensionArray(vector<jint>, shared_ptr<Clazz>);
        bool isInstanceOf(Clazz);
    public:
        bool operator==(const Object &) const;
        bool operator!=(const Object &) const;
    };

    typedef shared_ptr<Object> jobject;
    inline jobject make_jobject(runtime::Object obj) {return make_shared<runtime::Object>(obj);}
}


#endif //JVM_RUNTIME_OBJECT_H
