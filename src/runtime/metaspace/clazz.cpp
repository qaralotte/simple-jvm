#include "include/runtime/metaspace/clazz.h"
#include "include/accessflags.h"

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

bool Clazz::isAccessTo(Clazz d) {
    return haveAccess(ACCESS_PUBLIC) || getPackageName() == d.getPackageName();
}

bool Clazz::isAssignFrom(Clazz d) {
    if (*this == d) return true;
    if (!haveAccess(ACCESS_INTERFACE)) {
        return d.isSubClassOf(*this);
    } else {
        return d.isImplementOf(*this);
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