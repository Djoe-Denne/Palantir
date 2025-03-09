#include <nlohmann/json.hpp>
#include <memory>
#include <vector>
#include <algorithm>

#include "window/component/message/message_handler.hpp"
#include "window/component/message/logger_strategy.hpp"
#include "utils/logger.hpp"
#include "window/component/icontent_size_observer.hpp"

namespace palantir::window::component {

template<typename T>
class ContentManager<T>::ContentManagerImpl {
private:
    std::shared_ptr<T> view_;
    nlohmann::json content_;
    std::vector<IContentSizeObserver*> observers_;
    int currentContentWidth_ = 0;
    int currentContentHeight_ = 0;

    void updateWebView() {
        if (view_) {
            // Don't recreate the message handler since it's already initialized in the constructor
            nlohmann::json message = {
                {"type", "setContent"},
                {"content", content_}
            };
            view_->executeJavaScript("window.dispatchEvent(new MessageEvent('message', { data: " + message.dump() + " }));");
        }
    }

    void notifyObservers() {
        for (auto observer : observers_) {
            if (observer) {
                observer->onContentSizeChanged(currentContentWidth_, currentContentHeight_);
            }
        }
    }

public:
    ContentManagerImpl() : view_(new T()), content_({
              {"explanation", ""},
              {"response", ""},
              {"complexity", {
                  {"time", {{"value", ""}, {"explanation", ""}}},
                  {"space", {{"value", ""}, {"explanation", ""}}}
              }}
          })
    {
        DEBUG_LOG("ContentManagerImpl default constructor");
    }

    explicit ContentManagerImpl(std::shared_ptr<T> view) 
        : view_(view), 
          content_({
              {"explanation", ""},
              {"response", ""},
              {"complexity", {
                  {"time", {{"value", ""}, {"explanation", ""}}},
                  {"space", {{"value", ""}, {"explanation", ""}}}
              }}
          }) {
            DEBUG_LOG("ContentManagerImpl constructor with view");
          }

    ~ContentManagerImpl() = default;

    ContentManagerImpl(const ContentManagerImpl&) = delete;
    auto operator=(const ContentManagerImpl&) -> ContentManagerImpl& = delete;
    ContentManagerImpl(ContentManagerImpl&&) noexcept = delete;
    auto operator=(ContentManagerImpl&&) noexcept -> ContentManagerImpl& = delete;

    auto initialize(void* nativeWindowHandle) -> void {
        if(view_) {
            // Set up message handler to log WebView2 events
            registerMessageStrategy(std::make_unique<message::LoggerStrategy>("*"));

            // Initialize WebView2 with completion callback
            view_->initialize(nativeWindowHandle, [this]() {
                DEBUG_LOG("WebView2 initialization callback - loading URL");
                view_->loadURL("http://www.google.com");
            });
        }
    }

    auto setRootContent(const std::string& content) -> void {
        try {
            DEBUG_LOG("Setting root content: %s", content.c_str());
            content_ = nlohmann::json::parse(content);
            updateWebView();
        } catch (const nlohmann::json::exception& e) {
            // Handle JSON parsing error
            throw std::runtime_error("Invalid JSON format: " + std::string(e.what()));
        }
    }

    auto setContent(const std::string& elementId, const std::string& content) -> void {
        try {
            if (elementId == "explanation" || elementId == "response") {
                content_[elementId] = content;
            } else if (elementId.find("complexity.") == 0) {
                auto parts = split(elementId.substr(11), ".");  // Remove "complexity." prefix
                if (parts.size() == 2) {  // e.g., "time.value" or "time.explanation"
                    content_["complexity"][parts[0]][parts[1]] = content;
                }
            }
            updateWebView();
        } catch (const std::exception& e) {
            throw std::runtime_error("Failed to set content: " + std::string(e.what()));
        }
    }

    auto getContent(const std::string& elementId) -> std::string {
        try {
            if (elementId == "explanation" || elementId == "response") {
                return content_[elementId].get<std::string>();
            } else if (elementId.find("complexity.") == 0) {
                auto parts = split(elementId.substr(11), ".");
                if (parts.size() == 2) {
                    return content_["complexity"][parts[0]][parts[1]].get<std::string>();
                }
            }
            throw std::runtime_error("Invalid element ID");
        } catch (const std::exception& e) {
            throw std::runtime_error("Failed to get content: " + std::string(e.what()));
        }
    }

    auto toggleContentVisibility(const std::string& elementId) -> void {
        if (view_) {
            nlohmann::json message = {
                {"type", "toggleVisibility"},
                {"elementId", elementId}
            };
            view_->executeJavaScript("window.dispatchEvent(new MessageEvent('message', { data: " + message.dump() + " }));");
        }
    }

    auto setContentVisibility(const std::string& elementId, bool visible) -> void {
        if (view_) {
            nlohmann::json message = {
                {"type", "setVisibility"},
                {"elementId", elementId},
                {"visible", visible}
            };
            view_->executeJavaScript("window.dispatchEvent(new MessageEvent('message', { data: " + message.dump() + " }));");
        }
    }

    auto getContentVisibility(const std::string& elementId) -> bool {
        // This would require a callback from JavaScript to C++
        // For now, we'll return true as default
        return true;
    }

    auto destroy() -> void {
        if(view_) {
            view_->destroy();
        }
    }

    auto resize(int width, int height) -> void {
        if(view_) {
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
        auto it = std::find(observers_.begin(), observers_.end(), observer);
        if (it != observers_.end()) {
            observers_.erase(it);
        }
    }

    auto detectContentSizeChange() -> void {
        if (view_) {
            // Request current content size from JavaScript
            view_->executeJavaScript(R"(
                (function() {
                    const width = Math.max(
                        document.body.scrollWidth, 
                        document.documentElement.scrollWidth,
                        document.body.offsetWidth, 
                        document.documentElement.offsetWidth,
                        document.body.clientWidth, 
                        document.documentElement.clientWidth
                    );
                    
                    const height = Math.max(
                        document.body.scrollHeight, 
                        document.documentElement.scrollHeight,
                        document.body.offsetHeight, 
                        document.documentElement.offsetHeight,
                        document.body.clientHeight, 
                        document.documentElement.clientHeight
                    );
                    
                    window.chrome.webview.postMessage({
                        type: 'contentSize',
                        width: width,
                        height: height
                    });
                })();
            )");
        }
    }

    auto getContentWidth() const -> int {
        return currentContentWidth_;
    }

    auto getContentHeight() const -> int {
        return currentContentHeight_;
    }

    /**
     * @brief Sets a handler for messages received from the JavaScript context.
     *
     * @param handler A function to handle messages from JavaScript.
     */
    auto registerMessageStrategy(std::unique_ptr<message::MessageStrategy> strategy) -> void {
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
    auto split(const std::string& str, const std::string& delimiter) -> std::vector<std::string> {
        std::vector<std::string> tokens;
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

} // namespace palantir::window::component
