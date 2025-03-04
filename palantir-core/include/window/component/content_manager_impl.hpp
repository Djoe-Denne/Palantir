#include <nlohmann/json.hpp>
#include <memory>

//#include "window/component/webview/webview.hpp"
#include "utils/logger.hpp"

namespace palantir::window::component {

template<typename T>
class ContentManager<T>::ContentManagerImpl {
private:
    std::shared_ptr<T> view_;
    nlohmann::json content_;

    void updateWebView() {
        if (view_) {
            nlohmann::json message = {
                {"type", "setContent"},
                {"content", content_}
            };
            view_->executeJavaScript("window.dispatchEvent(new MessageEvent('message', { data: " + message.dump() + " }));");
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
          }) {
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
            view_->setMessageHandler([](const std::string& message) {
                DEBUG_LOG("WebView2 message received: %s", message.c_str());
            });

            // Initialize WebView2 with completion callback
            view_->initialize(nativeWindowHandle, [this]() {
                DEBUG_LOG("WebView2 initialization callback - loading URL");
                view_->loadURL("http://localhost:5173");
            });
        }
    }

    auto setRootContent(const std::string& content) -> void {
        try {
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
            view_->resize(width, height);
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
