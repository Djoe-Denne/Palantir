#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <sstream>
#include <string>
#include <string_view>

namespace interview_cheater::utils {
void PlatformLog(std::string_view function, int line, const std::string& message);

template <typename T>
void LogStream(std::ostringstream& stream, const T& value) {
    stream << value;
}

template <typename T, typename... Args>
void LogStream(std::ostringstream& stream, const T& value, const Args&... args) {
    stream << value;
    LogStream(stream, args...);
}
}  // namespace interview_cheater::utils

#define DEBUG_LOG(...)                                                               \
    do {                                                                             \
        std::ostringstream stream;                                                   \
        interview_cheater::utils::LogStream(stream, __VA_ARGS__);                    \
        interview_cheater::utils::PlatformLog(__FUNCTION__, __LINE__, stream.str()); \
    } while (0)

#endif  // LOGGER_HPP