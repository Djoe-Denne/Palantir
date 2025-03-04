#include "utils/logger.hpp"
#import <Foundation/Foundation.h>
#include <os/log.h>
#include <sstream>
#include <string_view>

namespace palantir::utils {

auto PlatformLog(std::string_view function, int line, const std::string& message) -> void {
    std::ostringstream finalStream;
    finalStream << function << ":" << line << " - " << message;
    NSString* logMessage = [NSString stringWithUTF8String:finalStream.str().c_str()];
    NSLog(@"%@", logMessage);
}

}  // namespace palantir::utils