#include "platform/application.hpp"

#ifdef _WIN32
#include "platform/windows/windows_application.hpp"
#elif defined(__APPLE__)
#include "platform/macos/macos_application.hpp"
#endif

std::shared_ptr<Application> Application::instance;

std::shared_ptr<Application> Application::getInstance(SignalManager& signalManager) {
    if (!instance) {
#ifdef _WIN32
        instance = std::make_shared<WindowsApplication>(signalManager);
#elif defined(__APPLE__)
        instance = std::make_shared<MacOSApplication>(signalManager);
#else
        #error "Unsupported platform"
#endif
    }
    return instance;
} 