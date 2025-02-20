#ifndef LOGGER_HPP
#define LOGGER_HPP

// Declaration only
void platform_log(const char* function, int line, const char* format, ...);

#define DEBUG_LOG(format, ...)

#endif // LOGGER_HPP 