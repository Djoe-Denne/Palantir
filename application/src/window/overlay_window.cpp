#include "window/overlay_window.hpp"

#include "window/overlay_window_impl.hpp"

namespace palantir::window {

OverlayWindow::OverlayWindow() : pImpl_(std::make_unique<Impl>()), type_(WindowType::MAIN) {}  // NOLINT
OverlayWindow::OverlayWindow(const WindowType& type) : pImpl_(std::make_unique<Impl>()), type_(type) {}  // NOLINT
OverlayWindow::~OverlayWindow() {
    getContentManager()->removeContentSizeObserver(this);
}

auto OverlayWindow::create() -> void { 
    pImpl_->create(); 
    // Register as observer for content size changes
    getContentManager()->addContentSizeObserver(this);
}
auto OverlayWindow::show() -> void { pImpl_->show(); }
auto OverlayWindow::update() -> void { pImpl_->update(); }
auto OverlayWindow::close() -> void { 
    getContentManager()->removeContentSizeObserver(this);
    pImpl_->close(); 
}
auto OverlayWindow::setTransparency(int transparency) -> void { pImpl_->setTransparency(transparency); }
auto OverlayWindow::toggleWindowAnonymity() -> void { pImpl_->toggleWindowAnonymity(); }
auto OverlayWindow::getNativeHandle() const -> void* { return pImpl_->getNativeHandle(); }
auto OverlayWindow::isRunning() const -> bool { return pImpl_->isRunning(); }
auto OverlayWindow::setRunning(bool runningState) -> void { pImpl_->setRunning(runningState); }
auto OverlayWindow::getContentManager() const -> std::shared_ptr<component::IContentManager> {
    return pImpl_->getContentManager();
}
auto OverlayWindow::getWindowType() const -> const WindowType& { return type_; }

auto OverlayWindow::onContentSizeChanged(int width, int height) -> void {
    auto [screenWidth, screenHeight] = getCurrentScreenResolution();
    width = std::min(width, screenWidth);
    height = std::min(height, screenHeight);
    
    DEBUG_LOG("Content size changed notification received: %dx%d", width, height);
    if (width > 0 && height > 0) {
        updateWindowSize(width, height);
    }
}

auto OverlayWindow::toggleWindowTool(bool isToolWindow) -> void {
    pImpl_->toggleWindowTool(isToolWindow);
}

auto OverlayWindow::updateWindowSize(int contentWidth, int contentHeight) -> void {
    pImpl_->updateWindowSize(contentWidth, contentHeight);
}

auto OverlayWindow::getCurrentScreenResolution() -> std::pair<int, int> {
    return pImpl_->getCurrentScreenResolution();
}

auto OverlayWindow::makeWindowFrameless() -> void {
    pImpl_->makeWindowFrameless();
}
}  // namespace palantir::window