#pragma once

#include "window/component/icontent_manager.hpp"
#include "window/component/message/logger/logger_strategy.hpp"
#include "window/component/message/resize/resize_strategy.hpp"
#include <memory>
#include <string>
#include "core_export.hpp"

namespace palantir::window::component {
template<typename T>
class PALANTIR_CORE_API ContentManager : public IContentManager, public std::enable_shared_from_this<ContentManager<T>> {
public:
    using std::enable_shared_from_this<ContentManager<T>>::shared_from_this;
    ContentManager() : pimpl_(std::make_unique<ContentManagerImpl>()) {}  // NOLINT
    explicit ContentManager(std::unique_ptr<T> view) : pimpl_(std::make_unique<ContentManagerImpl>(view)) {}
    ~ContentManager() override = default;

    ContentManager(const ContentManager&) = delete;
    auto operator=(const ContentManager&) -> ContentManager& = delete;
    ContentManager(ContentManager&&) noexcept = delete;
    auto operator=(ContentManager&&) noexcept -> ContentManager& = delete;

    /**
     * @brief Initialize the content manager.
     * 
     * @param nativeWindowHandle The native window handle.
     */
    auto initialize(void* nativeWindowHandle) -> void override {
        registerMessageStrategy(message::makeStrategy(std::make_unique<message::logger::LoggerStrategy>("*")));
        registerMessageStrategy(message::makeStrategy(std::make_unique<message::resize::ResizeStrategy>("contentSize", std::static_pointer_cast<IContentManager>(shared_from_this()))));
        pimpl_->initialize(nativeWindowHandle);
    }

    /**
     * @brief Set the root content.
     * 
     * @param content The content to set.
     */
    auto setRootContent(const std::string& content) -> void override {
        pimpl_->setRootContent(content);
    }

    /**
     * @brief Set the content.
     * 
     * @param elementId The element id.
     * @param content The content to set.
     */
    auto setContent(const std::string& elementId, const std::string& content) -> void override {
        pimpl_->setContent(elementId, content);
    }

    /**
     * @brief Get the content.
     * 
     * @param elementId The element id.
     * @return std::string The content.
     */
    auto getContent(const std::string& elementId) -> std::string override {
        return pimpl_->getContent(elementId);
    }

    /**
     * @brief Toggle the content visibility.
     * 
     * @param elementId The element id.
     */
    auto toggleContentVisibility(const std::string& elementId) -> void override {
        pimpl_->toggleContentVisibility(elementId);
    }

    /**
     * @brief Set the content visibility.
     * 
     * @param elementId The element id.
     * @param visible The visibility to set.
     */
    auto setContentVisibility(const std::string& elementId, bool visible) -> void override {
        pimpl_->setContentVisibility(elementId, visible);
    }

    /**
     * @brief Get the content visibility.
     * 
     * @param elementId The element id.
     * @return bool The visibility.
     */
    auto getContentVisibility(const std::string& elementId) -> bool override {
        return pimpl_->getContentVisibility(elementId);
    }

    /**
     * @brief Destroy the content manager.
     */
    auto destroy() -> void override {
        pimpl_->destroy();
    }

    /**
     * @brief Resize the content manager.
     * 
     * @param width The width to resize to.
     * @param height The height to resize to.
     */
    auto resize(int width, int height) -> void override {
        pimpl_->resize(width, height);
    }

    /**
     * @brief Add an observer to be notified of content size changes.
     * 
     * @param observer The observer to add.
     */
    auto addContentSizeObserver(IContentSizeObserver* observer) -> void override {
        pimpl_->addContentSizeObserver(observer);
    }

    /**
     * @brief Remove an observer from the notification list.
     * 
     * @param observer The observer to remove.
     */
    auto removeContentSizeObserver(IContentSizeObserver* observer) -> void override {
        pimpl_->removeContentSizeObserver(observer);
    }

    /**
     * @brief Detect and notify observers of content size changes.
     * 
     * This method should be called periodically to check content size and
     * notify observers if the size has changed.
     */
    auto detectContentSizeChange() -> void override {
        pimpl_->detectContentSizeChange();
    }

    /**
     * @brief Get the current content width.
     * 
     * @return int The current content width.
     */
    auto getContentWidth() const -> int override {
        return pimpl_->getContentWidth();
    }

    /**
     * @brief Get the current content height.
     * 
     * @return int The current content height.
     */
    auto getContentHeight() const -> int override {
        return pimpl_->getContentHeight();
    }

    /**
     * @brief Register a message strategy.
     * 
     * @param strategy The strategy to register.
     */
    auto registerMessageStrategy(std::unique_ptr<message::MessageStrategyBase> strategy) -> void override {
        pimpl_->registerMessageStrategy(std::move(strategy));
    }

    /**
     * @brief Handle a message.
     * 
     * @param message The message to handle.
     */ 
    auto handleMessage(const std::string& message) -> void override {
        pimpl_->handleMessage(message);
    }
private:
    class ContentManagerImpl;
    #pragma warning(push)
    #pragma warning(disable: 4251)
    std::unique_ptr<ContentManagerImpl> pimpl_;
    #pragma warning(pop)
};

} // namespace palantir::window::component
