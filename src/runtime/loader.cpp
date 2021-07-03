#include "include/runtime/loader.h"
#include "include/classfile/loader.h"
#include "include/runtime/metaspace/clazz.h"

#include "include/log.h"

using namespace runtime;

map<string, Clazz> ClassLoader::class_map;

/* 加载非数组类的Class */
Clazz ClassLoader::loadNonArrayClass(string class_name) {
    DEBUG("开始加载 %s.class", class_name.c_str());

    /* 搜索 class */
    classfinder::ClassFinder finder(class_name);
    classfile::ClassLoader cloader(finder.findClass());
    auto classfile = cloader.load();

    /* 定义 class */
    Clazz rclazz = runtime::Clazz(classfile);

    // 如果是Object，则一直加载他的超类
    if (rclazz.this_name == "java/lang/Object") {
        rclazz.super_class = make_shared<Clazz>(loadClass(rclazz.super_name));
    }
    if (!rclazz.interfaces_name.empty()) {
        rclazz.interfaces.resize(rclazz.interfaces_name.size());
        for (uint i = 0; i < rclazz.interfaces_name.size(); ++i) {
            rclazz.interfaces[i] = loadClass(rclazz.interfaces_name[i]);
        }
    }
    class_map[class_name] = rclazz;

    /* 链接 class */


    DEBUG("%s.class 加载完成", class_name.c_str());
    return rclazz;
}

/* 加载Class */
Clazz ClassLoader::loadClass(string class_name) {
    if (class_map.find(class_name) != class_map.end()) {
        return class_map[class_name];
    }
    return loadNonArrayClass(class_name);
}
