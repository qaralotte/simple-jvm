#include "include/runtime/metaspace/clazz.h"
#include "include/runtime/metaspace/object.h"
#include "include/runtime/loader.h"
#include "include/accessflags.h"
#include "include/descriptor.h"
#include "include/log.h"

using namespace runtime;

shared_ptr<Clazz> Clazz::init(classfile::ClassFile clazz) {
    access_flags = clazz.access_flags;
    this_name = clazz.this_class;
    super_name = clazz.super_class;
    interfaces_name = clazz.interfaces;
    fields = Field::arrayOf(shared_from_this(), clazz.fields);
    methods = Method::arrayOf(shared_from_this(), clazz.methods);
    constant_pool = make_shared<ConstantPool>() -> init(shared_from_this(), clazz.constant_pool);
    return shared_from_this();
}

shared_ptr<Object> Clazz::initArray(uint length) {
    if (!isArray()) {
        ERROR("不是一个数组: %s", this_name.c_str());
        exit(0);
    }

    if (this_name == "[Z" || this_name == "[B") {
        return make_jobject(Object::newArray<jbyte>(shared_from_this(), length));
    }
    if (this_name == "[C") {
        return make_jobject(Object::newArray<jchar>(shared_from_this(), length));
    }
    if (this_name == "[S") {
        return make_jobject(Object::newArray<jshort>(shared_from_this(), length));
    }
    if (this_name == "[I") {
        return make_jobject(Object::newArray<jint>(shared_from_this(), length));
    }
    if (this_name == "[J") {
        return make_jobject(Object::newArray<jlong>(shared_from_this(), length));
    }
    if (this_name == "[F") {
        return make_jobject(Object::newArray<jfloat>(shared_from_this(), length));
    }
    if (this_name == "[D") {
        return make_jobject(Object::newArray<jdouble>(shared_from_this(), length));
    }
    return make_jobject(Object::newArray<jobject>(shared_from_this(), length));
}

shared_ptr<Object> Clazz::toObject() {
    return make_jobject(Object(shared_from_this()));
}

shared_ptr<Clazz> Clazz::toArray() {
    auto class_name = DESC_ARRAY(DESC(this_name));
    return ClassLoader(class_name).loadClass();
}

shared_ptr<Clazz> Clazz::getComponentClass() {
    string component_name;
    if (this_name[0] == '[') {
        auto descriptor = this_name.substr(1, this_name.size() - 1);
        if (descriptor[0] == '[') {
            component_name = descriptor;
        } else if (descriptor[0] == 'L') {
            component_name = descriptor.substr(1, descriptor.size() - 1);

            auto name = DESC_TO_PRIM(descriptor);
            if (!name.empty()) {
                component_name = name;
            }
        } else {
            ERROR("不合法的 descriptor: %s", descriptor.c_str());
            exit(0);
        }
    } else {
        ERROR("%s 不是一个合法的数组", this_name.c_str());
        exit(0);
    }
    return ClassLoader(component_name).loadClass();
}

bool Clazz::haveAccess(uint16 flag) {
    return (access_flags & flag) != 0;
}

string Clazz::getPackageName() {
    string package_name;
    string tname;
    for (auto iter = this_name.begin(); iter != this_name.end(); ++iter) {
        tname.push_back(*iter);
        if (*iter == '/') {
            package_name += tname;
            tname.clear();
        }
    }
    return package_name;
}

shared_ptr<Method> Clazz::getMainMethod() {
    return findMethodInClass("main", "([Ljava/lang/String;)V", {ACCESS_STATIC});
}

shared_ptr<Method> Clazz::findMethod(string name, string descriptor, vector<uint16> accesses) {
    auto method = findMethodInClass(name, descriptor, accesses);
    if (findMethodInClass(name, descriptor, accesses) == nullptr) {
        method = findMethodInInterface(name, descriptor, accesses);
    }
    return method;
}

shared_ptr<Method> Clazz::findMethodInClass(string name, string descriptor, vector<uint16> accesses) {
    for (auto method : methods) {
        bool have_access = true;
        if (accesses.size() > 0) {
            for (auto access : accesses) {
                if (!method -> haveAccess(access)) {
                    have_access = false;
                    break;
                }
            }
        }
        if (have_access) {
            if (method -> name == name && method -> descriptor == descriptor) return method;
        }
    }
    return nullptr;
}

