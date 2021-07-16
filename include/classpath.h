#ifndef JVM_CLASSPATH_H
#define JVM_CLASSPATH_H

#include <string>
#include <vector>

#include "include/std.h"
#include "include/minizip-ng/unzip.h"

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
    unzFile unz_file;
    string class_name;
    string file_name;
private:
    vector<uchar> readInDir();
    vector<uchar> readInJar();
public:
    ClassPath() = default;
    ClassPath(classpath::PathType _type) : type(_type) {};
    ClassPath(classpath::PathType _type, string _path, unzFile _unz, string _class_name, string _file_name) :
            type(_type), path(_path), unz_file(_unz), class_name(_class_name), file_name(_file_name) {};
    vector<uchar> read();
};


#endif //JVM_CLASSPATH_H
