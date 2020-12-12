//
// Created by qaralotte on 2020/12/12.
//

#ifndef SIMPLE_JVM_LOG_H
#define SIMPLE_JVM_LOG_H

#include <cstdarg>

#define ASSERT(con, fmt, ...) con ? [](){}() : ERROR(fmt, ## __VA_ARGS__)

#define DEBUG(fmt, ...) makeDebug(__FILE__, __LINE__, fmt, ## __VA_ARGS__)
#define ERROR(fmt, ...) makeError(__FILE__, __LINE__, fmt, ## __VA_ARGS__)

void makeDebug(const char *, int, const char *, ...);
__attribute__((noreturn)) void makeError(const char *, int, const char *, ...);

#endif //SIMPLE_JVM_LOG_H
