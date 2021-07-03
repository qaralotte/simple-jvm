#ifndef JVM_LOADER_FINDER_H
#define JVM_LOADER_FINDER_H

#include <string>

#include "include/std.h"
#include "include/classpath.h"

namespace classfinder {

    class ClassFinder {
    private:
        string classname;
    private:
        string mergePath(string, string);
        vector<string> handleClasspath();
        ClassPath findInDir(const string &, bool);
        ClassPath findInJar(const string &);
    public:
        ClassFinder(const string &_classname) : classname(_classname + ".class") {};
        ClassPath findClass();
    };
}

#endif //JVM_LOADER_FINDER_H
