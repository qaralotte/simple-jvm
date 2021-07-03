#include "include/runtime/metaspace/method.h"
#include "include/runtime/metaspace/clazz.h"

using namespace runtime;

Method::Method(Clazz *_clazz, classfile::method_info info) {
    access_flags = info.access_flags;
    name = info.name;
    descriptor = info.descriptor;
    for (auto attribute : info.attributes) {
        if (attribute -> name == "Code") {
            auto code_attr = static_pointer_cast<classfile::Code>(attribute);
            max_stack = code_attr -> max_stack;
            max_locals = code_attr -> max_locals;
            code = code_attr -> code;
        }
    }

    clazz = _clazz;
}

vector<Method> Method::arrayOf(Clazz *_clazz, vector<classfile::method_info> cmethods) {
    vector<Method> methods;
    for (auto cmethod : cmethods) {
        methods.push_back(Method(_clazz, cmethod));
    }
    return methods;
}

