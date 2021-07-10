#include "include/runtime/metaspace/field.h"
#include "include/runtime/metaspace/clazz.h"
#include "include/accessflags.h"

using namespace runtime;

shared_ptr<Field> Field::init(shared_ptr<Clazz> _clazz, classfile::field_info info) {
    access_flags = info.access_flags;
    name = info.name;
    descriptor = info.descriptor;
    clazz = _clazz;
    for (auto attr : info.attributes) {
        if (attr -> name == "ConstantValue") {
            constvalue_index = static_pointer_cast<classfile::ConstantValue>(attr) -> constantvalue_index;
            break;
        }
    }
    return shared_from_this();
}

vector<shared_ptr<Field>> Field::arrayOf(shared_ptr<Clazz> _clazz, vector<classfile::field_info> cfields) {
    vector<shared_ptr<Field>> fields;
    for (auto cfield : cfields) {
        auto tfield = make_shared<Field>();
        auto field = tfield -> init(_clazz, cfield);
        fields.push_back(field);
    }
    return fields;
}

bool Field::haveAccess(uint16 flag) {
    return (access_flags & flag) != 0;
}

bool Field::isAccessTo(Clazz d) {
    if (haveAccess(ACCESS_PUBLIC)) return true;

    auto c = *clazz;
    if (haveAccess(ACCESS_PROTECTED)) {
        return c == d || d.super_name == c.this_name || c.getPackageName() == d.getPackageName();
    }
    if (!haveAccess(ACCESS_PRIVATE)) {
        return c.getPackageName() == d.getPackageName();
    }
    return c == d;
}