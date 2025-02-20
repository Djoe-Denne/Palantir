#include "utils/logger.hpp"
#include <Windows.h>
#include <sstream>

namespace interview_cheater::utils {

auto PlatformLog(const char* function, int line, const std::string& message) -> void {
    std::ostringstream finalStream;
    finalStream << "[" << function << ":" << line << "] " << message << "\n";
    OutputDebugStringA(finalStream.str().c_str());
}

} // namespace interview_cheater::utils