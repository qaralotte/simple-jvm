#ifndef JVM_CMD_H
#define JVM_CMD_H

#include <vector>
#include <map>

#include "std.h"
#include "config.h"

#include "include/runtime/metaspace/object.h"

class Cmd {
private:
    static map<string, bool> is_set;
private:
    static bool startWith(string, string);
    static bool endWith(string, string);
    static string deleteEnd(string, string);
    static void setDefault();
public:
    /* options */
    static string classpath;
    static ulong Xss;
    /* files */
    static string main;
    /* args */
    static vector<string> jargs;
public:
    Cmd() = delete;
    static void config(const vector<string> &);
    static bool isSet(string);
    static runtime::jobject handleJargs();
};

#endif //JVM_CMD_H
