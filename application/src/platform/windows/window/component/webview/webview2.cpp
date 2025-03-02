#include <WebView2.h>
#include <WebView2EnvironmentOptions.h>
#include <wrl.h>

#include <stdexcept>
#include <string>

#include "utils/logger.hpp"
#include "window/component/webview/webview.hpp"

using Microsoft::WRL::ComPtr;

namespace interview_cheater::window::component::webview {

class WebView::WebViewImpl {
public:
    WebViewImpl() = default;
    ~WebViewImpl() { destroy(); }

    HWND hwnd_{nullptr};
    ComPtr<ICoreWebView2> webView_;
    ComPtr<ICoreWebView2Controller> controller_;
    std::function<void(const std::string&)> messageHandler_;
    std::function<void()> initCallback_;
    EventRegistrationToken messageToken_;
    EventRegistrationToken navigationToken_;
    EventRegistrationToken sourceToken_;

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

WebView::WebView() : pimpl_(std::make_unique<WebViewImpl>()) {}

WebView::~WebView() = default;

auto WebView::initialize(void* nativeWindowHandle, std::function<void()> initCallback) -> void {
    pimpl_->hwnd_ = static_cast<HWND>(nativeWindowHandle);
    if (!pimpl_->hwnd_) {
        throw std::runtime_error("Invalid window handle");
    }

    // Store the initialization callback
    pimpl_->initCallback_ = std::move(initCallback);

    DEBUG_LOG("Starting WebView2 initialization");

    // Create WebView2 Environment with explicit options
    auto options = Microsoft::WRL::Make<CoreWebView2EnvironmentOptions>();
    if (!options) {
        throw std::runtime_error("Failed to create WebView2 environment options");
    }

    // Set background color to white to ensure visibility
    options->put_AdditionalBrowserArguments(L"--disable-background-timer-throttling");

    HRESULT hr = CreateCoreWebView2EnvironmentWithOptions(
        nullptr,  // Use default browser installation
        nullptr,  // Use default user data folder
        options.Get(),
        Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [this](HRESULT result, ICoreWebView2Environment* env) -> HRESULT {
                if (FAILED(result)) {
                    DEBUG_LOG("Failed to create WebView2 environment: 0x%08x", result);
                    return result;
                }
                if (!env) {
                    DEBUG_LOG("WebView2 environment is null");
                    return E_POINTER;
                }

                DEBUG_LOG("WebView2 environment created successfully");

                // Create WebView2 Controller
                return env->CreateCoreWebView2Controller(
                    pimpl_->hwnd_,
                    Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                        [this](HRESULT result, ICoreWebView2Controller* controller) -> HRESULT {
                            if (FAILED(result)) {
                                DEBUG_LOG("Failed to create WebView2 controller: 0x%08x", result);
                                return result;
                            }
                            if (!controller) {
                                DEBUG_LOG("WebView2 controller is null");
                                return E_POINTER;
                            }

                            DEBUG_LOG("WebView2 controller created successfully");
                            pimpl_->controller_ = controller;

                            // Get WebView2 from controller
                            HRESULT hr = controller->get_CoreWebView2(&pimpl_->webView_);
                            if (FAILED(hr) || !pimpl_->webView_) {
                                DEBUG_LOG("Failed to get CoreWebView2 from controller: 0x%08x", hr);
                                return hr;
                            }

                            DEBUG_LOG("CoreWebView2 obtained successfully");

                            // Configure WebView2 settings
                            ComPtr<ICoreWebView2Settings> settings;
                            hr = pimpl_->webView_->get_Settings(&settings);
                            if (SUCCEEDED(hr)) {
                                settings->put_IsScriptEnabled(TRUE);
                                settings->put_AreDefaultScriptDialogsEnabled(TRUE);
                                settings->put_IsWebMessageEnabled(TRUE);
                                settings->put_IsStatusBarEnabled(FALSE);
                                settings->put_AreDevToolsEnabled(TRUE);
                                DEBUG_LOG("WebView2 settings configured successfully");
                            } else {
                                DEBUG_LOG("Failed to configure WebView2 settings: 0x%08x", hr);
                            }

                            // Add source changed handler first to track all changes
                            hr = pimpl_->webView_->add_SourceChanged(
                                Microsoft::WRL::Callback<ICoreWebView2SourceChangedEventHandler>(
                                    [](ICoreWebView2* sender, ICoreWebView2SourceChangedEventArgs* args) -> HRESULT {
                                        LPWSTR uri;
                                        sender->get_Source(&uri);
                                        DEBUG_LOG("WebView2 source changed to: %ls", uri ? uri : L"<null>");
                                        CoTaskMemFree(uri);
                                        return S_OK;
                                    })
                                    .Get(),
                                &pimpl_->sourceToken_);

                            if (FAILED(hr)) {
                                DEBUG_LOG("Failed to add source changed handler: 0x%08x", hr);
                            }

                            // Add navigation completed handler before starting navigation
                            hr = pimpl_->webView_->add_NavigationCompleted(
                                Microsoft::WRL::Callback<ICoreWebView2NavigationCompletedEventHandler>(
                                    [this](ICoreWebView2* sender,
                                           ICoreWebView2NavigationCompletedEventArgs* args) -> HRESULT {
                                        DEBUG_LOG("Navigation completed callback triggered");
                                        BOOL success;
                                        args->get_IsSuccess(&success);
                                        if (!success) {
                                            COREWEBVIEW2_WEB_ERROR_STATUS status;
                                            args->get_WebErrorStatus(&status);
                                            DEBUG_LOG("Navigation failed with error status: %d", status);

                                            // Try to get more error details
                                            LPWSTR uri;
                                            sender->get_Source(&uri);
                                            DEBUG_LOG("Failed navigation URL: %ls", uri ? uri : L"<null>");
                                            CoTaskMemFree(uri);
                                        } else {
                                            DEBUG_LOG("Navigation completed successfully");

                                            // Execute test JavaScript after successful navigation
                                            const wchar_t* testScript =
                                                LR"(
                                                try {
                                                    console.log('WebView2 JavaScript test starting');
                                                    window.chrome.webview.postMessage({
                                                        type: 'test',
                                                        message: 'WebView2 is working!'
                                                    });
                                                    document.body.style.backgroundColor = '#ffffff';
                                                    console.log('WebView2 JavaScript test completed');
                                                } catch (e) {
                                                    console.error('WebView2 test error:', e);
                                                }
                                            )";

                                            sender->ExecuteScript(
                                                testScript,
                                                Microsoft::WRL::Callback<ICoreWebView2ExecuteScriptCompletedHandler>(
                                                    [](HRESULT error, LPCWSTR result) -> HRESULT {
                                                        if (FAILED(error)) {
                                                            DEBUG_LOG("Failed to execute test script: 0x%08x", error);
                                                        } else {
                                                            DEBUG_LOG("Test script executed successfully, result: %ls",
                                                                      result ? result : L"<null>");
                                                        }
                                                        return S_OK;
                                                    })
                                                    .Get());
                                        }
                                        return S_OK;
                                    })
                                    .Get(),
                                &pimpl_->navigationToken_);

                            if (FAILED(hr)) {
                                DEBUG_LOG("Failed to add navigation completed handler: 0x%08x", hr);
                            }

                            // Set up message handler
                            hr = pimpl_->webView_->add_WebMessageReceived(
                                Microsoft::WRL::Callback<ICoreWebView2WebMessageReceivedEventHandler>(
                                    [this](ICoreWebView2* sender,
                                           ICoreWebView2WebMessageReceivedEventArgs* args) -> HRESULT {
                                        LPWSTR message = nullptr;
                                        args->TryGetWebMessageAsString(&message);
                                        if (pimpl_->messageHandler_ && message) {
                                            std::wstring wmessage(message);
                                            DEBUG_LOG("Received WebView2 message: %ls", message);
                                            pimpl_->messageHandler_(std::string(wmessage.begin(), wmessage.end()));
                                        }
                                        CoTaskMemFree(message);
                                        return S_OK;
                                    })
                                    .Get(),
                                &pimpl_->messageToken_);

                            if (FAILED(hr)) {
                                DEBUG_LOG("Failed to add message received handler: 0x%08x", hr);
                            }

                            // Show WebView and set initial size
                            RECT bounds;
                            GetClientRect(pimpl_->hwnd_, &bounds);
                            DEBUG_LOG("Setting WebView2 bounds to: left=%ld, top=%ld, right=%ld, bottom=%ld",
                                      bounds.left, bounds.top, bounds.right, bounds.bottom);
                            controller->put_Bounds(bounds);
                            controller->put_IsVisible(TRUE);

                            DEBUG_LOG("WebView2 initialization completed successfully");

                            // Call the initialization callback if provided
                            if (pimpl_->initCallback_) {
                                DEBUG_LOG("Calling initialization callback");
                                pimpl_->initCallback_();
                            }

                            return S_OK;
                        })
                        .Get());
            })
            .Get());

    if (FAILED(hr)) {
        DEBUG_LOG("Failed to start WebView2 environment creation: 0x%08x", hr);
        throw std::runtime_error("Failed to create WebView2 environment");
    }

    DEBUG_LOG("WebView2 initialization process started");
}