shared_ptr<Method> Clazz::findMethodInInterface(string name, string descriptor, vector<uint16> accesses) {
    for (auto interface : interfaces) {
        for (auto method : interface -> methods) {
            bool have_access = true;
            if (accesses.size() > 0) {
                for (auto access : accesses) {
                    if (!method -> haveAccess(access)) {
                        have_access = false;
                        break;
                    }
                }
            }
            if (have_access) {
                if (method -> name == name && method -> descriptor == descriptor) return method;
                method = interface ->findMethodInInterface(name, descriptor, accesses);
                if (method != nullptr) return method;
            }
        }
    }
    return nullptr;
}

shared_ptr<Field> Clazz::findField(string name, string descriptor, vector<uint16> accesses) {
    for (auto c = shared_from_this(); c != nullptr; c = c -> super_class) {
        for (auto field : c -> fields) {
            bool have_access = true;
            if (accesses.size() > 0) {
                for (auto access : accesses) {
                    if (!field -> haveAccess(access)) {
                        have_access = false;
                        break;
                    }
                }
            }
            if (have_access) {
                if (field -> name == name && field -> descriptor == descriptor) return field;
            }
        }
    }
    return nullptr;
}

bool Clazz::isAccessTo(Clazz d) {
    return haveAccess(ACCESS_PUBLIC) || getPackageName() == d.getPackageName();
}

bool Clazz::isAssignFrom(Clazz d) {
    if (*this == d) return true;

    if (!isArray()) {
        if (!haveAccess(ACCESS_INTERFACE)) {
            if (!d.haveAccess(ACCESS_INTERFACE)) {
                return d.isSubClassOf(*this);
            } else {
                return d.isImplementOf(*this);
            }
        } else {
            if (!d.haveAccess(ACCESS_INTERFACE)) {
                return d.this_name == "java/lang/Object";
            } else {
                return d.isSubInterfaceOf(*this);
            }
        }
    } else {
        if (!d.isArray()) {
            if (!d.haveAccess(ACCESS_INTERFACE)) {
                return d.this_name == "java/lang/Object";
            } else {
                return d.this_name == "java/lang/Cloneable";
            }
        } else {
            auto c = getComponentClass();
            auto dc = d.getComponentClass();
            return *c == *dc || dc -> isAssignFrom(*c);
        }
    }
}

bool Clazz::isSubClassOf(Clazz d) {
    auto subclass = super_class;
    while (subclass != nullptr) {
        if (*subclass == d) return true;
        subclass = subclass -> super_class;
    }
    return false;
}

bool Clazz::isImplementOf(Clazz d) {
    auto subclass = super_class;
    while (subclass != nullptr) {
        for (auto interface : subclass -> interfaces) {
            if (*interface == d || isSubInterfaceOf(d)) return true;
        }
        subclass = subclass -> super_class;
    }
    return false;
}

bool Clazz::isSubInterfaceOf(Clazz d) {
    for (auto interface : interfaces) {
        if (*interface == d || isSubInterfaceOf(d)) return true;
    }
    return false;
}

bool Clazz::isArray() {
    return this_name[0] == '[';
}

#pragma clang diagnostic ignored "-Wtautological-undefined-compare"
bool Clazz::operator==(const Clazz &other) const {
    if (this == nullptr && &other == nullptr) return true;
    if (this == nullptr || &other == nullptr) return false;
    if (access_flags != other.access_flags) return false;
    if (this_name != other.this_name) return false;
    if (super_name != other.super_name) return false;
    if (interfaces_name != other.interfaces_name) return false;
    if (fields != other.fields) return false;
    if (methods != other.methods) return false;
    if (constant_pool -> constants != other.constant_pool -> constants) return false;
    if (*super_class != *other.super_class) return false;
    if (interfaces != other.interfaces) return false;
    if (instance_slot_count != other.instance_slot_count) return false;
    if (static_slot_count != other.static_slot_count) return false;
    // if (static_vars.getSlots() != other.static_vars.getSlots()) return false;
    return true;
}

bool Clazz::operator!=(const Clazz &other) const {
    return !operator==(other);
}