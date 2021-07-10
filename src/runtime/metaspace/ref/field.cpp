#include "include/runtime/metaspace/ref/field.h"
#include "include/classfile/constant.h"
#include "include/runtime/loader.h"
#include "include/accessflags.h"
#include "include/log.h"

using namespace runtime;

shared_ptr<Field> FieldRef::lookupField(shared_ptr<Clazz> _clazz, string _name, string _descriptor) {
    for (auto _field : _clazz -> fields) {
        if (_field -> name == _name && _field -> descriptor == _descriptor) {
            return _field;
        }
    }
    for (auto _interface : _clazz -> interfaces) {
        auto _field = lookupField(_interface, _name, _descriptor);
        if (_field != nullptr) {
            return _field;
        }
    }
    if (_clazz -> super_class != nullptr) {
        return lookupField(_clazz -> super_class, _name, _descriptor);
    }
    return nullptr;
}

FieldRef::FieldRef(shared_ptr<ConstantPool> rconstant_pool, const vector<shared_ptr<classfile::cp_info>> &cconstant_pool, shared_ptr<classfile::Field> info) {
    auto class_const = classfile::ConstantUtils::getConstant<classfile::Class>(cconstant_pool, info -> class_index);
    auto name_and_type_const = classfile::ConstantUtils::getConstant<classfile::NameAndType>(cconstant_pool, info -> name_and_type_index);
    class_name = classfile::ConstantUtils::getConstant<classfile::Utf8>(cconstant_pool, class_const -> name_index) -> value;
    name = classfile::ConstantUtils::getConstant<classfile::Utf8>(cconstant_pool, name_and_type_const -> name_index) -> value;
    descriptor = classfile::ConstantUtils::getConstant<classfile::Utf8>(cconstant_pool, name_and_type_const -> descriptor_index) -> value;
    constant_pool = rconstant_pool;
}

shared_ptr<Clazz> FieldRef::resolvedClass() {
    if (clazz == nullptr) {
        shared_ptr<Clazz> d = constant_pool -> clazz;
        shared_ptr<Clazz> c = ClassLoader(class_name).loadClass();
        /* 当类是 public 且同一个 package 内才可以访问 */
        if (!c -> isAccessTo(*d)) {
            ERROR("java.lang.IllegalAccessError: %s", c -> this_name.c_str());
            exit(0);
        }
        clazz = c;
    }
    return clazz;
}

shared_ptr<Field> FieldRef::resolvedField() {
    if (field == nullptr) {
        shared_ptr<Clazz> d = constant_pool -> clazz;
        shared_ptr<Clazz> c = resolvedClass();
        field = lookupField(c, name, descriptor);
        if (field == nullptr) {
            ERROR("java.lang.NoSuchFieldError");
            exit(0);
        }
        if (!field -> isAccessTo(*d)) {
            ERROR("java.lang.IllegalAccessError");
            exit(0);
        }
    }
    return field;
}

bool Field::operator==(const Field &other) const {
    if (access_flags != other.access_flags) return false;
    if (name != other.name) return false;
    if (descriptor != other.descriptor) return false;
    if (slot_id != other.slot_id) return false;
    if (constvalue_index != other.constvalue_index) return false;
    return true;
}

bool Field::operator!=(const Field &other) const {
    return !operator==(other);
}