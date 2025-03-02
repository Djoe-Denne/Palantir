#pragma once

#include <memory>
#include <string>
#include "core_export.hpp"

namespace interview_cheater::window::component {
template<typename T>
class PALANTIR_CORE_API ContentManager {
public:
    ContentManager() : pimpl_(std::make_unique<ContentManagerImpl>()) {}
    explicit ContentManager(std::unique_ptr<T> view) : pimpl_(std::make_unique<ContentManagerImpl>(view)) {}
    virtual ~ContentManager() = default;

    ContentManager(const ContentManager&) = delete;
    auto operator=(const ContentManager&) -> ContentManager& = delete;
    ContentManager(ContentManager&&) noexcept = delete;
    auto operator=(ContentManager&&) noexcept -> ContentManager& = delete;

    /**
     * @brief Initialize the content manager.
     * 
     * @param nativeWindowHandle The native window handle.
     */
    virtual auto initialize(void* nativeWindowHandle) -> void {
        pimpl_->initialize(nativeWindowHandle);
    }

    /**
     * @brief Set the root content.
     * 
     * @param content The content to set.
     */
    virtual auto setRootContent(const std::string& content) -> void {
        pimpl_->setRootContent(content);
    }

    /**
     * @brief Set the content.
     * 
     * @param elementId The element id.
     * @param content The content to set.
     */
    virtual auto setContent(const std::string& elementId, const std::string& content) -> void {
        pimpl_->setContent(elementId, content);
    }

    /**
     * @brief Get the content.
     * 
     * @param elementId The element id.
     * @return std::string The content.
     */
    virtual auto getContent(const std::string& elementId) -> std::string {
        return pimpl_->getContent(elementId);
    }

    /**
     * @brief Toggle the content visibility.
     * 
     * @param elementId The element id.
     */
    virtual auto toggleContentVisibility(const std::string& elementId) -> void {
        pimpl_->toggleContentVisibility(elementId);
    }

    /**
     * @brief Set the content visibility.
     * 
     * @param elementId The element id.
     * @param visible The visibility to set.
     */
    virtual auto setContentVisibility(const std::string& elementId, bool visible) -> void {
        pimpl_->setContentVisibility(elementId, visible);
    }

    /**
     * @brief Get the content visibility.
     * 
     * @param elementId The element id.
     * @return bool The visibility.
     */
    virtual auto getContentVisibility(const std::string& elementId) -> bool {
        return pimpl_->getContentVisibility(elementId);
    }

    /**
     * @brief Destroy the content manager.
     */
    virtual auto destroy() -> void {
        pimpl_->destroy();
    }

    /**
     * @brief Resize the content manager.
     * 
     * @param width The width to resize to.
     * @param height The height to resize to.
     */
    virtual auto resize(int width, int height) -> void {
        pimpl_->resize(width, height);
    }

private:
    class ContentManagerImpl;
    #pragma warning(push)
    #pragma warning(disable: 4251)
    std::unique_ptr<ContentManagerImpl> pimpl_;
    #pragma warning(pop)
};

} // namespace interview_cheater::window::component
