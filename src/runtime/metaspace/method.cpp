#include "include/runtime/metaspace/method.h"
#include "include/runtime/metaspace/clazz.h"
#include "include/accessflags.h"

using namespace runtime;

shared_ptr<Method> Method::init(shared_ptr<Clazz> _clazz, classfile::method_info info) {
    access_flags = info.access_flags;
    name = info.name;
    descriptor = info.descriptor;
    for (auto attribute : info.attributes) {
        if (attribute -> name == "Code") {
            auto code_attr = static_pointer_cast<classfile::Code>(attribute);
            max_stack = code_attr -> max_stack;
            max_locals = code_attr -> max_locals;
            code = code_attr -> code;
            break;
        }
    }
    clazz = _clazz;
    return shared_from_this();
}

vector<shared_ptr<Method>> Method::arrayOf(shared_ptr<Clazz> _clazz, vector<classfile::method_info> cmethods) {
    vector<shared_ptr<Method>> methods;
    for (auto cmethod : cmethods) {
        auto tmethod = make_shared<Method>();
        auto method = tmethod -> init(_clazz, cmethod);
        methods.push_back(method);
    }
    return methods;
}

bool Method::haveAccess(uint16 flag) {
    return (access_flags & flag) != 0;
}

bool Method::isAccessTo(Clazz d) {
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

bool Method::operator==(const Method &other) const {
    if (access_flags != other.access_flags) return false;
    if (name != other.name) return false;
    if (descriptor != other.descriptor) return false;
    if (max_stack != other.max_stack) return false;
    if (max_locals != other.max_locals) return false;
    if (code != other.code) return false;
    return true;
}

bool Method::operator!=(const Method &other) const {
    return !operator==(other);
}