auto WebView::loadURL(const std::string& url) -> void {
    if (pimpl_->webView_) {
        std::wstring wurl(url.begin(), url.end());
        pimpl_->webView_->Navigate(wurl.c_str());
    }
}

auto WebView::executeJavaScript(const std::string& script) -> void {
    if (pimpl_->webView_) {
        std::wstring wscript(script.begin(), script.end());
        pimpl_->webView_->ExecuteScript(wscript.c_str(), nullptr);
    }
}

auto WebView::sendMessageToJS(const std::string& message) -> void {
    if (pimpl_->webView_) {
        std::wstring wmessage(message.begin(), message.end());
        pimpl_->webView_->PostWebMessageAsString(wmessage.c_str());
    }
}

auto WebView::setMessageHandler(std::function<void(const std::string&)> handler) -> void {
    pimpl_->messageHandler_ = std::move(handler);
}

auto WebView::reload() -> void {
    if (pimpl_->webView_) {
        pimpl_->webView_->Reload();
    }
}

auto WebView::goBack() -> void {
    if (pimpl_->webView_) {
        pimpl_->webView_->GoBack();
    }
}

auto WebView::goForward() -> void {
    if (pimpl_->webView_) {
        pimpl_->webView_->GoForward();
    }
}

auto WebView::stop() -> void {
    if (pimpl_->webView_) {
        pimpl_->webView_->Stop();
    }
}

