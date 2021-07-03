#include "include/runtime/metaspace/constant.h"
#include "include/runtime/metaspace/ref/clazz.h"
#include "include/runtime/metaspace/ref/field.h"
#include "include/runtime/metaspace/ref/method.h"
#include "include/runtime/metaspace/ref/interface_method.h"

using namespace runtime;

ConstantPool::ConstantPool(Clazz *_clazz, vector<shared_ptr<classfile::cp_info>> constant_pool) {
    constants.resize(constant_pool.size());
    for (int i = 1; i < constant_pool.size(); ++i) {
        clazz = _clazz;
        auto info = constant_pool[i];
        uint8 tag = info -> tag;
        switch (tag) {
            /* 数字 */
            case classfile::Constant::INTEGER: {
                auto origin_info = static_pointer_cast<classfile::Integer>(info);
                auto constant = make_shared<IntegerConstant>();
                constant -> tag = ConstantType::INTEGER;
                constant -> value = origin_info -> value;
                constants[i] = constant;
                break;
            }
            case classfile::Constant::FLOAT: {
                auto origin_info = static_pointer_cast<classfile::Float>(info);
                auto constant = make_shared<FloatConstant>();
                constant -> tag = ConstantType::FLOAT;
                constant -> value = origin_info -> value;
                constants[i] = constant;
                break;
            }
            case classfile::Constant::LONG: {
                auto origin_info = static_pointer_cast<classfile::Long>(info);
                auto constant = make_shared<LongConstant>();
                constant -> tag = ConstantType::LONG;
                constant -> value = origin_info -> value;
                constants[i] = constant;
                i += 1;
                break;
            }
            case classfile::Constant::DOUBLE: {
                auto origin_info = static_pointer_cast<classfile::Double>(info);
                auto constant = make_shared<DoubleConstant>();
                constant -> tag = ConstantType::DOUBLE;
                constant -> value = origin_info -> value;
                constants[i] = constant;
                i += 1;
                break;
            }
            case classfile::Constant::STRING: {
                auto origin_info = static_pointer_cast<classfile::String>(info);
                auto constant = make_shared<StringConstant>();
                constant -> tag = ConstantType::STRING;
                constant -> value = classfile::ConstantUtils::getConstant<classfile::Utf8>(constant_pool, origin_info -> string_index) -> value;
                constants[i] = constant;
                break;
            }
            case classfile::Constant::CLASS: {
                auto origin_info = static_pointer_cast<classfile::Class>(info);
                auto constant = make_shared<ClassRef>(ClassRef(this, constant_pool, origin_info));
                constant -> tag = ConstantType::CLASSREF;
                constants[i] = constant;
                break;
            }
            case classfile::Constant::FIELDREF: {
                auto origin_info = static_pointer_cast<classfile::Field>(info);
                auto constant = make_shared<FieldRef>(FieldRef(this, constant_pool, origin_info));
                constant -> tag = ConstantType::FIELDREF;
                constants[i] = constant;
                break;
            }
            case classfile::Constant::METHODREF: {
                auto origin_info = static_pointer_cast<classfile::Method>(info);
                auto constant = make_shared<MethodRef>(MethodRef(this, constant_pool, origin_info));
                constant -> tag = ConstantType::METHODREF;
                constants[i] = constant;
                break;
            }
            case classfile::Constant::INTERFACE_METHODREF: {
                auto origin_info = static_pointer_cast<classfile::InterfaceMethodref>(info);
                auto constant = make_shared<InterfaceMethodRef>(InterfaceMethodRef(this, constant_pool, origin_info));
                constant -> tag = ConstantType::INTERFACE_METHODREF;
                constants[i] = constant;
                break;
            }
        }
    }
}
