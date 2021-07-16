#ifndef JVM_OPCODE_READER_H
#define JVM_OPCODE_READER_H

#include <vector>

#include "include/std.h"
#include "include/classfile/std.h"

#include "include/runtime/thread.h"

namespace opcode {
    class Reader {
    private:
        vector<classfile::u1> code;
        uint pc;
        shared_ptr<runtime::JVMThread> thread;
    public:
        Reader(shared_ptr<runtime::JVMThread> _thread, vector<classfile::u1> _code) : thread(_thread), code(_code), pc(0) {};
        uint jumpPC(int);
        uint getPC() const;
    public:
        classfile::u1 readByte();
        classfile::u2 readShort();
        classfile::u4 readInt();
    };
}

#endif //JVM_OPCODE_READER_H
