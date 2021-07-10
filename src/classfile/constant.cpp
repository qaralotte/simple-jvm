#include <numeric>
#include <cmath>

#include "include/classfile/loader.h"
#include "include/log.h"

using namespace classfile;

// 读取常量池中的单个常量
shared_ptr<cp_info> ClassLoader::readConstant() {
    u1 tag = readU1();
    switch (tag) {
        case UTF8: {
            auto info = make_shared<Utf8>();
            u2 length = readU2();
            for (uint i = 0; i < length; ++i) {
                info -> value.push_back((char) readU1());
            }
            info -> tag = UTF8;
            return info;
        }
        case INTEGER: {
            auto info = make_shared<Integer>();
            info -> tag = INTEGER;
            info -> value = (jint) readU4();
            return info;
        }
        case FLOAT: {
            auto info = make_shared<Float>();
            info -> tag = FLOAT;
            u4 bit = readU4();
            if (bit == 0x7F800000) {
                info -> value = numeric_limits<jfloat>::infinity();
            } else if (bit == 0xFF800000) {
                info -> value = -numeric_limits<jfloat>::infinity();
            } else if ((bit > 0x7F800001 && bit < 0x7FFFFFFF) || (bit > 0xFF800001 && bit < 0xFFFFFFFF)) {
                info -> value = numeric_limits<jfloat>::quiet_NaN();
            } else {
                jint s = ((bit >> 31) == 0) ? 1 : -1;
                jint e = (jint) ((bit >> 23) & 0xFF);
                jint m = (e == 0) ?
                         (jint) (bit & 0x7FFFFF) << 1 :
                         (jint) (bit & 0x7FFFFF) | 0x800000;
                info -> value = (jfloat) (s * m * pow(2, e - 150));
            }
            return info;
        }
        case LONG: {
            auto info = make_shared<Long>();
            info -> tag = LONG;
            u4 high_byte = readU4();
            u4 low_byte = readU4();
            info -> value = ((jlong) high_byte << 32) + low_byte;
            return info;
        }
        case DOUBLE: {
            auto info = make_shared<Double>();
            info -> tag = DOUBLE;
            u4 high_byte = readU4();
            u4 low_byte = readU4();
            u8 bit = ((jlong) high_byte << 32) + low_byte;
            if (bit == 0x7FF0000000000000) {
                info -> value = numeric_limits<jdouble>::infinity();
            } else if (bit == 0xFFF0000000000000) {
                info -> value = -numeric_limits<jdouble>::infinity();
            } else if ((bit > 0x7FF0000000000001 && bit < 0x7FFFFFFFFFFFFFFF) || (bit > 0xFFF0000000000001 && bit < 0xFFFFFFFFFFFFFFFF)) {
                info -> value = numeric_limits<jdouble>::quiet_NaN();
            } else {
                jint s = ((bit >> 63) == 0) ? 1 : -1;
                jint e = (jint) ((bit >> 52) & 0x7FFL);
                jlong m = (e == 0) ?
                          (jlong) ((bit & 0xFFFFFFFFFFFFFL) << 1) :
                          (jlong) ((bit & 0xFFFFFFFFFFFFFL) | 0x10000000000000L);
                info -> value = (jdouble) (s * m) * pow(2, e - 1075);
            }
            return info;
        }
        case CLASS: {
            auto info = make_shared<Class>();
            info -> tag = CLASS;
            info -> name_index = readU2();
            return info;
        }
        case STRING: {
            auto info = make_shared<String>();
            info -> tag = STRING;
            info -> string_index = readU2();
            return info;
        }
        case FIELDREF: {
            auto info = make_shared<Field>();
            info -> tag = FIELDREF;
            info -> class_index = readU2();
            info -> name_and_type_index = readU2();
            return info;
        }
        case METHODREF: {
            auto info = make_shared<Method>();
            info -> tag = METHODREF;
            info -> class_index = readU2();
            info -> name_and_type_index = readU2();
            return info;
        }
        case INTERFACE_METHODREF: {
            auto info = make_shared<InterfaceMethodref>();
            info -> tag = INTERFACE_METHODREF;
            info -> class_index = readU2();
            info -> name_and_type_index = readU2();
            return info;
        }
        case NAME_AND_TYPE: {
            auto info = make_shared<NameAndType>();
            info -> tag = NAME_AND_TYPE;
            info -> name_index = readU2();
            info -> descriptor_index = readU2();
            return info;
        }
        case METHOD_HANDLE: {
            auto info = make_shared<MethodHandle>();
            info -> tag = METHOD_HANDLE;
            info -> reference_kind = readU1();
            info -> reference_index = readU2();
            return info;
        }
        case METHOD_TYPE: {
            auto info = make_shared<MethodType>();
            info -> tag = METHOD_TYPE;
            info -> descriptor_index = readU2();
            return info;
        }
        case INVOKE_DYNAMIC: {
            auto info = make_shared<InvokeDynamic>();
            info -> tag = INVOKE_DYNAMIC;
            info -> bootstrap_method_attr_index = readU2();
            info -> name_and_type_index = readU2();
            return info;
        }
        default: {
            ERROR("未知的Constant tag: %u", tag);
            exit(0);
        }
    }
}