auto WebView::setZoomFactor(float factor) -> void {
    if (pimpl_->controller_) {
        pimpl_->controller_->put_ZoomFactor(factor);
    }
}

auto WebView::openDevTools() -> void {
    if (pimpl_->webView_) {
        pimpl_->webView_->OpenDevToolsWindow();
    }
}

auto WebView::enableJavaScript(bool enable) -> void {
    if (pimpl_->webView_) {
        ComPtr<ICoreWebView2Settings> settings;
        if (SUCCEEDED(pimpl_->webView_->get_Settings(&settings))) {
            settings->put_IsScriptEnabled(enable);
        }
    }
}

auto WebView::enableCookies(bool enable) -> void {
    if (pimpl_->webView_) {
        ComPtr<ICoreWebView2Settings> settings;
        if (SUCCEEDED(pimpl_->webView_->get_Settings(&settings))) {
            settings->put_IsWebMessageEnabled(enable);
        }
    }
}

auto WebView::clearCookies() -> void {
    if (pimpl_->webView_) {
        ComPtr<ICoreWebView2Profile> profile;
        if (SUCCEEDED(dynamic_cast<ICoreWebView2_13*>(pimpl_->webView_.Get())->get_Profile(&profile))) {
            ComPtr<ICoreWebView2Profile2> profile2;
            if (SUCCEEDED(profile.As(&profile2))) {
                profile2->ClearBrowsingData(COREWEBVIEW2_BROWSING_DATA_KINDS_COOKIES,
                                            Microsoft::WRL::Callback<ICoreWebView2ClearBrowsingDataCompletedHandler>(
                                                [](HRESULT error) -> HRESULT { return S_OK; })
                                                .Get());
            }
        }
    }
}

auto WebView::resize(int width, int height) -> void {
    if (pimpl_->controller_) {
        RECT bounds = {0, 0, width, height};
        pimpl_->controller_->put_Bounds(bounds);
    }
}

auto WebView::destroy() -> void { pimpl_->destroy(); }

}  // namespace interview_cheater::window::component::webview