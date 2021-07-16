#include <regex>

#include "include/cmd.h"

map<string, string> Cmd::mapper;
string Cmd::classpath;
uint Cmd::Xss;

// private:
string Cmd::get(string key) {
    if (mapper.find(key) == mapper.end()) return string();
    return mapper[key];
}

// public:
void Cmd::config(vector<string> cmd_vec) {
    for (string cmd : cmd_vec) {
        if (cmd.find('=') == string::npos) continue; // 无效命令
        regex re("=");
        auto keyvalue = vector<string> {
                sregex_token_iterator(cmd.begin(), cmd.end(), re, -1),
                sregex_token_iterator()
        };
        mapper[keyvalue[0]] = keyvalue[1];
    }

    // classpath:
    classpath = get("-classpath");
    Xss = stoi(get("-Xss")); // todo 自动转化
}
