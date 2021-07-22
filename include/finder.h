#ifndef JVM_LOADER_FINDER_H
#define JVM_LOADER_FINDER_H

#include <string>
#include <map>

#include "include/std.h"
#include "include/classpath.h"
#include "include/libzip/zip.h"

namespace classfinder {

    class ClassFinder {
    private:
        string classname;
        string filename;
    private:
        string mergePath(string, string);
        vector<string> handleClasspath();
        ClassPath findInDir(const string &, bool);
        ClassPath findInJar(const string &);
    public:
        static map<string, zip *> unz_jars;
    public:
        ClassFinder(const string &_classname) : classname(_classname), filename(_classname + ".class") {};
        ClassPath findClass();
    };
}

#endif //JVM_LOADER_FINDER_H
