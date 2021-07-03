#include "include/opcode/reader.h"

using namespace opcode;

uint Reader::jumpPC(int offset) {
    pc = self_thread -> getPC() + offset;
    return pc;
}

uint Reader::getPC() {
    return pc;
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