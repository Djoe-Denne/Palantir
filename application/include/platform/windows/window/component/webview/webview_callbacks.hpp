#pragma once

// clang-format off
// order of includes is important
#include <objbase.h>
#include <Windows.h>
#include <WebView2.h>
#include <WebView2EnvironmentOptions.h>
#include <wrl.h>
// clang-format on
#include <functional>
#include <memory>
#include <string>

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
    auto getEnvironmentCompletedHandler(class WebView* webview)
        -> Microsoft::WRL::ComPtr<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>;

    auto getControllerCompletedHandler(class WebView* webview)
        -> Microsoft::WRL::ComPtr<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>;

    auto getWebMessageReceivedHandler(class WebView* webview)
        -> Microsoft::WRL::ComPtr<ICoreWebView2WebMessageReceivedEventHandler>;

    auto getNavigationCompletedHandler(class WebView* webview)
        -> Microsoft::WRL::ComPtr<ICoreWebView2NavigationCompletedEventHandler>;

    auto getSourceChangedHandler() -> Microsoft::WRL::ComPtr<ICoreWebView2SourceChangedEventHandler>;

    auto getExecuteScriptCompletedHandler() -> Microsoft::WRL::ComPtr<ICoreWebView2ExecuteScriptCompletedHandler>;

private:
    // Handler implementations
    auto handleEnvironmentCompleted(HRESULT result, ICoreWebView2Environment* env, class WebView* webview) -> HRESULT;
    auto handleControllerCompleted(HRESULT result, ICoreWebView2Controller* controller, class WebView* webview) -> HRESULT;
    auto handleWebMessageReceived(const ICoreWebView2* sender, ICoreWebView2WebMessageReceivedEventArgs* args, class WebView* webview) -> HRESULT;
    auto handleNavigationCompleted(ICoreWebView2* sender, ICoreWebView2NavigationCompletedEventArgs* args) -> HRESULT;
    auto handleSourceChanged(ICoreWebView2* sender, const ICoreWebView2SourceChangedEventArgs* args) -> HRESULT;
    auto handleExecuteScriptCompleted(HRESULT error, LPCWSTR result) -> HRESULT;

    std::function<void()> initCallback_;
};

}  // namespace palantir::window::component::webview