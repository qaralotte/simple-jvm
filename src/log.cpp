//
// Created by qaralotte on 2020/12/12.
//

#include <cstdarg>
#include <cstdio>
#include <cstdlib>

#include "include/log.h"

void makeDebug(const char *file, int line, const char *format, ...) {
    va_list args_list;
    va_start(args_list, format);
    printf("\033[1;33m[Debug]\033[0m %s:%d: ", file, line);
    vprintf(format, args_list);
    putchar('\n');
    va_end(args_list);
}

void makeError(const char *file, int line, const char *format, ...) {
    va_list args_list;
    va_start(args_list, format);
    printf("\033[1;31m[Error]\033[0m %s:%d: ", file, line);
    vprintf(format, args_list);
    putchar('\n');
    va_end(args_list);
    exit(1);
}