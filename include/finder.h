#ifndef JVM_LOADER_FINDER_H
#define JVM_LOADER_FINDER_H

#include <string>

#include "include/std.h"

namespace classfinder {
    class ClassFinder {
    private:
        string classname;
    private:
        vector<uchar> readBinary(const string &);
        string mergePath(string, string);
        vector<string> handleClasspath();
        vector<uchar> findInDir(const string &, bool);
        vector<uchar> findInJar(const string &);
    public:
        ClassFinder(const string &_classname) : classname(_classname + ".class") {};
        vector<uchar> findClass();
    };
}

#endif //JVM_LOADER_FINDER_H
