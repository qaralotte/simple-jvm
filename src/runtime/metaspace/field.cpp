#include "include/runtime/metaspace/field.h"
#include "include/runtime/metaspace/clazz.h"

using namespace runtime;

Field::Field(Clazz *_clazz, classfile::field_info info) {
    access_flags = info.access_flags;
    name = info.name;
    descriptor = info.descriptor;
    clazz = _clazz;
}

vector<Field> Field::arrayOf(Clazz *_clazz, vector<classfile::field_info> cfields) {
    vector<Field> fields;
    for (auto cfield : cfields) {
        fields.push_back(Field(_clazz, cfield));
    }
    return fields;
}
