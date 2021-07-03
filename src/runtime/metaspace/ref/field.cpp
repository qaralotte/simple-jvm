#include "include/runtime/metaspace/ref/field.h"
#include "include/classfile/constant.h"

using namespace runtime;

FieldRef::FieldRef(ConstantPool *rconstant_pool, const vector<shared_ptr<classfile::cp_info>> &cconstant_pool, shared_ptr<classfile::Field> info) {
    auto class_const = classfile::ConstantUtils::getConstant<classfile::Class>(cconstant_pool, info -> class_index);
    auto name_and_type_const = classfile::ConstantUtils::getConstant<classfile::NameAndType>(cconstant_pool, info -> name_and_type_index);
    class_name = classfile::ConstantUtils::getConstant<classfile::Utf8>(cconstant_pool, class_const -> name_index) -> value;
    name = classfile::ConstantUtils::getConstant<classfile::Utf8>(cconstant_pool, name_and_type_const -> name_index) -> value;
    descriptor = classfile::ConstantUtils::getConstant<classfile::Utf8>(cconstant_pool, name_and_type_const -> descriptor_index) -> value;
    constant_pool = rconstant_pool;
    clazz = rconstant_pool -> clazz;
}


