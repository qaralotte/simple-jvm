#include "include/runtime/loader.h"
#include "include/classfile/loader.h"
#include "include/runtime/metaspace/clazz.h"
#include "include/runtime/metaspace/object.h"
#include "include/runtime/metaspace/string.h"
#include "include/accessflags.h"
#include "include/descriptor.h"
#include "include/atype.h"

#include "include/log.h"

using namespace runtime;

map<string, shared_ptr<Clazz>> ClassLoader::class_map;

/* 加载非数组类的Class */
void ClassLoader::loadNonArrayClass() {
    DEBUG("加载 %s", class_name.c_str());

    /* 搜索 class */
    classfinder::ClassFinder finder(class_name);
    classfile::ClassLoader cloader(finder.findClass());
    auto classfile = cloader.load();

    /* 定义 class */
    auto clazz = make_shared<runtime::Clazz>() -> init(classfile);

    // 如果是Object，则一直加载他的超类
    if (clazz -> this_name != "java/lang/Object") {
        ClassLoader loader(clazz -> super_name);
        clazz -> super_class = loader.loadClass();
    }
    if (!clazz -> interfaces_name.empty()) {
        clazz -> interfaces.resize(clazz -> interfaces_name.size());
        for (uint i = 0; i < clazz -> interfaces_name.size(); ++i) {
            ClassLoader loader(clazz -> interfaces_name[i]);
            clazz -> interfaces[i] = loader.loadClass();
        }
    }
    ClassLoader::class_map[class_name] = clazz;

    /* 链接 class */
    // todo verify
    prepare();
}

void ClassLoader::loadArrayClass() {
    auto clazz = make_shared<Clazz>();
    clazz -> access_flags = ACCESS_PUBLIC;
    clazz -> this_name = class_name;
    clazz -> init_started = true;
    clazz -> super_class = ClassLoader("java/lang/Object").loadClass();
    clazz -> interfaces = {
            ClassLoader("java/lang/Cloneable").loadClass(),
            ClassLoader("java/io/Serializable").loadClass(),
    };
    ClassLoader::class_map[class_name] = clazz;
}

void ClassLoader::prepare() {
    /* 总共slot个数 */
    uint slot_id = 0;
    /* 计算instance个数 */
    if (class_map[class_name] -> super_class != nullptr) {
        slot_id = class_map[class_name] -> super_class -> instance_slot_count;
    }

    /* 计算field个数 */
    uint field_count = 0;
    for (auto field : class_map[class_name] -> fields) {
        if (field -> haveAccess(ACCESS_STATIC)) {
            field -> slot_id = field_count;
            slot_id += 1;
            field_count += 1;
            if (field -> descriptor == DESC_LONG || field -> descriptor == DESC_DOUBLE) {
                slot_id += 1; // LONG 和 DOUBLE 多占一格
                field_count += 1;
            }
        }
    }
    class_map[class_name] -> instance_slot_count = slot_id;
    class_map[class_name] -> static_slot_count = field_count;

    /* 分配空间 */
    class_map[class_name] -> static_vars = VariableTable(class_map[class_name] -> static_slot_count);
    for (auto field : class_map[class_name] -> fields) {
        if (field -> haveAccess(ACCESS_STATIC) && field -> haveAccess(ACCESS_FINAL)) {
            if (field -> constvalue_index > 0) {
                if (field -> descriptor == DESC_BOOLEAN ||
                    field -> descriptor == DESC_BYTE ||
                    field -> descriptor == DESC_CHAR ||
                    field -> descriptor == DESC_SHORT ||
                    field -> descriptor == DESC_INT) {
                    auto val = static_pointer_cast<IntegerConstant>(class_map[class_name] -> constant_pool -> constants[field -> constvalue_index]) -> value;
                    class_map[class_name] -> static_vars.set<jint>(field -> slot_id, val);
                } else if (field -> descriptor == DESC_LONG) {
                    auto val = static_pointer_cast<LongConstant>(class_map[class_name] -> constant_pool -> constants[field -> constvalue_index]) -> value;
                    class_map[class_name] -> static_vars.set<jlong>(field -> slot_id, val);
                } else if (field -> descriptor == DESC_FLOAT) {
                    auto val = static_pointer_cast<FloatConstant>(class_map[class_name] -> constant_pool -> constants[field -> constvalue_index]) -> value;
                    class_map[class_name] -> static_vars.set<jfloat>(field -> slot_id, val);
                } else if (field -> descriptor == DESC_DOUBLE) {
                    auto val = static_pointer_cast<DoubleConstant>(class_map[class_name] -> constant_pool -> constants[field -> constvalue_index]) -> value;
                    class_map[class_name] -> static_vars.set<jdouble>(field -> slot_id, val);
                } else if (field -> descriptor == DESC_CLASS("java/lang/String")) {
                    auto val = static_pointer_cast<StringConstant>(class_map[class_name] -> constant_pool -> constants[field -> constvalue_index]) -> value;
                    auto jstr = JString(val).toObject();
                    class_map[class_name] -> static_vars.set<runtime::jobject>(field -> slot_id, jstr);
                } else {
                    ERROR("未知的 descriptor: %s", field -> descriptor.c_str());
                    exit(0);
                }
            }
        }
    }
}

/* 加载Class */
shared_ptr<Clazz> ClassLoader::loadClass() {
    if (class_map.find(class_name) != class_map.end()) {
        return class_map[class_name];
    }
    if (class_name[0] == '[') loadArrayClass();
    else loadNonArrayClass();
    return class_map[class_name];
}

shared_ptr<Clazz> ClassLoader::loadPrimtiveArrayClass(jint atype) {
    switch (atype) {
        case ATYPE_BOOLEAN: return ClassLoader("[Z").loadClass();
        case ATYPE_BYTE: return ClassLoader("[B").loadClass();
        case ATYPE_CHAR: return ClassLoader("[C").loadClass();
        case ATYPE_SHORT: return ClassLoader("[S").loadClass();
        case ATYPE_INT: return ClassLoader("[I").loadClass();
        case ATYPE_LONG: return ClassLoader("[J").loadClass();
        case ATYPE_FLOAT: return ClassLoader("[F").loadClass();
        case ATYPE_DOUBLE: return ClassLoader("[D").loadClass();
        default:
            ERROR("错误的atype: %d", atype);
            exit(0);
    }
}
