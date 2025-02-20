#include "utils/logger.hpp"
#import <Foundation/Foundation.h>
#include <sstream>

namespace interview_cheater::utils {

auto PlatformLog(const char* function, int line, const std::string& message) -> void {
    std::ostringstream finalStream;
    finalStream << function << ":" << line << " - " << message;
    NSLog(@"%s", finalStream.str().c_str());
}

} // namespace interview_cheater::utils