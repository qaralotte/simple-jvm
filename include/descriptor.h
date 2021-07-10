#ifndef JVM_DESCRIPTOR_H
#define JVM_DESCRIPTOR_H

#include <string>

#include "std.h"

const string DESC_BYTE =        "B";
const string DESC_CHAR =        "C";
const string DESC_DOUBLE =      "D";
const string DESC_FLOAT =       "F";
const string DESC_INT =         "I";
const string DESC_LONG =        "J";
const string DESC_SHORT =       "S";
const string DESC_BOOLEAN =     "Z";
const char DESC_ARRAY_HEAD =    '[';
const char DESC_CLASS_HEAD =    'L';
inline string DESC_ARRAY(string name) {return "[" + name;}
inline string DESC_CLASS(string name) {return "L" + name;}

#endif //JVM_DESCRIPTOR_H
