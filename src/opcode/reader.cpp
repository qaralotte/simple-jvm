#include "include/opcode/reader.h"

using namespace opcode;

uint Reader::jumpPC(int offset) {
    pc = thread -> getPC() + offset;
    return pc;
}

uint Reader::getPC() const {
    return pc;
}

void Reader::skipPadding() {
    while (pc % 4 != 0) {
        readByte();
    }
}

classfile::u1 Reader::readByte() {
    classfile::u1 byte = code[pc];
    pc += 1;
    return byte;
}

classfile::u2 Reader::readShort() {
    classfile::u1 byte1 = readByte();
    classfile::u1 byte2 = readByte();
    return (byte1 << 8) | byte2;
}

classfile::u4 Reader::readInt() {
    classfile::u2 short1 = readShort();
    classfile::u2 short2 = readShort();
    return (short1 << 16) | short2;
}

vector<classfile::u4> Reader::readInts(uint length) {
    vector<classfile::u4> ints(length);
    for (int i = 0; i < length; ++i) {
        ints[i] = readInt();
    }
    return ints;
}