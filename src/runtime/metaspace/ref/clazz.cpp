#include "include/runtime/metaspace/ref/clazz.h"
#include "include/classfile/constant.h"

using namespace runtime;

ClassRef::ClassRef(ConstantPool *rconstant_pool, const vector<shared_ptr<classfile::cp_info>> &cconstant_pool, shared_ptr<classfile::Class> info) {
    class_name = classfile::ConstantUtils::getConstant<classfile::Utf8>(cconstant_pool, info -> name_index) -> value;
    constant_pool = rconstant_pool;
    clazz = rconstant_pool -> clazz;
}
