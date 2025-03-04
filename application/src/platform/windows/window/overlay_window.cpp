#include "window/overlay_window.hpp"

#include "window/overlay_window_impl.hpp"

namespace palantir::window {

OverlayWindow::OverlayWindow() : pImpl_(std::make_unique<Impl>()) {}  // NOLINT
OverlayWindow::~OverlayWindow() = default;

auto OverlayWindow::create() -> void { pImpl_->create(); }
auto OverlayWindow::show() -> void { pImpl_->show(); }
auto OverlayWindow::update() -> void { pImpl_->update(); }
auto OverlayWindow::close() -> void { pImpl_->close(); }
auto OverlayWindow::setTransparency(int transparency) -> void { pImpl_->setTransparency(transparency); }
auto OverlayWindow::toggleWindowAnonymity() -> void { pImpl_->toggleWindowAnonymity(); }
auto OverlayWindow::getNativeHandle() const -> void* { return pImpl_->getNativeHandle(); }
auto OverlayWindow::isRunning() const -> bool { return pImpl_->isRunning(); }
auto OverlayWindow::setRunning(bool runningState) -> void { pImpl_->setRunning(runningState); }
auto OverlayWindow::getContentManager() const -> std::shared_ptr<component::IContentManager> {
    return pImpl_->getContentManager();
}
}  // namespace palantir::window