#include "utils/logger.hpp"
#include <windows.h>
#include <cstdarg>
#include <cstdio>

void platform_log(const char* function, int line, const char* format, ...) {
    // Buffer for the formatted message
    char formatted_msg[1024];

    // Format the variadic arguments
    va_list args;
    va_start(args, format);
    vsnprintf(formatted_msg, sizeof(formatted_msg), format, args);
    va_end(args);

    // Buffer for the final message with function and line
    char final_msg[1024];
    snprintf(final_msg, sizeof(final_msg), "%s:%d - %s\n", function, line, formatted_msg);

    // Output to debug console
    OutputDebugStringA(final_msg);
}