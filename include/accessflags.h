#ifndef JVM_ACCESSFLAGS_H
#define JVM_ACCESSFLAGS_H

#include "include/std.h"

const uint16 ACC_PUBLIC =           0x0001;
const uint16 ACC_PRIVATE =          0x0002;
const uint16 ACC_PROTECTED =        0x0004;
const uint16 ACC_STATIC =           0x0008;
const uint16 ACC_FINAL =            0x0010;
const uint16 ACC_SUPER =            0x0020;
const uint16 ACC_SYNCHRONIZED =     0x0020;
const uint16 ACC_VOLATILE =         0x0040;
const uint16 ACC_BRIDGE =           0x0040;
const uint16 ACC_TRANSIENT =        0x0080;
const uint16 ACC_VARAGES =          0x0080;
const uint16 ACC_NATIVE =           0x0100;
const uint16 ACC_INTERFACE =        0x0200;
const uint16 ACC_ABSTRACT =         0x0400;
const uint16 ACC_STRICT =           0x0800;
const uint16 ACC_SYNTHETIC =        0x1000;
const uint16 ACC_ANNOTATION =       0x2000;
const uint16 ACC_ENUM =             0x2000;

#endif //JVM_ACCESSFLAGS_H
