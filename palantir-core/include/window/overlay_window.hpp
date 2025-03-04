#ifndef OVERLAY_WINDOW_HPP
#define OVERLAY_WINDOW_HPP

#include <memory>
#include "window/iwindow.hpp"
#include "window/component/content_manager.hpp"
#include "window/component/content_manager_impl.hpp"
#include "core_export.hpp"

namespace palantir::window {

class PALANTIR_CORE_API OverlayWindow : public IWindow {
public:
    OverlayWindow();
    ~OverlayWindow() override;

    // Delete copy operations
    OverlayWindow(const OverlayWindow&) = delete;
    auto operator=(const OverlayWindow&) -> OverlayWindow& = delete;

    // Define move operations
    OverlayWindow(OverlayWindow&&) noexcept = delete;
    auto operator=(OverlayWindow&&) noexcept -> OverlayWindow& = delete;

    auto create() -> void override;
    auto show() -> void override;
    auto update() -> void override;
    auto close() -> void override;
    auto setTransparency(int transparency) -> void override;
    auto toggleWindowAnonymity() -> void override;
    [[nodiscard]] auto getNativeHandle() const -> void* override;

    [[nodiscard]] auto isRunning() const -> bool override;
    auto setRunning(bool runningState) -> void override;

    [[nodiscard]] auto getContentManager() const -> std::shared_ptr<component::IContentManager> override;

private:
    class Impl;
    #pragma warning(push)
    #pragma warning(disable: 4251)
    std::unique_ptr<Impl> pImpl_;
    #pragma warning(pop)
    bool running_ = false;
};

}  // namespace palantir::window

#endif  // OVERLAY_WINDOW_HPP