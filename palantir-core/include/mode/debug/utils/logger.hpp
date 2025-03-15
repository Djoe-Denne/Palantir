#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <sstream>
#include <string>
#include <string_view>

#include "core_export.hpp"

namespace palantir::utils {

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
}  // namespace palantir::utils

#define DEBUG_LOG(...)                                                      \
    do {                                                                    \
        std::ostringstream stream;                                          \
        palantir::utils::LogStream(stream, __VA_ARGS__);                    \
        palantir::utils::PlatformLog(__FUNCTION__, __LINE__, stream.str()); \
    } while (0)

#endif  // LOGGER_HPP
