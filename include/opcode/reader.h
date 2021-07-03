#ifndef JVM_OPCODE_READER_H
#define JVM_OPCODE_READER_H

#include <vector>

#include "include/std.h"
#include "include/classfile/std.h"

#include "include/runtime/thread.h"

namespace opcode {
    class Reader {
    private:
        runtime::JVMThread *self_thread;
        vector<classfile::u1> code;
        uint pc;
    public:
        Reader(runtime::JVMThread *_thread, vector<classfile::u1> _code) : self_thread(_thread), code(_code), pc(0) {};
        uint jumpPC(int);
        uint getPC();
    public:
        classfile::u1 readByte();
        classfile::u2 readShort();
        classfile::u4 readInt();
    };
}

#endif //JVM_OPCODE_READER_H
