#ifndef JVM_ACCESSFLAGS_H
#define JVM_ACCESSFLAGS_H

#include "include/std.h"

const uint16 ACCESS_PUBLIC =           0x0001;
const uint16 ACCESS_PRIVATE =          0x0002;
const uint16 ACCESS_PROTECTED =        0x0004;
const uint16 ACCESS_STATIC =           0x0008;
const uint16 ACCESS_FINAL =            0x0010;
const uint16 ACCESS_SUPER =            0x0020;
const uint16 ACCESS_SYNCHRONIZED =     0x0020;
const uint16 ACCESS_VOLATILE =         0x0040;
const uint16 ACCESS_BRIDGE =           0x0040;
const uint16 ACCESS_TRANSIENT =        0x0080;
const uint16 ACCESS_VARAGES =          0x0080;
const uint16 ACCESS_NATIVE =           0x0100;
const uint16 ACCESS_INTERFACE =        0x0200;
const uint16 ACCESS_ABSTRACT =         0x0400;
const uint16 ACCESS_STRICT =           0x0800;
const uint16 ACCESS_SYNTHETIC =        0x1000;
const uint16 ACCESS_ANNOTATION =       0x2000;
const uint16 ACCESS_ENUM =             0x2000;

#endif //JVM_ACCESSFLAGS_H
