#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <sstream>
#include <string>
#include <string_view>
#include "core_export.hpp"

namespace interview_cheater::utils {

void PALANTIR_CORE_API PlatformLog(std::string_view function, int line, const std::string& message);

template <typename T>
void PALANTIR_CORE_API LogStream(std::ostringstream& stream, const T& value) {
    stream << value;  // NOLINT (hicpp-no-array-decay)
}

template <typename T, typename... Args>
void PALANTIR_CORE_API LogStream(std::ostringstream& stream, const T& value, const Args&... args) {
    stream << value;  // NOLINT
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
