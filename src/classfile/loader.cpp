#include <fstream>

#include "include/log.h"

#include "include/classfile/loader.h"

using namespace classfile;

/* 预设值 */

// private:

/* 读取二进制码并把指针偏移n个单位 */
u1 ClassLoader::readU1() {
    u1 byte = bytes[offset++];
    return byte;
}

u2 ClassLoader::readU2() {
    u1 byte1 = readU1();
    u1 byte2 = readU1();
    return (u2) byte2 | (byte1 << 8);
}

u4 ClassLoader::readU4() {
    u2 byte1 = readU2();
    u2 byte2 = readU2();
    return (u4) byte2 | (byte1 << 16);
}

/* 读取 (this / super) class并返回class名字 */
jstring ClassLoader::readClass() {
    u2 class_index = readU2();
    if (class_index == 0) {
        return "java/lang/Object";
    }
    auto classinfo = ConstantUtils::getConstant<Class>(clazz.constant_pool, class_index);
    auto utf8info = ConstantUtils::getConstant<Utf8>(clazz.constant_pool, classinfo -> name_index);
    return utf8info -> value;
}

/* 读取field和method (两个结构相同) */
field_info ClassLoader::readField() {
    field_info info;
    info.access_flags = readU2();
    info.name = ConstantUtils::getConstant<Utf8>(clazz.constant_pool, readU2()) -> value;
    info.descriptor = ConstantUtils::getConstant<Utf8>(clazz.constant_pool, readU2()) -> value;
    info.attributes.resize(readU2());
    for (auto &attribute : info.attributes) {
        attribute = readAttribute();
    }
    return info;
}

method_info ClassLoader::readMethod() {
    method_info info;
    info.access_flags = readU2();
    info.name = ConstantUtils::getConstant<Utf8>(clazz.constant_pool, readU2()) -> value;
    info.descriptor = ConstantUtils::getConstant<Utf8>(clazz.constant_pool, readU2()) -> value;
    info.attributes.resize(readU2());
    for (auto &attribute : info.attributes) {
        attribute = readAttribute();
    }
    return info;
}

// public:

ClassLoader::ClassLoader(ClassPath path) : class_name(path.class_name) {
    if (path.type == classpath::NOT_FOUND) {
        ERROR("ClassNotFoundException: %s", class_name.c_str());
        exit(0);
    }
    bytes = path.read();
}

/* 加载class文件到缓冲区 */
ClassFile ClassLoader::load() {
    DEBUG("开始读取 %s", class_name.c_str());

    /* magic == 0xCAFEBABE ? */
    if (readU4() != 0xCAFEBABE) {
        ERROR("%s 不是一个有效的class文件", class_name.c_str());
        exit(0);
    }

    /* 判断版本是否为jdk 1.8 (52.0) */
    u2 minor_version = readU2();
    u2 major_version = readU2();
    if (major_version != 52) {
        ERROR("不受支持的版本: %d.%d, 本jvm仅支持jdk 1.8及以下版本", major_version, minor_version);
        exit(0);
    }

    /* 如果版本 < 1.8, 则发出警告 */
    if (major_version < 52) {
        WARNING("版本 < 52.0 (jdk 1.8), 可能一些功能不受支持");
    }

    /* 编译器常量池 */
    clazz.constant_pool.resize(readU2());
    for (int i = 1; i < clazz.constant_pool.size(); ++i) {
        clazz.constant_pool[i] = readConstant();
        if (clazz.constant_pool[i] -> tag == LONG || clazz.constant_pool[i] -> tag == DOUBLE) i += 1;
    }

    /* access_flags */
    clazz.access_flags = readU2();

    /* (this / super) class */
    clazz.this_class = readClass();
    clazz.super_class = readClass();

    /* interface */
    clazz.interfaces.resize(readU2());
    for (auto &interface : clazz.interfaces) {
        auto clz_info = ConstantUtils::getConstant<Class>(clazz.constant_pool, readU2());
        auto utf8_info = ConstantUtils::getConstant<Utf8>(clazz.constant_pool, clz_info -> name_index);
        interface = utf8_info -> value;
    }

    /* field */
    clazz.fields.resize(readU2());
    for (auto &field : clazz.fields) {
        field = readField();
    }

    /* method */
    clazz.methods.resize(readU2());
    for (auto &method : clazz.methods) {
        method = readMethod();
    }

    /* attribute */
    clazz.attributes.resize(readU2());
    for (auto &attribute : clazz.attributes) {
        attribute = readAttribute();
    }

    DEBUG("%s 成功加载到 ClassFile", class_name.c_str());

    return clazz;
}