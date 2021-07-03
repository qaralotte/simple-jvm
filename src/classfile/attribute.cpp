#include <functional>

#include "include/classfile/loader.h"
#include "include/classfile/attribute.h"
#include "include/classfile/constant.h"

using namespace classfile;

shared_ptr<attribute_info> ClassLoader::readAttribute() {
    jstring name = ConstantUtils::getConstant<Utf8>(clazz.constant_pool, readU2()) -> value;
    u4 attribute_length = readU4();

    if (name == "ConstantValue") {
        auto attr = make_shared<ConstantValue>();
        attr -> name = name;
        attr -> attribute_length = attribute_length;
        attr -> constantvalue_index = readU2();
        return attr;
    }
    if (name == "Code") {
        auto attr = make_shared<Code>();
        attr -> name = name;
        attr -> attribute_length = attribute_length;
        attr -> max_stack = readU2();
        attr -> max_locals = readU2();
        attr -> code.resize(readU4());
        for (auto &co : attr -> code) {
            co = readU1();
        }
        attr -> exception_table.resize(readU2());
        for (auto &exception : attr -> exception_table) {
            exception.start_pc = readU2();
            exception.end_pc = readU2();
            exception.handle_pc = readU2();
            exception.catch_type = readU2();
        }
        attr -> attributes.resize(readU2());
        for (auto &attribute : attr -> attributes) {
            attribute = readAttribute();
        }
        return attr;
    }
    if (name == "StackMapTable") {
        using namespace stack_map_table;
        auto attr = make_shared<StackMapTable>();
        attr -> name = name;
        attr -> attribute_length = attribute_length;
        attr -> entries.resize(readU2());
        auto readVerification = [&]() -> verification_type_info {
            verification_type_info info;
            info.tag = readU1();
            if (info.tag == ITEM_OBJECT) {
                info.cpool_index = readU2();
            } else if (info.tag == ITEM_UNINITIALIZED) {
                info.offset = readU2();
            }
            return info;
        };
        for (auto &entry : attr -> entries) {
            entry.frame_type = readU1();
            if (entry.frame_type >= 0 && entry.frame_type <= 63) {
                entry.frame_type = SAME;
            } else if (entry.frame_type >= 64 && entry.frame_type <= 127) {
                entry.frame_type = SAME_LOCALS_1_STACK_ITEM;
                entry.stack.resize(1);
                entry.stack[0] = readVerification();
            } else if (entry.frame_type == 247) {
                entry.frame_type = SAME_LOCALS_1_STACK_ITEM_EXTENDED;
                entry.offset_delta = readU2();
                entry.stack.resize(1);
                entry.stack[0] = readVerification();
            } else if (entry.frame_type >= 248 && entry.frame_type <= 250) {
                entry.frame_type = CHOP;
                entry.offset_delta = readU2();
            } else if (entry.frame_type == 251) {
                entry.frame_type = SAME_FRAME_EXTENDED;
                entry.offset_delta = readU2();
            } else if (entry.frame_type >= 252 && entry.frame_type <= 254) {
                u1 frame_type = entry.frame_type;
                entry.frame_type = APPEND;
                entry.offset_delta = readU2();
                entry.locals.resize(frame_type - 251);
                for (auto &local : entry.locals) {
                    local = readVerification();
                }
            } else if (entry.frame_type == 255) {
                entry.frame_type = FULL_FRAME;
                entry.offset_delta = readU2();
                entry.locals.resize(readU2());
                for (auto &local : entry.locals) {
                    local = readVerification();
                }
                entry.stack.resize(readU2());
                for (auto &frame : entry.stack) {
                    frame = readVerification();
                }
            }
        }
        return attr;
    }
    if (name == "Exceptions") {
        auto attr = make_shared<Exceptions>();
        attr -> name = name;
        attr -> attribute_length = attribute_length;
        attr -> exception_index_table.resize(readU2());
        for (auto &exception_index : attr->exception_index_table) {
            exception_index = readU2();
        }
        return attr;
    }
    if (name == "InnerClasses") {
        auto attr = make_shared<InnerClasses>();
        attr -> name = name;
        attr -> attribute_length = attribute_length;
        attr -> classes.resize(readU2());
        for (auto &clz : attr -> classes) {
            clz.inner_class_info_index = readU2();
            clz.outer_class_info_index = readU2();
            clz.inner_name_index = readU2();
            clz.inner_class_access_flags = readU2();
        }
        return attr;
    }
    if (name == "EnclosingMethod") {
        auto attr = make_shared<EnclosingMethod>();
        attr -> name = name;
        attr -> attribute_length = attribute_length;
        attr -> class_index = readU2();
        attr -> method_index = readU2();
        return attr;
    }
    if (name == "Synthetic") {
        auto attr = make_shared<Synthetic>();
        attr -> name = name;
        attr -> attribute_length = attribute_length;
        return attr;
    }
    if (name == "Signature") {
        auto attr = make_shared<Signature>();
        attr -> name = name;
        attr -> attribute_length = attribute_length;
        attr -> signature_index = readU2();
        return attr;
    }
    if (name == "SourceFile") {
        auto attr = make_shared<SourceFile>();
        attr -> name = name;
        attr -> attribute_length = attribute_length;
        attr -> sourcefile_index = readU2();
        return attr;
    }
    if (name == "SourceDebugExtension") {
        auto attr = make_shared<SourceDebugExtension>();
        attr -> name = name;
        attr -> attribute_length = attribute_length;
        attr -> debug_extension.resize(readU1());
        for (int i = 0; i < attr -> debug_extension.size(); ++i) {
            attr -> debug_extension[i] = readU1();
        }
        return attr;
    }
    if (name == "LineNumberTable") {
        auto attr = make_shared<LineNumberTable>();
        attr -> name = name;
        attr -> attribute_length = attribute_length;
        attr -> line_number_table.resize(readU2());
        for (int i = 0; i < attr -> line_number_table.size(); ++i) {
            attr -> line_number_table[i].start_pc = readU2();
            attr -> line_number_table[i].line_number = readU2();
        }
        return attr;
    }
    if (name == "LocalVariableTable") {
        auto attr = make_shared<LocalVariableTable>();
        attr -> name = name;
        attr -> attribute_length = attribute_length;
        attr -> local_variable_table.resize(readU2());
        for (int i = 0; i < attr -> local_variable_table.size(); ++i) {
            attr -> local_variable_table[i].start_pc = readU2();
            attr -> local_variable_table[i].length = readU2();
            attr -> local_variable_table[i].name_index = readU2();
            attr -> local_variable_table[i].descriptor_index = readU2();
            attr -> local_variable_table[i].index = readU2();
        }
        return attr;
    }
    if (name == "LocalVariableTypeTable") {
        auto attr = make_shared<LocalVariableTypeTable>();
        attr -> name = name;
        attr -> attribute_length = attribute_length;
        attr -> local_variable_type_table.resize(readU2());
        for (int i = 0; i < attr -> local_variable_type_table.size(); ++i) {
            attr -> local_variable_type_table[i].start_pc = readU2();
            attr -> local_variable_type_table[i].length = readU2();
            attr -> local_variable_type_table[i].name_index = readU2();
            attr -> local_variable_type_table[i].signature_index = readU2();
            attr -> local_variable_type_table[i].index = readU2();
        }
        return attr;
    }
    if (name == "Deprecated") {
        auto attr = make_shared<Deprecated>();
        attr -> name = name;
        attr -> attribute_length = attribute_length;
        return attr;
    }
    if (name == "BootstrapMethods") {
        auto attr = make_shared<BootstrapMethods>();
        attr -> name = name;
        attr -> attribute_length = attribute_length;
        attr -> bootstrap_methods.resize(readU2());
        for (auto &bootstrap_method : attr -> bootstrap_methods) {
            bootstrap_method.bootstrap_method_ref = readU2();
            bootstrap_method.bootstrap_arguments.resize(readU2());
            for (auto &bootstrap_argument : bootstrap_method.bootstrap_arguments) {
                bootstrap_argument = readU2();
            }
        }
        return attr;
    }
    if (name == "MethodParameters") {
        auto attr = make_shared<MethodParameters>();
        attr -> name = name;
        attr -> attribute_length = attribute_length;
        attr -> parameters.resize(readU1());
        for (auto &parameter : attr -> parameters) {
            parameter.name_index = readU2();
            parameter.access_flags = readU2();
        }
        return attr;
    }

    // Custom
    auto attr = make_shared<Custom>();
    attr -> name = name;
    attr -> attribute_length = attribute_length;
    attr -> bytes.resize(attribute_length);
    for (auto &byte : bytes) {
        byte = readU1();
    }
    return attr;
}