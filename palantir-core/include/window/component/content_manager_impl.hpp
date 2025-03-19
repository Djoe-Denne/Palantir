#include <algorithm>
#include <memory>
#include <nlohmann/json.hpp>
#include <vector>

#include "exception/exceptions.hpp"
#include "utils/logger.hpp"
#include "window/component/content_manager.hpp"
#include "window/component/icontent_size_observer.hpp"
#include "window/component/message/logger/logger_strategy.hpp"
#include "window/component/message/message_handler.hpp"

namespace palantir::window::component {

template <typename T>
class ContentManager<T>::ContentManagerImpl {
private:
    std::shared_ptr<T> view_{std::make_shared<T>()};
    nlohmann::json content_{
        {{"explanation", ""},
         {"response", ""},
         {"complexity",
          {{"time", {{"value", ""}, {"explanation", ""}}}, {"space", {{"value", ""}, {"explanation", ""}}}}}}};
    std::vector<IContentSizeObserver*> observers_;
    int currentContentWidth_ = 0;
    int currentContentHeight_ = 0;

    void updateWebView() {
        if (view_) {
            // Don't recreate the message handler since it's already initialized in the constructor
            nlohmann::json message = {{"type", "setContent"}, {"content", content_}};
            view_->executeJavaScript("window.dispatchEvent(new MessageEvent('message', { data: " + message.dump() +
                                     " }));");
        }
    }

    void notifyObservers() {
        for (auto observer : observers_) {
            if (observer) {
                DebugLog("Notifying observer of content size change: ", currentContentWidth_, "x",
                         currentContentHeight_);
                observer->onContentSizeChanged(currentContentWidth_, currentContentHeight_);
            }
        }
    }

public:
    ContentManagerImpl() = default;

    explicit ContentManagerImpl(std::shared_ptr<T> view) : view_(view) {}

    ~ContentManagerImpl() = default;

    ContentManagerImpl(const ContentManagerImpl&) = delete;
    auto operator=(const ContentManagerImpl&) -> ContentManagerImpl& = delete;
    ContentManagerImpl(ContentManagerImpl&&) noexcept = delete;
    auto operator=(ContentManagerImpl&&) noexcept -> ContentManagerImpl& = delete;

    auto initialize(uintptr_t nativeWindowHandle) -> void {
        if (view_) {
            // Initialize WebView2 with completion callback
            view_->initialize(nativeWindowHandle, [this]() {
                DebugLog("WebView2 initialization callback - loading URL");
                view_->loadURL("http://www.google.com");
            });
        }
    }

    auto setRootContent(const std::string& content) -> void {
        try {
            DebugLog("Setting root content: %s", content.c_str());
            content_ = nlohmann::json::parse(content);
            updateWebView();
        } catch (const nlohmann::json::exception& e) {
            // Handle JSON parsing error
            throw palantir::exception::TraceableContentManagerException("Invalid JSON format: " +
                                                                        std::string(e.what()));
        }
    }

    auto setContent(const std::string& elementId, const std::string& content) -> void {
        try {
            if (elementId == "explanation" || elementId == "response") {
                content_[elementId] = content;
            } else if (elementId.find("complexity.") == 0) {
                auto parts = split(elementId.substr(11), ".");  // Remove "complexity." prefix
                if (parts.size() == 2) {                        // e.g., "time.value" or "time.explanation"
                    content_["complexity"][parts[0]][parts[1]] = content;
                }
            }
            updateWebView();
        } catch (const std::exception& e) {
            throw palantir::exception::TraceableContentManagerException("Failed to set content: " +
                                                                        std::string(e.what()));
        }
    }

    [[nodiscard]] auto getContent(const std::string& elementId) const -> std::string {
        try {
            if (elementId == "explanation" || elementId == "response") {
                return content_[elementId].get<std::string>();
            } else if (elementId.find("complexity.") == 0) {
                auto parts = split(elementId.substr(11), ".");
                if (parts.size() == 2) {
                    return content_["complexity"][parts[0]][parts[1]].get<std::string>();
                }
            }
            throw palantir::exception::TraceableContentManagerException("Invalid element ID");
        } catch (const std::exception& e) {
            throw palantir::exception::TraceableContentManagerException("Failed to get content: " +
                                                                        std::string(e.what()));
        }
    }

    auto toggleContentVisibility(const std::string& elementId) -> void {
        if (view_) {
            nlohmann::json message = {{"type", "toggleVisibility"}, {"elementId", elementId}};
            view_->executeJavaScript("window.dispatchEvent(new MessageEvent('message', { data: " + message.dump() +
                                     " }));");
        }
    }

    auto setContentVisibility(const std::string& elementId, bool visible) -> void {
        if (view_) {
            nlohmann::json message = {{"type", "setVisibility"}, {"elementId", elementId}, {"visible", visible}};
            view_->executeJavaScript("window.dispatchEvent(new MessageEvent('message', { data: " + message.dump() +
                                     " }));");
        }
    }

    [[nodiscard]] auto getContentVisibility([[maybe_unused]] const std::string_view& elementId) const -> bool {
        // This would require a callback from JavaScript to C++
        // For now, we'll return true as default
        return true;
    }

    auto destroy() -> void {
        if (view_) {
            view_->destroy();
        }
    }

    auto resize(int width, int height) -> void {
        if (view_) {
            currentContentHeight_ = height;
            currentContentWidth_ = width;
            view_->resize(width, height);
            notifyObservers();
        }
    }

    auto addContentSizeObserver(IContentSizeObserver* observer) -> void {
        if (observer && std::find(observers_.begin(), observers_.end(), observer) == observers_.end()) {
            observers_.push_back(observer);

            // Notify new observer of current size immediately if we have a size
            if (currentContentWidth_ > 0 && currentContentHeight_ > 0) {
                observer->onContentSizeChanged(currentContentWidth_, currentContentHeight_);
            }
        }
    }

    auto removeContentSizeObserver(IContentSizeObserver* observer) -> void {
        auto iterator = std::find(observers_.begin(), observers_.end(), observer);
        if (iterator != observers_.end()) {
            observers_.erase(iterator);
        }
    }

    [[nodiscard]] auto getContentWidth() const -> int { return currentContentWidth_; }

    [[nodiscard]] auto getContentHeight() const -> int { return currentContentHeight_; }

    /**
     * @brief Sets a handler for messages received from the JavaScript context.
     *
     * @param handler A function to handle messages from JavaScript.
     */
    auto registerMessageStrategy(std::unique_ptr<message::MessageStrategyBase> strategy) -> void {
        if (view_) {
            view_->registerMessageStrategy(std::move(strategy));
        }
    }

    auto handleMessage(const std::string& message) -> void {
        if (view_) {
            view_->handleMessage(message);
        }
    }

private:
    [[nodiscard]] auto split(const std::string_view& str, const std::string_view& delimiter) const
        -> std::vector<std::string_view> {
        std::vector<std::string_view> tokens;
        size_t start = 0;
        size_t end = str.find(delimiter);
        while (end != std::string::npos) {
            tokens.push_back(str.substr(start, end - start));
            start = end + delimiter.length();
            end = str.find(delimiter, start);
        }
        tokens.push_back(str.substr(start));
        return tokens;
    }
};

}  // namespace palantir::window::component
