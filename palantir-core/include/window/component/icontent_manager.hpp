#pragma once

#include <memory>
#include <string>
#include <vector>
#include "core_export.hpp"
#include "window/component/icontent_size_observer.hpp"
#include "window/component/message/message_strategy_concept.hpp"

namespace palantir::window::component {
class PALANTIR_CORE_API IContentManager {
public:
    virtual ~IContentManager() = default;

    IContentManager(const IContentManager&) = delete;
    auto operator=(const IContentManager&) -> IContentManager& = delete;
    IContentManager(IContentManager&&) noexcept = delete;
    auto operator=(IContentManager&&) noexcept -> IContentManager& = delete;

    /**
     * @brief Initialize the content manager.
     * 
     * @param nativeWindowHandle The native window handle.
     */
    virtual auto initialize(void* nativeWindowHandle) -> void = 0;

    /**
     * @brief Set the root content.
     * 
     * @param content The content to set.
     */
    virtual auto setRootContent(const std::string& content) -> void = 0;

    /**
     * @brief Set the content.
     * 
     * @param elementId The element id.
     * @param content The content to set.
     */
    virtual auto setContent(const std::string& elementId, const std::string& content) -> void = 0;

    /**
     * @brief Get the content.
     * 
     * @param elementId The element id.
     * @return std::string The content.
     */
    virtual auto getContent(const std::string& elementId) -> std::string = 0;

    /**
     * @brief Toggle the content visibility.
     * 
     * @param elementId The element id.
     */
    virtual auto toggleContentVisibility(const std::string& elementId) -> void = 0;

    /**
     * @brief Set the content visibility.
     * 
     * @param elementId The element id.
     * @param visible The visibility to set.
     */
    virtual auto setContentVisibility(const std::string& elementId, bool visible) -> void = 0;

    /**
     * @brief Get the content visibility.
     * 
     * @param elementId The element id.
     * @return bool The visibility.
     */
    virtual auto getContentVisibility(const std::string& elementId) -> bool = 0;

    /**
     * @brief Destroy the content manager.
     */
    virtual auto destroy() -> void = 0;

    /**
     * @brief Resize the content manager.
     * 
     * @param width The width to resize to.
     * @param height The height to resize to.
     */
    virtual auto resize(int width, int height) -> void = 0;

    /**
     * @brief Add an observer to be notified of content size changes.
     * 
     * @param observer The observer to add.
     */
    virtual auto addContentSizeObserver(IContentSizeObserver* observer) -> void = 0;

    /**
     * @brief Remove an observer from the notification list.
     * 
     * @param observer The observer to remove.
     */
    virtual auto removeContentSizeObserver(IContentSizeObserver* observer) -> void = 0;

    /**
     * @brief Detect and notify observers of content size changes.
     * 
     * This method should be called periodically to check content size and
     * notify observers if the size has changed.
     */
    virtual auto detectContentSizeChange() -> void = 0;

    /**
     * @brief Get the current content width.
     * 
     * @return int The current content width.
     */
    [[nodiscard]] virtual auto getContentWidth() const -> int = 0;

    /**
     * @brief Get the current content height.
     * 
     * @return int The current content height.
     */
    [[nodiscard]] virtual auto getContentHeight() const -> int = 0;

    /**
     * @brief Register a message strategy.
     * 
     * @param strategy The strategy to register.
     */
    virtual auto registerMessageStrategy(std::unique_ptr<message::MessageStrategyBase> strategy) -> void = 0;

    /**
     * @brief Handle a message.
     * 
     * @param message The message to handle.
     */
    virtual auto handleMessage(const std::string& message) -> void = 0;
protected:
    IContentManager() = default;
};

} // namespace palantir::window::component
