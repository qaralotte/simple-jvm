#ifndef JVM_CLASSPATH_H
#define JVM_CLASSPATH_H

#include <string>
#include <vector>

#include "include/std.h"

namespace classpath {
    enum PathType {
        NOT_FOUND,
        JAR,
        DIR,
    };
}

class ClassPath {
public:
    classpath::PathType type;
    string path;
    string class_name;
private:
    vector<uchar> readInDir();
    vector<uchar> readInJar();
public:
    ClassPath() = default;
    ClassPath(classpath::PathType _type) : type(_type) {};
    ClassPath(classpath::PathType _type, string _path, string _class_name) : type(_type), path(_path), class_name(_class_name) {};
    vector<uchar> read();
};


#endif //JVM_CLASSPATH_H
