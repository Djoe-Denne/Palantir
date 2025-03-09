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

    // Callback registration methods
    void setInitCallback(std::function<void()> callback);

    // Event handler getters
    Microsoft::WRL::ComPtr<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler> 
    getEnvironmentCompletedHandler(class WebView* webview);

    Microsoft::WRL::ComPtr<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>
    getControllerCompletedHandler(class WebView* webview);

    Microsoft::WRL::ComPtr<ICoreWebView2WebMessageReceivedEventHandler>
    getWebMessageReceivedHandler(class WebView* webview);

    Microsoft::WRL::ComPtr<ICoreWebView2NavigationCompletedEventHandler>
    getNavigationCompletedHandler(class WebView* webview);

    Microsoft::WRL::ComPtr<ICoreWebView2SourceChangedEventHandler>
    getSourceChangedHandler();

    Microsoft::WRL::ComPtr<ICoreWebView2ExecuteScriptCompletedHandler>
    getExecuteScriptCompletedHandler();

private:
    std::function<void()> initCallback_;
    
};

} // namespace palantir::window::component::webview 