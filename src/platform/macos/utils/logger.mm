#include "utils/logger.hpp"
#import <Foundation/Foundation.h>
#include <sstream>

namespace interview_cheater::utils {

[[nodiscard]] auto PlatformLog(std::string_view function, int line, const std::string& message) -> void {
    std::ostringstream finalStream;
    finalStream << function << ":" << line << " - " << message;
    NSString* logMessage = [NSString stringWithUTF8String:finalStream.str().c_str()];
    NSLog(@"%@", logMessage);
}

}  // namespace interview_cheater::utils