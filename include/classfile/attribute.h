#ifndef JVM_CLASSFILE_ATTRIBUTE_H
#define JVM_CLASSFILE_ATTRIBUTE_H

#include <vector>

#include "std.h"
#include "include/std.h"

namespace classfile {

    /* 在结构体里需要用到的结构体 */
    namespace code {
        struct exception_table {
            u2 start_pc;
            u2 end_pc;
            u2 handle_pc;
            u2 catch_type;
        };
    }

    namespace stack_map_table {
        enum {
            ITEM_TOP,
            ITEM_INTEGER,
            ITEM_FLOAT,
            ITEM_DOUBLE,
            ITEM_LONG,
            ITEM_NULL,
            ITEM_UNINITIALIZED_THIS,
            ITEM_OBJECT,
            ITEM_UNINITIALIZED
        };

        struct verification_type_info {
            u1 tag;
            union {
                u2 cpool_index;     // Object
                u2 offset;          // Uninitialized
            };
        };

        enum {
            SAME,
            SAME_LOCALS_1_STACK_ITEM,
            SAME_LOCALS_1_STACK_ITEM_EXTENDED,
            CHOP,
            SAME_FRAME_EXTENDED,
            APPEND,
            FULL_FRAME
        };

        struct stack_map_frame {
            u1 frame_type;
            struct {
                u2 offset_delta;
                vector<verification_type_info> locals;
                vector<verification_type_info> stack;
            };
        };
    }

    namespace inner_classes {
        struct clazz {
            u2 inner_class_info_index;
            u2 outer_class_info_index;
            u2 inner_name_index;
            u2 inner_class_access_flags;
        };
    }

    namespace line_number_table {
        struct line_number_table {
            u2 start_pc;
            u2 line_number;
        };
    }

    namespace local_variable_table {
        struct local_variable_table {
            u2 start_pc;
            u2 length;
            u2 name_index;
            u2 descriptor_index;
            u2 index;
        };
    }

    namespace local_variable_type_table {
        struct local_variable_type_table {
            u2 start_pc;
            u2 length;
            u2 name_index;
            u2 signature_index;
            u2 index;
        };
    }

    namespace annotation {

        struct element_value {
            u1 tag;
        };

        struct element_value_pair {
            u2 element_name_index;
            shared_ptr<element_value> value;
        };

        struct annotation {
            u2 type_index;
            vector<element_value_pair> element_value_pairs;
        };

        struct const_value : element_value {
            u2 index;
        };

        struct enum_const_value : element_value {
            u2 type_name_index;
            u2 const_name_index;
        };

        struct class_info : public element_value {
            u2 index;
        };

        struct annotation_value : public element_value {
            annotation value;
        };

        struct array_value : public element_value {
            vector<shared_ptr<element_value>> value;
        };

        struct path {
            u1 type_path_kind;
            u1 type_argument_index;
        };

        struct target {
            u1 target_type;
        };

        struct type_parameter_target : target {
            u1 type_parameter_index;
        };

        struct supertype_target : target {
            u2 supertype_index;
        };

        struct type_parameter_bound_target : target {
            u1 type_parameter_index;
            u1 bound_index;
        };

        struct empty_target : target {
            // nothing
        };

        struct formal_parameter_target : target {
            u1 formal_parameter_index;
        };

        struct throws_target : target {
            u2 throws_type_index;
        };

        struct table {
            u2 start_pc;
            u2 length;
            u2 index;
        };

        struct localvar_target : target {
            vector<table> table;
        };

        struct catch_target : target {
            u2 exception_table_index;
        };

        struct offset_target : target {
            u2 offset;
        };

        struct type_argument_target : target {
            u2 offset;
            u1 type_argument_index;
        };

        struct type_annotation {
            shared_ptr<target> target;
            path type_path;
            u2 type_index;
            vector<element_value_pair> element_value_pairs;
        };
    }

    namespace bootstrap_methods {
        struct bootstrap_methods {
            u2 bootstrap_method_ref;
            vector<u2> bootstrap_arguments;
        };
    }

    namespace method_parameters {
        struct parameters {
            u2 name_index;
            u2 access_flags;
        };
    }

    /* 属性结构体 */
    struct attribute_info {
        string name;
        u4 attribute_length;
    };

    struct Custom : attribute_info {
        vector<u1> bytes;
    };

    struct ConstantValue : attribute_info {
        u2 constantvalue_index;
    };

    struct Code : attribute_info {
        u2 max_stack;
        u2 max_locals;
        vector<u1> code;
        vector<code::exception_table> exception_table;
        vector<shared_ptr<attribute_info>> attributes;
    };

    struct StackMapTable : attribute_info {
        vector<stack_map_table::stack_map_frame> entries;
    };

    struct Exceptions : attribute_info {
        vector<u2> exception_index_table;
    };

    struct InnerClasses : attribute_info {
        vector<inner_classes::clazz> classes;
    };

    struct EnclosingMethod : attribute_info {
        u2 class_index;
        u2 method_index;
    };

    struct Synthetic : attribute_info {
        // nothing
    };

    struct Signature : attribute_info {
        u2 signature_index;
    };

    struct SourceFile : attribute_info {
        u2 sourcefile_index;
    };

    struct SourceDebugExtension : attribute_info {
        vector<u1> debug_extension;
    };

    struct LineNumberTable : attribute_info {
        vector<line_number_table::line_number_table> line_number_table;
    };

    struct LocalVariableTable : attribute_info {
        vector<local_variable_table::local_variable_table> local_variable_table;
    };

    struct LocalVariableTypeTable : attribute_info {
        vector<local_variable_type_table::local_variable_type_table> local_variable_type_table;
    };

    struct Deprecated : attribute_info {
        // nothing
    };

    struct RuntimeVisibleAnnotations : attribute_info {
        vector<annotation::annotation> annotations;
    };

    struct RuntimeInvisibleAnnotations : attribute_info {
        vector<annotation::annotation> annotations;
    };

    struct RuntimeVisibleParameterAnnotations : attribute_info {
        vector<vector<annotation::annotation>> parameter_annotations;
    };

    struct RuntimeInvisibleParameterAnnotations : attribute_info {
        vector<vector<annotation::annotation>> parameter_annotations;
    };

    struct RuntimeVisibleTypeAnnotations : attribute_info {
        vector<annotation::type_annotation> annotations;
    };

    struct RuntimeInvisibleTypeAnnotations : attribute_info {
        vector<annotation::type_annotation> annotations;
    };

    struct AnnotationDefault : attribute_info {
        shared_ptr<annotation::element_value> default_value;
    };

    struct BootstrapMethods : attribute_info {
        vector<bootstrap_methods::bootstrap_methods> bootstrap_methods;
    };

    struct MethodParameters : attribute_info {
        vector<method_parameters::parameters> parameters;
    };

}

#endif //JVM_CLASSFILE_ATTRIBUTE_H
