#ifndef IWINDOW_HPP
#define IWINDOW_HPP

#include <memory>

namespace interview_cheater::window {

class IWindow {
public:
    virtual ~IWindow() = default;

    // Platform-agnostic window lifecycle methods
    virtual void create() = 0;
    virtual void show() = 0;
    virtual void update() = 0;
    virtual void close() = 0;

    // Window state methods
    virtual bool isRunning() const = 0;
    virtual void setRunning(bool runningState) = 0;

    // Native handle accessor - returns void* to avoid platform-specific types
    // The actual implementation will cast this to the appropriate type
    virtual void* getNativeHandle() const = 0;
};

}  // namespace interview_cheater::window

#endif  // IWINDOW_HPP