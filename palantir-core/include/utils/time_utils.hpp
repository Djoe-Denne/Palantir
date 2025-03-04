#pragma once

#include <chrono>
#include <ctime>

namespace palantir::utils {

inline auto safe_localtime(const std::time_t* time, std::tm* tm_buf) -> void {
#ifdef _WIN32
    localtime_s(tm_buf, time);
#else
    localtime_r(time, tm_buf);
#endif
}

} // namespace palantir::utils 