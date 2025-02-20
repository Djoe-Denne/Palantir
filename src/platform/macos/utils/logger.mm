#include "utils/logger.hpp"
#import <Foundation/Foundation.h>
#include <cstdarg>
#include <cstdio>

void platform_log(const char* function, int line, const char* format, ...) {
    va_list args;
    va_start(args, format);

    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), format, args);

    va_end(args);

    NSLog(@"%s:%d - %s", function, line, buffer);
}