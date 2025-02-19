#ifndef OVERLAY_WINDOW_HPP
#define OVERLAY_WINDOW_HPP

#include "window/iwindow.hpp"

#include <memory>

namespace interview_cheater::window
{

class OverlayWindow : public IWindow
{
public:
    OverlayWindow();
    ~OverlayWindow() override;

    void  create() override;
    void  show() override;
    void  update() override;
    void  close() override;
    void* getNativeHandle() const override;

    bool isRunning() const override;
    void setRunning(bool runningState) override;

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
    bool                  running;
};

}  // namespace interview_cheater::window

#endif  // OVERLAY_WINDOW_HPP