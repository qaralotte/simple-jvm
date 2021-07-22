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
            VariableTable single_data;    // non-array
            vector<any> array_data;       // array
        };
    public:
        Object() = default;
        Object(shared_ptr<Clazz> _clazz) : clazz(_clazz) {};
        Object(shared_ptr<Clazz> _clazz, VariableTable _single) : clazz(_clazz), single_data(_single) {};
        template<typename T> static Object newArray(shared_ptr<Clazz> _clazz, uint length) {
            Object object;
            object.clazz = _clazz;
            object.array_data.resize(length);
            for (int i = 0; i < length; ++i) {
                object.array_data[i] = T();
            }
            return object;
        }
        static shared_ptr<Object> newMultiDimensionArray(vector<jint>, shared_ptr<Clazz>);
        bool isInstanceOf(Clazz);
        void setRefVal(string, string, shared_ptr<Object>);
        shared_ptr<Object> getRefVal(string, string);
    public:
        bool operator==(const Object &) const;
        bool operator!=(const Object &) const;
    };

    typedef shared_ptr<Object> jobject;
    inline jobject make_jobject(runtime::Object obj) {return make_shared<runtime::Object>(obj);}
}


#endif //JVM_RUNTIME_OBJECT_H
