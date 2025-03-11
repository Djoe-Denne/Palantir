#pragma once

// clang-format off
// order of includes is important
#include <objbase.h>
#include <windows.h>
#include <WebView2.h>
#include <WebView2EnvironmentOptions.h>
#include <wrl.h>
// clang-format on
#include <functional>
#include <string>
#include <memory>

#include "utils/logger.hpp"

namespace palantir::window::component::webview {

class WebViewCallbacks {
public:
    WebViewCallbacks();
    ~WebViewCallbacks() = default;

    WebViewCallbacks(const WebViewCallbacks& other) = delete;
    auto operator=(const WebViewCallbacks& other) -> WebViewCallbacks& = delete;
    WebViewCallbacks(WebViewCallbacks&& other) = delete;
    auto operator=(WebViewCallbacks&& other) -> WebViewCallbacks& = delete;

    // Callback registration methods
    void setInitCallback(std::function<void()> callback);

    // Event handler getters
    auto getEnvironmentCompletedHandler(class WebView* webview) -> Microsoft::WRL::ComPtr<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>;

    auto getControllerCompletedHandler(class WebView* webview) -> Microsoft::WRL::ComPtr<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>;

    auto getWebMessageReceivedHandler(class WebView* webview) -> Microsoft::WRL::ComPtr<ICoreWebView2WebMessageReceivedEventHandler>;

    auto getNavigationCompletedHandler(class WebView* webview) -> Microsoft::WRL::ComPtr<ICoreWebView2NavigationCompletedEventHandler>;

    auto getSourceChangedHandler() -> Microsoft::WRL::ComPtr<ICoreWebView2SourceChangedEventHandler>;

    auto getExecuteScriptCompletedHandler() -> Microsoft::WRL::ComPtr<ICoreWebView2ExecuteScriptCompletedHandler>;

private:
    std::function<void()> initCallback_;
    
};

} // namespace palantir::window::component::webview 