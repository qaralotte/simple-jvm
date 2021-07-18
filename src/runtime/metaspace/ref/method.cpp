#include "include/runtime/metaspace/ref/method.h"
#include "include/classfile/constant.h"
#include "include/runtime/loader.h"
#include "include/accessflags.h"
#include "include/log.h"

using namespace runtime;

MethodRef::MethodRef(shared_ptr<ConstantPool> rconstant_pool, const vector<shared_ptr<classfile::cp_info>> &cconstant_pool, shared_ptr<classfile::Method> info) {
    auto class_const = classfile::ConstantUtils::getConstant<classfile::Class>(cconstant_pool, info -> class_index);
    auto name_and_type_const = classfile::ConstantUtils::getConstant<classfile::NameAndType>(cconstant_pool, info -> name_and_type_index);
    class_name = classfile::ConstantUtils::getConstant<classfile::Utf8>(cconstant_pool, class_const -> name_index) -> value;
    name = classfile::ConstantUtils::getConstant<classfile::Utf8>(cconstant_pool, name_and_type_const -> name_index) -> value;
    descriptor = classfile::ConstantUtils::getConstant<classfile::Utf8>(cconstant_pool, name_and_type_const -> descriptor_index) -> value;
    constant_pool = rconstant_pool;
}

shared_ptr<Clazz> MethodRef::resolvedClass() {
    if (clazz == nullptr) {
        auto d = constant_pool -> clazz;
        auto c = ClassLoader(class_name).loadClass();
        /* 当类是 public 且同一个 package 内才可以访问 */
        if (!c -> isAccessTo(*d)) {
            ERROR("java.lang.IllegalAccessError: %s", c -> this_name.c_str());
            exit(0);
        }
        clazz = c;
    }
    return clazz;
}

shared_ptr<Method> MethodRef::resolvedMethod() {
    if (method == nullptr) {
        auto d = constant_pool -> clazz;
        auto c = resolvedClass();
        if (c -> haveAccess(ACCESS_INTERFACE)) {
            ERROR("java.lang.IncompatibleClassChangeError");
            exit(0);
        }
        method = c -> findMethod(name, descriptor);
        if (method == nullptr) {
            ERROR("java.lang.NoSuchMethodError");
            exit(0);
        }
        if (!method -> isAccessTo(*d)) {
            ERROR("java.lang.IllegalAccessError");
            exit(0);
        }
    }
    return method;
}