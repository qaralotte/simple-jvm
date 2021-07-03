#ifndef JVM_CMD_H
#define JVM_CMD_H

#include <vector>
#include <map>

#include "std.h"
#include "config.h"

class Cmd {
private:
    static map<string, string> mapper;
private:
    static string get(string);
public:
    static string classpath;
    static uint Xss;
public:
    Cmd() = delete;
    static void config(vector<string>);
};

#endif //JVM_CMD_H
