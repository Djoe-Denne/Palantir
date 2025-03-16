#include "window/component/webview/webview.hpp"

#include "window/component/message/logger/logger_strategy.hpp"
#include "window/component/message/message_handler.hpp"
#include "window/component/webview/webview_callbacks.hpp"

// clang-format off
// order of includes is important
#include <objbase.h>
#include <Windows.h>
#include <WebView2.h>
#include <WebView2EnvironmentOptions.h>
#include <wrl.h>
// clang-format on

#include <stdexcept>
#include <string>

#include "utils/logger.hpp"

using Microsoft::WRL::ComPtr;

namespace palantir::window::component::webview {

class WebView::WebViewImpl {
public:
    WebViewImpl() = default;
    ~WebViewImpl() { destroy(); }

    WebViewImpl(const WebViewImpl&) = delete;
    auto operator=(const WebViewImpl&) -> WebViewImpl& = delete;
    WebViewImpl(WebViewImpl&&) = delete;
    auto operator=(WebViewImpl&&) -> WebViewImpl& = delete;

    HWND hwnd_{nullptr};
    ComPtr<ICoreWebView2> webView_;
    ComPtr<ICoreWebView2Controller> controller_;
    std::unique_ptr<WebViewCallbacks> callbacks_{std::make_unique<WebViewCallbacks>()};
    EventRegistrationToken messageToken_;
    EventRegistrationToken navigationToken_;
    EventRegistrationToken sourceToken_;
    std::unique_ptr<message::MessageHandler> messageHandler_{std::make_unique<message::MessageHandler>()};

