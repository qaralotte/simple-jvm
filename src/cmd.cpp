#include <regex>
#include <vector>

#include "include/cmd.h"
#include "include/log.h"

#include "include/runtime/loader.h"
#include "include/runtime/metaspace/string.h"

map<string, bool> Cmd::is_set;

string Cmd::classpath;
ulong Cmd::Xss;

string Cmd::main;
vector<string> Cmd::jargs;

// private:

bool Cmd::startWith(string content, string prefix) {
    if (prefix.size() > content.size()) return false;
    return content.substr(0, prefix.size()) == prefix;
}

bool Cmd::endWith(string content, string suffix) {
    if (suffix.size() > content.size()) return false;
    return content.substr(content.size() - suffix.size(), suffix.size()) == suffix;
}

string Cmd::deleteEnd(string content, string suffix) {
    return content.substr(0, content.size() - suffix.size());
}

void Cmd::setDefault() {
    if (!Cmd::isSet("-Xss")) {
        Cmd::is_set["-Xss"] = true;
        Cmd::Xss = 512 * 1024 * 1024; // 512M
    }
}

// public:
void Cmd::config(const vector<string> &cmds) {
    uint offset = 0;
    /* options */
    while (cmds[offset][0] == '-') {
        // classpath
        if (cmds[offset] == "-classpath" || cmds[offset] == "-cp") {
            offset += 1;
            classpath = cmds[offset];
            is_set["-classpath"] = true;
            is_set["-cp"] = true;
        } else if (startWith(cmds[offset], "-Xss")) {
            auto Xss_raw = cmds[offset].substr(string("-Xss").size(), cmds[offset].size() - string("-Xss").size());
            if (Xss_raw.back() == 'K' || Xss_raw.back() == 'k') {
                Xss = stoi(Xss_raw.substr(0, Xss_raw.size() - 1)) * 1024;
            } else if (Xss_raw.back() == 'M' || Xss_raw.back() == 'm') {
                Xss = stoi(Xss_raw.substr(0, Xss_raw.size() - 1)) * 1024 * 1024;
            } else if (Xss_raw.back() == 'G' || Xss_raw.back() == 'g') {
                Xss = stoi(Xss_raw.substr(0, Xss_raw.size() - 1)) * 1024 * 1024 * 1024;
            } else if (Xss_raw.back() >= '0' && Xss_raw.back() <= '9') {
                Xss = stoi(Xss_raw.substr(0, Xss_raw.size()));
            } else {
                ERROR("未知的参数: -Xss");
                exit(0);
            }
            is_set["-Xss"] = true;
        }

        offset += 1;
    }

    /* file */
    if (endWith(cmds[offset], ".class")) {
        main = deleteEnd(cmds[offset], ".class");
    } else {
        main = cmds[offset];
    }
    is_set["main"] = true;
    offset += 1;
    /* args */
    for (ulong i = offset; i < cmds.size(); ++i) {
        jargs.push_back(cmds[i]);
    }
    setDefault();
}

bool Cmd::isSet(string cmd) {
    if (Cmd::is_set.find(cmd) == Cmd::is_set.end()) return false;
    return Cmd::is_set[cmd];
}

runtime::jobject Cmd::handleJargs() {
    auto string_class = runtime::ClassLoader("java/lang/String").loadClass();
    auto args_class = string_class -> toArray() -> initArray(jargs.size());
    for (int i = 0; i < jargs.size(); ++i) {
        args_class -> array_data[i] = runtime::JString(jargs[i]).toObject();
    }
    return args_class;
}