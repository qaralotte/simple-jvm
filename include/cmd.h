#ifndef JVM_CMD_H
#define JVM_CMD_H

#include <vector>
#include <map>

#include "std.h"
#include "config.h"

class Cmd {
public:
    static map<string, string> mapper;
public:
    Cmd() = delete;
public:
    static void config(vector<string>);
    static string get(string);
};

#endif //JVM_CMD_H