    auto destroy() -> void {
        if (webView_) {
            webView_->remove_WebMessageReceived(messageToken_);
            webView_->remove_NavigationCompleted(navigationToken_);
            webView_->remove_SourceChanged(sourceToken_);
            webView_.Reset();
        }
        if (controller_) {
            controller_->Close();
            controller_.Reset();
        }
        hwnd_ = nullptr;
    }
};

WebView::WebView() : pimpl_(std::make_unique<WebViewImpl>()) {}  // NOLINT
WebView::~WebView() = default;

auto WebView::initialize(uintptr_t nativeWindowHandle, std::function<void()> initCallback) -> void {
    pimpl_->hwnd_ = reinterpret_cast<HWND>(nativeWindowHandle);
    if (!pimpl_->hwnd_) {
        throw std::runtime_error("Invalid window handle");
    }

    pimpl_->callbacks_->setInitCallback(std::move(initCallback));

    DEBUG_LOG("Starting WebView2 initialization");

    auto options = Microsoft::WRL::Make<CoreWebView2EnvironmentOptions>();
    if (!options) {
        throw std::runtime_error("Failed to create WebView2 environment options");
    }

    options->put_AdditionalBrowserArguments(L"--disable-background-timer-throttling");

    HRESULT hResult = CreateCoreWebView2EnvironmentWithOptions(
        nullptr,  // Use default browser installation
        nullptr,  // Use default user data folder
        options.Get(), pimpl_->callbacks_->getEnvironmentCompletedHandler(this).Get());

    if (FAILED(hResult)) {
        throw std::runtime_error("Failed to create WebView2 environment");
    }
}

auto WebView::initializeController(uintptr_t controller) -> intptr_t {
    if (controller == 0) {
        throw std::runtime_error("Invalid controller pointer");
    }

    pimpl_->controller_ = reinterpret_cast<ICoreWebView2Controller*>(controller);

    HRESULT hResult = pimpl_->controller_->get_CoreWebView2(&pimpl_->webView_);
    if (FAILED(hResult) || !pimpl_->webView_) {
        DEBUG_LOG("Failed to get CoreWebView2 from controller: 0x%08x", hResult);
        return hResult;
    }

    DEBUG_LOG("CoreWebView2 obtained successfully");

    // Configure WebView2 settings
    ComPtr<ICoreWebView2Settings> settings;
    hResult = pimpl_->webView_->get_Settings(&settings);
    if (SUCCEEDED(hResult)) {
        settings->put_IsScriptEnabled(TRUE);
        settings->put_AreDefaultScriptDialogsEnabled(TRUE);
        settings->put_IsWebMessageEnabled(TRUE);
        settings->put_IsStatusBarEnabled(FALSE);
        settings->put_AreDevToolsEnabled(TRUE);
        DEBUG_LOG("WebView2 settings configured successfully");
    }

    // Add event handlers
    hResult =
        pimpl_->webView_->add_SourceChanged(pimpl_->callbacks_->getSourceChangedHandler().Get(), &pimpl_->sourceToken_);

    if (FAILED(hResult)) {
        DEBUG_LOG("Failed to add source changed handler: 0x%08x", hResult);
        return hResult;
    }

    hResult = pimpl_->webView_->add_NavigationCompleted(pimpl_->callbacks_->getNavigationCompletedHandler(this).Get(),
                                                        &pimpl_->navigationToken_);

    if (FAILED(hResult)) {
        DEBUG_LOG("Failed to add navigation completed handler: 0x%08x", hResult);
        return hResult;
    }

    hResult = pimpl_->webView_->add_WebMessageReceived(pimpl_->callbacks_->getWebMessageReceivedHandler(this).Get(),
                                                       &pimpl_->messageToken_);

    if (FAILED(hResult)) {
        DEBUG_LOG("Failed to add message received handler: 0x%08x", hResult);
        return hResult;
    }

    RECT bounds;
    GetClientRect(pimpl_->hwnd_, &bounds);
    DEBUG_LOG("Setting WebView2 bounds to: left=%ld, top=%ld, right=%ld, bottom=%ld", bounds.left, bounds.top,
              bounds.right, bounds.bottom);
    pimpl_->controller_->put_Bounds(bounds);
    pimpl_->controller_->put_IsVisible(TRUE);

    return hResult;
}

auto WebView::getNativeHandle() -> uintptr_t { return reinterpret_cast<uintptr_t>(pimpl_->hwnd_); }

auto WebView::loadURL(const std::string& url) -> void {
    if (!pimpl_->webView_) {
        return;
    }

    std::wstring wurl(url.begin(), url.end());
    HRESULT hResult = pimpl_->webView_->Navigate(wurl.c_str());
    if (FAILED(hResult)) {
        throw std::runtime_error("Failed to navigate to URL");
    }
}

auto WebView::executeJavaScript(const std::string& script) -> void {
    if (!pimpl_->webView_) {
        return;
    }

    std::wstring wscript(script.begin(), script.end());
    HRESULT hResult = pimpl_->webView_->ExecuteScript(wscript.c_str(), nullptr);
    if (FAILED(hResult)) {
        throw std::runtime_error("Failed to execute JavaScript");
    }
}

auto WebView::sendMessageToJS(const std::string& message) -> void {
    if (!pimpl_->webView_) {
        return;
    }

    std::wstring wmessage(message.begin(), message.end());
    HRESULT hResult = pimpl_->webView_->PostWebMessageAsString(wmessage.c_str());
    if (FAILED(hResult)) {
        throw std::runtime_error("Failed to send message to JavaScript");
    }
}

auto WebView::reload() -> void {
    if (!pimpl_->webView_) {
        return;
    }

    HRESULT hResult = pimpl_->webView_->Reload();
    if (FAILED(hResult)) {
        throw std::runtime_error("Failed to reload WebView2");
    }
}

auto WebView::goBack() -> void {
    if (!pimpl_->webView_) {
        return;
    }

    HRESULT hResult = pimpl_->webView_->GoBack();
    if (FAILED(hResult)) {
        throw std::runtime_error("Failed to navigate back");
    }
}

auto WebView::goForward() -> void {
    if (!pimpl_->webView_) {
        return;
    }

    HRESULT hResult = pimpl_->webView_->GoForward();
    if (FAILED(hResult)) {
        throw std::runtime_error("Failed to navigate forward");
    }
}

auto WebView::stop() -> void {
    if (!pimpl_->webView_) {
        return;
    }

    HRESULT hResult = pimpl_->webView_->Stop();
    if (FAILED(hResult)) {
        throw std::runtime_error("Failed to stop navigation");
    }
}

auto WebView::openDevTools() -> void {
    if (!pimpl_->webView_) {
        return;
    }

    HRESULT hResult = pimpl_->webView_->OpenDevToolsWindow();
    if (FAILED(hResult)) {
        throw std::runtime_error("Failed to open DevTools window");
    }
}

auto WebView::setZoomFactor(float factor) -> void {
    if (!pimpl_->controller_) {
        return;
    }

    HRESULT hResult = pimpl_->controller_->put_ZoomFactor(factor);
    if (FAILED(hResult)) {
        throw std::runtime_error("Failed to set zoom factor");
    }
}

auto WebView::enableJavaScript(bool enable) -> void {
    if (!pimpl_->webView_) {
        return;
    }

    ComPtr<ICoreWebView2Settings> settings;
    HRESULT hResult = pimpl_->webView_->get_Settings(&settings);
    if (SUCCEEDED(hResult)) {
        settings->put_IsScriptEnabled(enable ? TRUE : FALSE);
    }
}

auto WebView::enableCookies(bool enable) -> void {
    if (!pimpl_->webView_) {
        return;
    }

    ComPtr<ICoreWebView2Settings> settings;
    if (SUCCEEDED(pimpl_->webView_->get_Settings(&settings))) {
        settings->put_IsWebMessageEnabled(enable);
    }
}

auto WebView::clearCookies() -> void {
    if (pimpl_->webView_) {
        return;
    }

    ComPtr<ICoreWebView2Profile> profile;
    if (SUCCEEDED(dynamic_cast<ICoreWebView2_13*>(pimpl_->webView_.Get())->get_Profile(&profile))) {
        ComPtr<ICoreWebView2Profile2> profile2;
        if (SUCCEEDED(profile.As(&profile2))) {
            profile2->ClearBrowsingData(COREWEBVIEW2_BROWSING_DATA_KINDS_COOKIES,
                                        Microsoft::WRL::Callback<ICoreWebView2ClearBrowsingDataCompletedHandler>(
                                            []([[maybe_unused]] HRESULT error) { return S_OK; })
                                            .Get());
        }
    }
}

auto WebView::resize(int width, int height) -> void {
    if (!pimpl_->controller_) {
        return;
    }

    RECT bounds;
    bounds.left = 0;
    bounds.top = 0;
    bounds.right = width;
    bounds.bottom = height;

    HRESULT hResult = pimpl_->controller_->put_Bounds(bounds);
    if (FAILED(hResult)) {
        throw std::runtime_error("Failed to resize WebView2");
    }
}

auto WebView::destroy() -> void { pimpl_->destroy(); }

auto WebView::registerMessageStrategy(std::unique_ptr<message::MessageStrategyBase> strategy) -> void {
    if (pimpl_->messageHandler_) {
        pimpl_->messageHandler_->registerStrategy(std::move(strategy));
    }
}

auto WebView::handleMessage(const std::string& message) -> void {
    if (pimpl_->messageHandler_) {
        pimpl_->messageHandler_->handleMessage(message);
    }
}

}  // namespace palantir::window::component::webview