#ifndef LOGGER_HPP
#define LOGGER_HPP

// Declaration only
void platform_log(const char* function, int line, const char* format, ...);

#define DEBUG_LOG(format, ...) platform_log(__FUNCTION__, __LINE__, format, ##__VA_ARGS__)

#endif  // LOGGER_HPP