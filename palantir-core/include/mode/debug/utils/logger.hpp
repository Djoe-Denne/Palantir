#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <sstream>
#include <string>
#include <string_view>
#include <source_location>

#include "core_export.hpp"

namespace palantir::utils {

void PALANTIR_CORE_API PlatformLog(std::string_view function, int line, const std::string& message);

template <typename T, typename... Args>
auto PALANTIR_CORE_API LogStream(std::ostringstream& stream, const T& value, const Args&... args) -> void {
    stream << value;  // NOLINT
    (stream << ... << args);
}
}  // namespace palantir::utils

template <typename... Args>
auto PALANTIR_CORE_API DebugLog(const std::source_location& location, const Args&... args) -> void {
    std::ostringstream stream;
    palantir::utils::LogStream(stream, args...);
    palantir::utils::PlatformLog(location.function_name(), location.line(), stream.str());
}

template <typename... Args>
auto PALANTIR_CORE_API DebugLog(const Args&... args) -> void {
    DebugLog(std::source_location::current(), args...);
}

#endif  // LOGGER_HPP