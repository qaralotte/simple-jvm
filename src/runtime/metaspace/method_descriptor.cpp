#include "include/runtime/metaspace/method.h"
#include "include/log.h"

using namespace runtime;

// private:
char MethodDescriptor::nextChar() {
    if (offset == descriptor.size()) {
        ERROR("bad descriptor: %s", descriptor.c_str());
        exit(0);
    }
    return descriptor[offset++];
}

void MethodDescriptor::parseParam() {
    if (nextChar() != '(') {
        ERROR("bad descriptor: %s", descriptor.c_str());
        exit(0);
    }

    auto ch = nextChar();
    while (ch != ')') {
        parameter_types.push_back(parseType(ch));
        ch = nextChar();
    }
}

void MethodDescriptor::parseReturn() {
    return_type = parseType(nextChar());
}

string MethodDescriptor::parseType(char type) {
    switch (type) {
        case 'B':
        case 'C':
        case 'D':
        case 'F':
        case 'I':
        case 'J':
        case 'S':
        case 'Z':
        case 'V':
            return {type};
        case 'L':
            return 'L' + parseObject() + ';';
        case '[':
            return '[' + parseType(nextChar());
        default:
            ERROR("bad descriptor: %s", descriptor.c_str());
            exit(0);
    }
}

string MethodDescriptor::parseObject() {
    string object;
    char ch = nextChar();
    while (ch != ';') {
        object.push_back(ch);
        ch = nextChar();
    }
    return object;
}

// public:
void MethodDescriptor::parse() {
    parseParam();
    parseReturn();
}
