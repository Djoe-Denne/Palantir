#include "window/component/webview/webview_callbacks.hpp"

#include "utils/resource_utils.hpp"
#include "utils/string_utils.hpp"
#include "window/component/webview/webview.hpp"

namespace palantir::window::component::webview {

WebViewCallbacks::WebViewCallbacks() = default;

void WebViewCallbacks::setInitCallback(std::function<void()> callback) { initCallback_ = std::move(callback); }

auto WebViewCallbacks::handleEnvironmentCompleted(HRESULT result, ICoreWebView2Environment* env, WebView* webview)
    -> HRESULT {
    if (FAILED(result)) {
        DebugLog("Failed to create WebView2 environment: ", result);
        return result;
    }
    if (!env) {
        DebugLog("WebView2 environment is null");
        return E_POINTER;
    }

    DebugLog("WebView2 environment created successfully");
    auto nativeHandle = reinterpret_cast<HWND>(webview->getNativeHandle());  // NOLINT
    return env->CreateCoreWebView2Controller(nativeHandle, this->getControllerCompletedHandler(webview).Get());
}

auto WebViewCallbacks::handleControllerCompleted(HRESULT result, ICoreWebView2Controller* controller,
                                                 WebView* webview) const -> HRESULT {
    if (FAILED(result)) {
        DebugLog("Failed to create WebView2 controller: ", result);
        return result;
    }
    if (!controller) {
        DebugLog("WebView2 controller is null");
        return E_POINTER;
    }

    DebugLog("WebView2 controller created successfully");
    HRESULT hResult = webview->initializeController(reinterpret_cast<uintptr_t>(controller));  // NOLINT
    if (FAILED(hResult)) {
        return hResult;
    }

    if (this->initCallback_) {
        this->initCallback_();
    }
    return S_OK;
}

auto WebViewCallbacks::handleWebMessageReceived([[maybe_unused]] const ICoreWebView2* sender,
                                                ICoreWebView2WebMessageReceivedEventArgs* args, WebView* webview) const
    -> HRESULT {
    LPWSTR message = nullptr;
    if (args->TryGetWebMessageAsString(&message) == E_INVALIDARG ||
        args->get_WebMessageAsJson(&message) == E_INVALIDARG) {
        DebugLog("Failed to get web message as String or JSON");
        return S_OK;
    }

    if (message != nullptr) {
        std::wstring wmessage(message);
        std::string messageStr(wmessage.begin(), wmessage.end());
        DebugLog("Received WebView2 message: ", messageStr);

        webview->handleMessage(messageStr);
        CoTaskMemFree(message);
    }
    return S_OK;
}

auto WebViewCallbacks::handleNavigationCompleted(ICoreWebView2* sender,
                                                 ICoreWebView2NavigationCompletedEventArgs* args) const -> HRESULT {
    DebugLog("Navigation completed callback triggered");
    BOOL success;
    args->get_IsSuccess(&success);
    if (!success) {
        COREWEBVIEW2_WEB_ERROR_STATUS status;
        args->get_WebErrorStatus(&status);
        DebugLog("Navigation failed with error status: ", status);
        CoTaskMemFree(nullptr);
    } else {
        DebugLog("Navigation completed successfully");
        auto resourceUtils = palantir::utils::ResourceUtils::getInstance();
        std::vector<std::pair<std::string, std::string>> scripts;
        try {
            scripts = resourceUtils->loadAllJavaScriptsFromDirectory("post-nav");
        } catch (const std::ifstream::failure& e) {
            DebugLog("Failed to load and execute post-navigation scripts: ", e.what());
        }

        for (const auto& [filename, script] : scripts) {
            DebugLog("Executing post-navigation script: ", filename);

            std::wstring wScript(script.begin(), script.end());
            sender->ExecuteScript(wScript.c_str(), nullptr);
        }
    }
    return S_OK;
}

auto WebViewCallbacks::handleSourceChanged(ICoreWebView2* sender,
                                           [[maybe_unused]] const ICoreWebView2SourceChangedEventArgs* args) const
    -> HRESULT {
    LPWSTR uri;
    sender->get_Source(&uri);
    std::string uriStr = palantir::utils::StringUtils::wToStr(uri);
    DebugLog("WebView2 source changed to: ", uriStr);
    CoTaskMemFree(uri);
    return S_OK;
}

auto WebViewCallbacks::handleExecuteScriptCompleted(HRESULT error, LPCWSTR result) const -> HRESULT {
    if (FAILED(error)) {
        DebugLog("Failed to execute script: ", error);
    } else {
        std::string resultStr = palantir::utils::StringUtils::wToStr(result);
        DebugLog("Script executed successfully, result: ", resultStr);
    }
    return S_OK;
}

auto WebViewCallbacks::getEnvironmentCompletedHandler(WebView* webview)
    -> Microsoft::WRL::ComPtr<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler> {
    return Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
        [webview, this](HRESULT result, ICoreWebView2Environment* env) {
            return this->handleEnvironmentCompleted(result, env, webview);
        });
}

auto WebViewCallbacks::getControllerCompletedHandler(WebView* webview)
    -> Microsoft::WRL::ComPtr<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler> {
    return Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
        [webview, this](HRESULT result, ICoreWebView2Controller* controller) {
            return this->handleControllerCompleted(result, controller, webview);
        });
}

auto WebViewCallbacks::getWebMessageReceivedHandler(WebView* webview)
    -> Microsoft::WRL::ComPtr<ICoreWebView2WebMessageReceivedEventHandler> {
    return Microsoft::WRL::Callback<ICoreWebView2WebMessageReceivedEventHandler>(
        [webview, this]([[maybe_unused]] const ICoreWebView2* sender, ICoreWebView2WebMessageReceivedEventArgs* args) {
            return this->handleWebMessageReceived(sender, args, webview);
        });
}

auto WebViewCallbacks::getNavigationCompletedHandler([[maybe_unused]] const WebView* webview)
    -> Microsoft::WRL::ComPtr<ICoreWebView2NavigationCompletedEventHandler> {
    return Microsoft::WRL::Callback<ICoreWebView2NavigationCompletedEventHandler>(
        [this](ICoreWebView2* sender, ICoreWebView2NavigationCompletedEventArgs* args) {
            return this->handleNavigationCompleted(sender, args);
        });
}

auto WebViewCallbacks::getSourceChangedHandler() -> Microsoft::WRL::ComPtr<ICoreWebView2SourceChangedEventHandler> {
    return Microsoft::WRL::Callback<ICoreWebView2SourceChangedEventHandler>(
        [this](ICoreWebView2* sender, const ICoreWebView2SourceChangedEventArgs* args) {
            return this->handleSourceChanged(sender, args);
        });
}

auto WebViewCallbacks::getExecuteScriptCompletedHandler()
    -> Microsoft::WRL::ComPtr<ICoreWebView2ExecuteScriptCompletedHandler> {
    return Microsoft::WRL::Callback<ICoreWebView2ExecuteScriptCompletedHandler>(
        [this](HRESULT error, LPCWSTR result) { return this->handleExecuteScriptCompleted(error, result); });
}

}  // namespace palantir::window::component::webview