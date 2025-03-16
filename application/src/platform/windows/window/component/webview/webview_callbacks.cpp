#include "window/component/webview/webview_callbacks.hpp"

#include "utils/resource_utils.hpp"
#include "utils/string_utils.hpp"
#include "window/component/webview/webview.hpp"

namespace palantir::window::component::webview {

WebViewCallbacks::WebViewCallbacks() = default;

void WebViewCallbacks::setInitCallback(std::function<void()> callback) { initCallback_ = std::move(callback); }

HRESULT WebViewCallbacks::handleEnvironmentCompleted(HRESULT result, ICoreWebView2Environment* env, WebView* webview) {
    if (FAILED(result)) {
        DEBUG_LOG("Failed to create WebView2 environment: ", result);
        return result;
    }
    if (!env) {
        DEBUG_LOG("WebView2 environment is null");
        return E_POINTER;
    }

    DEBUG_LOG("WebView2 environment created successfully");
    auto nativeHandle = reinterpret_cast<HWND>(webview->getNativeHandle());
    return env->CreateCoreWebView2Controller(nativeHandle, this->getControllerCompletedHandler(webview).Get());
}

HRESULT WebViewCallbacks::handleControllerCompleted(HRESULT result, ICoreWebView2Controller* controller, WebView* webview) {
    if (FAILED(result)) {
        DEBUG_LOG("Failed to create WebView2 controller: ", result);
        return result;
    }
    if (!controller) {
        DEBUG_LOG("WebView2 controller is null");
        return E_POINTER;
    }

    DEBUG_LOG("WebView2 controller created successfully");
    HRESULT hResult = webview->initializeController(reinterpret_cast<uintptr_t>(controller));  // NOLINT
    if (FAILED(hResult)) {
        return hResult;
    }

    if (this->initCallback_) {
        this->initCallback_();
    }
    return S_OK;
}

HRESULT WebViewCallbacks::handleWebMessageReceived([[maybe_unused]] const ICoreWebView2* sender, 
    ICoreWebView2WebMessageReceivedEventArgs* args, WebView* webview) {
    LPWSTR message = nullptr;
    if (args->TryGetWebMessageAsString(&message) == E_INVALIDARG || args->get_WebMessageAsJson(&message) == E_INVALIDARG) {
        DEBUG_LOG("Failed to get web message as String or JSON");
        return S_OK;
    }

    if (message) {
        std::wstring wmessage(message);
        std::string messageStr(wmessage.begin(), wmessage.end());
        DEBUG_LOG("Received WebView2 message: ", messageStr);

        try {
            webview->handleMessage(messageStr);
        } catch (const std::exception& e) {
            DEBUG_LOG("Exception in message handler: ", e.what());
        }
        CoTaskMemFree(message);
    }
    return S_OK;
}

HRESULT WebViewCallbacks::handleNavigationCompleted(ICoreWebView2* sender, ICoreWebView2NavigationCompletedEventArgs* args) {
    DEBUG_LOG("Navigation completed callback triggered");
    BOOL success;
    args->get_IsSuccess(&success);
    if (!success) {
        COREWEBVIEW2_WEB_ERROR_STATUS status;
        args->get_WebErrorStatus(&status);
        DEBUG_LOG("Navigation failed with error status: ", status);
        CoTaskMemFree(nullptr);
    } else {
        DEBUG_LOG("Navigation completed successfully");
        try {
            auto resourceUtils = palantir::utils::ResourceUtils::getInstance();
            auto scripts = resourceUtils->loadAllJavaScriptsFromDirectory("post-nav");

            for (const auto& [filename, script] : scripts) {
                DEBUG_LOG("Executing post-navigation script: ", filename);

                std::wstring wScript(script.begin(), script.end());
                sender->ExecuteScript(wScript.c_str(), nullptr);
            }
        } catch (const std::exception& e) {
            DEBUG_LOG("Failed to load and execute post-navigation scripts: ", e.what());
        }
    }
    return S_OK;
}

HRESULT WebViewCallbacks::handleSourceChanged(ICoreWebView2* sender, 
    [[maybe_unused]] const ICoreWebView2SourceChangedEventArgs* args) {
    LPWSTR uri;
    sender->get_Source(&uri);
    std::string uriStr = palantir::utils::StringUtils::wToStr(uri);
    DEBUG_LOG("WebView2 source changed to: ", uriStr);
    CoTaskMemFree(uri);
    return S_OK;
}

HRESULT WebViewCallbacks::handleExecuteScriptCompleted(HRESULT error, LPCWSTR result) {
    if (FAILED(error)) {
        DEBUG_LOG("Failed to execute script: ", error);
    } else {
        std::string resultStr = palantir::utils::StringUtils::wToStr(result);
        DEBUG_LOG("Script executed successfully, result: ", resultStr);
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

auto WebViewCallbacks::getNavigationCompletedHandler([[maybe_unused]] WebView* webview)
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
        [this](HRESULT error, LPCWSTR result) {
            return this->handleExecuteScriptCompleted(error, result);
        });
}

}  // namespace palantir::window::component::webview