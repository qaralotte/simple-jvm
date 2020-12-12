//
// Created by qaralotte on 2020/12/12.
//

#include <fstream>
#include <algorithm>

#include "include/jvm/engine/loader.h"
#include "include/log.h"
#include "include/jvm/classfile/classfile.h"

// private
void ClassLoader::next() {
    ASSERT(cursor < bytes.size(), "ClassFile 读取超出大小限制");
    cursor += 1;
}

ubyte ClassLoader::readU1() {
    ubyte ubyte1 = bytes[cursor];
    next();
    return ubyte1;
}

ushort ClassLoader::readU2() {
    ubyte ubyte1 = readU1();
    ubyte ubyte2 = readU1();
    return (ushort) ubyte2 | (ubyte1 << 8);
}

uint ClassLoader::readU4() {
    ushort ushort1 = readU2();
    ushort ushort2 = readU2();
    return (uint) ushort2 | (ushort1 << 16);
}

classfile::constant::cp_info *ClassLoader::readConstant() {
    using namespace classfile::constant;

    ubyte tag = readU1();
    switch (tag) {
        case UTF8: {
            auto constant = new Utf8_info;
            constant -> tag = tag;
            constant -> length = readU2();
            constant -> bytes.resize(constant -> length);
            for (int i = 0; i < constant -> length; ++i) {
                constant -> bytes[i] = readU1();
            }
            return constant;
        }
        case INTEGER: {
            auto constant = new Integer_info;
            constant -> tag = tag;
            constant -> bytes = readU4();
            return constant;
        }
        case FLOAT: {
            auto constant = new Float_info;
            constant -> tag = tag;
            constant -> bytes = readU4();
            return constant;
        }
        case LONG: {
            auto constant = new Long_info;
            constant -> tag = tag;
            constant -> high_bytes = readU4();
            constant -> low_bytes = readU4();
            return constant;
        }
        case DOUBLE: {
            auto constant = new Double_info;
            constant -> tag = tag;
            constant -> high_bytes = readU4();
            constant -> low_bytes = readU4();
            return constant;
        }
        case CLASS: {
            auto constant = new Class_info;
            constant -> tag = tag;
            constant -> name_index = readU2();
            return constant;
        }
        case STRING: {
            auto constant = new String_info;
            constant -> tag = tag;
            constant -> string_index = readU2();
            return constant;
        }
        case FIELDREF: {
            auto constant = new Fieldref_info;
            constant -> tag = tag;
            constant -> class_index = readU2();
            constant -> name_and_type_index = readU2();
            return constant;
        }
        case METHODREF: {
            auto constant = new Methodref_info;
            constant -> tag = tag;
            constant -> class_index = readU2();
            constant -> name_and_type_index = readU2();
            return constant;
        }
        case INTERFACE_METHODREF: {
            auto constant = new InterfaceMethodref_info;
            constant -> tag = tag;
            constant -> class_index = readU2();
            constant -> name_and_type_index = readU2();
            return constant;
        }
        case NAME_AND_TYPE: {
            auto constant = new NameAndType_info;
            constant -> tag = tag;
            constant -> name_index = readU2();
            constant -> decriptor_index = readU2();
            return constant;
        }
        case METHOD_HANDLE: {
            auto constant = new MethodHandle_info;
            constant -> tag = tag;
            constant -> reference_kind = readU1();
            constant -> reference_index = readU2();
            return constant;
        }
        case METHOD_TYPE: {
            auto constant = new MethodType_info;
            constant -> tag = tag;
            constant -> decriptor_index = readU2();
            return constant;
        }
        case INVOKE_DYNAMIC: {
            auto constant = new InvokeDynamic_info;
            constant -> tag = tag;
            constant -> bootstrap_method_attr_index = readU2();
            constant -> name_and_type_index = readU2();
            return constant;
        }
        default:
            ERROR("constant 未知的tag: %d", tag);
    }
}

// public
ClassLoader::ClassLoader(const string &path) {
    cursor = 0;
    ifstream in(path, ios::binary);
    in.seekg(0, ios::end);
    bytes.resize(in.tellg());
    in.seekg(0);
    in.read(reinterpret_cast<char*>(bytes.data()), bytes.size());
    in.close();
}

classfile::ClassFile ClassLoader::load() {
    classfile::ClassFile clazz{};
    clazz.magic = readU4();
    clazz.minor_version = readU2();
    clazz.major_version = readU2();
    clazz.constant_pool_count = readU2();
    clazz.constant_pool.resize(clazz.constant_pool_count);
    for (int i = 1; i < clazz.constant_pool_count; ++i) {
        clazz.constant_pool[i] = readConstant();
    }
    return clazz;
}

void ClassLoader::release(classfile::ClassFile &clazz) {
    for (int i = 0; i < clazz.constant_pool_count; ++i) {
        delete clazz.constant_pool[i];
    }
}
