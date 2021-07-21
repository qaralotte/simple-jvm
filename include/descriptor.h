#ifndef JVM_DESCRIPTOR_H
#define JVM_DESCRIPTOR_H

#include <string>

#include "std.h"

const string DESC_VOID =        "V";
const string DESC_BYTE =        "B";
const string DESC_CHAR =        "C";
const string DESC_DOUBLE =      "D";
const string DESC_FLOAT =       "F";
const string DESC_INT =         "I";
const string DESC_LONG =        "J";
const string DESC_SHORT =       "S";
const string DESC_BOOLEAN =     "Z";
inline string PRIM_TO_DESC(string name) {
    if (name == "void") return "V";
    if (name == "byte") return "B";
    if (name == "char") return "C";
    if (name == "double") return "D";
    if (name == "float") return "F";
    if (name == "int") return "I";
    if (name == "long") return "J";
    if (name == "short") return "S";
    if (name == "boolean") return "Z";
    return "";
}
inline string DESC_TO_PRIM(string desc) {
    if (desc == "V") return "void";
    if (desc == "B") return "byte";
    if (desc == "C") return "char";
    if (desc == "D") return "double";
    if (desc == "F") return "float";
    if (desc == "I") return "int";
    if (desc == "J") return "long";
    if (desc == "S") return "short";
    if (desc == "Z") return "boolean";
    return "";
}
const char DESC_ARRAY_HEAD =    '[';
const char DESC_CLASS_HEAD =    'L';
inline string DESC_ARRAY(string name) {return "[" + name;}
inline string DESC_CLASS(string name) {return "L" + name + ";";}
inline string DESC(string name) {
    if (name[0] == '[') return name;
    auto prim = PRIM_TO_DESC(name);
    if (!prim.empty()) {
        return prim;
    }
    return "L" + name + ";";
}

#endif //JVM_DESCRIPTOR_H
