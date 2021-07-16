#include "include/runtime/metaspace/ref/clazz.h"
#include "include/classfile/constant.h"
#include "include/runtime/loader.h"
#include "include/accessflags.h"
#include "include/log.h"

using namespace runtime;

ClassRef::ClassRef(shared_ptr<ConstantPool> rconstant_pool, const vector<shared_ptr<classfile::cp_info>> &cconstant_pool, shared_ptr<classfile::Class> info) {
    class_name = classfile::ConstantUtils::getConstant<classfile::Utf8>(cconstant_pool, info -> name_index) -> value;
    constant_pool = rconstant_pool;
}

shared_ptr<Clazz> ClassRef::resolvedClass() {
    if (clazz == nullptr) {
        shared_ptr<Clazz> d = constant_pool -> clazz;
        shared_ptr<Clazz> c = ClassLoader(class_name).loadClass();
        /* 当类是 public 且同一个 package 内才可以访问 */
        if (!c -> isAccessTo(*d)) {
            ERROR("java.lang.IllegalAccessError: %s.class", c -> this_name.c_str());
            exit(0);
        }
        clazz = c;
    }
    return clazz;
}