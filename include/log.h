#ifndef JVM_LOG_H
#define JVM_LOG_H

#include <iostream>
#include <string>
#include <cstdarg>
#include <functional>

#include "std.h"

#define DEBUG(fmt, ...) inFuncLog(__FILE__, __LINE__, "\033[0;32m", (fmt), ##__VA_ARGS__)
#define ERROR(fmt, ...) inFuncLog(__FILE__, __LINE__, "\033[0;31m", (fmt), ##__VA_ARGS__)
#define WARNING(fmt, ...) inFuncLog(__FILE__, __LINE__, "\033[0;33m", (fmt), ##__VA_ARGS__)

void inFuncLog(const string &file, int line, const char *color, const char *fmt, ...);

#endif //JVM_LOG_H
