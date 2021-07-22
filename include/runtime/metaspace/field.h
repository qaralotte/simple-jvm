#ifndef JVM_RUNTIME_FIELD_H
#define JVM_RUNTIME_FIELD_H

#include <string>
#include <vector>

#include "include/std.h"
#include "include/classfile/field.h"

namespace runtime {
    class Clazz;
    class Field : public enable_shared_from_this<Field> {
    public:
        uint16 access_flags;
        string name;
        string descriptor;
        uint slot_id;
        uint constvalue_index;
        shared_ptr<Clazz> clazz = nullptr;
    private:
        shared_ptr<Field> init(shared_ptr<Clazz>, classfile::field_info);
    public:
        Field() = default;
        static vector<shared_ptr<Field>> arrayOf(shared_ptr<Clazz>, vector<classfile::field_info>);
    public:
        bool haveAccess(uint16);
        bool isAccessTo(Clazz);
    public:
        bool operator==(const Field &) const;
        bool operator!=(const Field &) const;
    };
}


#endif //JVM_RUNTIME_FIELD_H
