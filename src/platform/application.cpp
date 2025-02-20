#include "platform/application.hpp"

#include "platform_application.hpp"

std::shared_ptr<interview_cheater::Application> interview_cheater::Application::instance_;

auto interview_cheater::Application::getInstance(signal::SignalManager& signalManager) -> std::shared_ptr<Application> {
    if (!instance_) {
#ifdef _WIN32
        instance_ = std::make_shared<PlatformApplication>(signalManager);
#elif defined(__APPLE__)
        instance_ = std::make_shared<PlatformApplication>(signalManager);
#endif
    }
    return instance_;
}