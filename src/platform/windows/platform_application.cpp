#include "platform_application.hpp"
#include "signal/signal_manager.hpp"
#include <windows.h>
#import "utils/logger.hpp"


namespace interview_cheater
{

class PlatformApplication::Impl
{
public:
    explicit Impl(signal::SignalManager& signalManager) : signalManager_(signalManager) {}

    auto run() -> int
    {
        while (running_)
        {
            MSG msg;
            while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
            {
                if (msg.message == WM_QUIT)
                {
                    running_ = false;
                    break;
                }
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }

            // Check signals every frame
            signalManager_.checkSignals();

            // Small sleep to prevent high CPU usage
            Sleep(kSleepDurationMs);
        }
        return 0;
    }

    void quit()
    {
        running_ = false;
        PostQuitMessage(0);
    }

private:
    signal::SignalManager& signalManager_;
    bool                   running_         = true;
    static constexpr int   kSleepDurationMs = 10;
};

PlatformApplication::PlatformApplication(signal::SignalManager& signalManager)
    : pImpl(std::make_unique<Impl>(signalManager))
{
}

PlatformApplication::~PlatformApplication() = default;

auto PlatformApplication::run() -> int
{
    return pImpl->run();
}

void PlatformApplication::quit()
{
    pImpl->quit();
}

}  // namespace interview_cheater