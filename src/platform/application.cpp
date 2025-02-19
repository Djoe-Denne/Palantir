#include "platform/application.hpp"
#include "platform_application.hpp"

namespace interview_cheater {
std::shared_ptr<Application> Application::instance;

auto Application::getInstance(signal::SignalManager &signalManager)
    -> std::shared_ptr<Application> {
  if (!instance) {
    instance = std::make_shared<PlatformApplication>(signalManager);
  }
  return instance;
}
} // namespace interview_cheater