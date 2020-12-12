//
// Created by qaralotte on 2020/12/12.
//

#ifndef SIMPLE_JVM_CONSTANT_H
#define SIMPLE_JVM_CONSTANT_H

#include <string>

#include "include/type.h"

namespace classfile {
    namespace constant {
        enum {
            UTF8 = 1,
            INTEGER = 3,
            FLOAT,
            LONG,
            DOUBLE,
            CLASS,
            STRING,
            FIELDREF,
            METHODREF,
            INTERFACE_METHODREF,
            NAME_AND_TYPE,
            METHOD_HANDLE = 15,
            METHOD_TYPE,
            INVOKE_DYNAMIC = 18
        };

        struct cp_info {
            ubyte tag;
        };

        struct Class_info : cp_info {
            ushort name_index;
        };

        struct Fieldref_info : cp_info {
            ushort class_index;
            ushort name_and_type_index;
        };

        struct Methodref_info : cp_info {
            ushort class_index;
            ushort name_and_type_index;
        };

        struct InterfaceMethodref_info : cp_info {
            ushort class_index;
            ushort name_and_type_index;
        };

        struct String_info : cp_info {
            ushort string_index;
        };

        struct Integer_info : cp_info {
            uint bytes;
        };

        struct Float_info : cp_info {
            uint bytes;
        };

        struct Long_info : cp_info {
            uint high_bytes;
            uint low_bytes;
        };

        struct Double_info : cp_info {
            uint high_bytes;
            uint low_bytes;
        };

        struct NameAndType_info : cp_info {
            ushort name_index;
            ushort decriptor_index;
        };

        struct Utf8_info : cp_info {
            uint length;
            string bytes;
        };

        struct MethodHandle_info : cp_info {
            ubyte reference_kind;
            ushort reference_index;
        };

        struct MethodType_info : cp_info {
            ushort decriptor_index;
        };

        struct InvokeDynamic_info : cp_info {
            ushort bootstrap_method_attr_index;
            ushort name_and_type_index;
        };
    }
}

#endif //SIMPLE_JVM_CONSTANT_H
