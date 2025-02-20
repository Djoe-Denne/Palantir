#include "platform/application.hpp"

#include "platform_application.hpp"

using namespace interview_cheater;
std::shared_ptr<Application> Application::instance;

std::shared_ptr<Application> Application::getInstance(signal::SignalManager& signalManager) {
    if (!instance) {
        instance = std::make_shared<PlatformApplication>(signalManager);
    }
    return instance;
}