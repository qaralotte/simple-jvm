#include <cstdlib>

#include "include/log.h"

void inFuncLog(const string &file, int line, const char *color, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    printf("%s:%d: %s", file.c_str(), line, color);
    vprintf(fmt, args);
    printf("\033[0m\n");
    va_end(args);
}