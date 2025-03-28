#include "utils/logger.hpp"

#include <Windows.h>

#include <sstream>

namespace palantir::utils {

auto PlatformLog(std::string_view function, int line, const std::string& message) -> void {
    std::ostringstream finalStream;
    finalStream << "[" << function << ":" << line << "] " << message << "\n";
    OutputDebugStringA(finalStream.str().c_str());
}

}  // namespace palantir::utils