#include "window/component/webview/webview_callbacks.hpp"
#include "window/component/webview/webview.hpp"
#include "utils/resource_utils.hpp"
#include "utils/string_utils.hpp"

namespace palantir::window::component::webview {

WebViewCallbacks::WebViewCallbacks() 
    : initCallback_(nullptr) {
}

void WebViewCallbacks::setInitCallback(std::function<void()> callback) {
    initCallback_ = std::move(callback);
}

Microsoft::WRL::ComPtr<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>
WebViewCallbacks::getEnvironmentCompletedHandler(WebView* webview) {
    return Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
        [webview, this](HRESULT result, ICoreWebView2Environment* env) -> HRESULT {
            if (FAILED(result)) {
                DEBUG_LOG("Failed to create WebView2 environment: ", result);
                return result;
            }
            if (!env) {
                DEBUG_LOG("WebView2 environment is null");
                return E_POINTER;
            }

            DEBUG_LOG("WebView2 environment created successfully");
            auto nativeHandle = static_cast<HWND>(webview->getNativeHandle());
            return env->CreateCoreWebView2Controller(
                nativeHandle,
                this->getControllerCompletedHandler(webview).Get());
        });
}

Microsoft::WRL::ComPtr<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>
WebViewCallbacks::getControllerCompletedHandler(WebView* webview) {
    return Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
        [webview, this](HRESULT result, ICoreWebView2Controller* controller) -> HRESULT {
            if (FAILED(result)) {
                DEBUG_LOG("Failed to create WebView2 controller: ", result);
                return result;
            }
            if (!controller) {
                DEBUG_LOG("WebView2 controller is null");
                return E_POINTER;
            }

            DEBUG_LOG("WebView2 controller created successfully");
            HRESULT hResult = static_cast<HRESULT>(webview->initializeController(controller));
            if (FAILED(hResult)) {
                return hResult;
            }

            if (this->initCallback_) {
                this->initCallback_();
            }
            return S_OK;
            
        });
}

Microsoft::WRL::ComPtr<ICoreWebView2WebMessageReceivedEventHandler>
WebViewCallbacks::getWebMessageReceivedHandler(WebView* webview) {
    return Microsoft::WRL::Callback<ICoreWebView2WebMessageReceivedEventHandler>(
        [webview, this](ICoreWebView2* sender, ICoreWebView2WebMessageReceivedEventArgs* args) -> HRESULT {
            LPWSTR message = nullptr;
            LPWSTR uri = nullptr;
            args->get_Source(&uri);
            HRESULT hr = args->TryGetWebMessageAsString(&message);
            if (hr == E_INVALIDARG) {
                hr = args->get_WebMessageAsJson(&message);
                if (hr == E_INVALIDARG) {
                    DEBUG_LOG("Failed to get web message as String or JSON");
                    return S_OK;
                }
            }
            
            if (message) {
                std::wstring wmessage(message);
                std::string messageStr(wmessage.begin(), wmessage.end());
                DEBUG_LOG("Received WebView2 message: ", messageStr);
                
                try {
                    // Try to handle the message with the new MessageHandler first
                    webview->handleMessage(messageStr);
                } catch (const std::exception& e) {
                    DEBUG_LOG("Exception in message handler: ", e.what());
                } catch (...) {
                    DEBUG_LOG("Unknown exception in message handler");
                }
                
                CoTaskMemFree(message);
            }
            
            return S_OK;
        });
}

Microsoft::WRL::ComPtr<ICoreWebView2NavigationCompletedEventHandler>
WebViewCallbacks::getNavigationCompletedHandler(WebView* webview) {
    return Microsoft::WRL::Callback<ICoreWebView2NavigationCompletedEventHandler>(
        [webview](ICoreWebView2* sender, ICoreWebView2NavigationCompletedEventArgs* args) -> HRESULT {
            DEBUG_LOG("Navigation completed callback triggered");
            BOOL success;
            args->get_IsSuccess(&success);
            if (!success) {
                COREWEBVIEW2_WEB_ERROR_STATUS status;
                args->get_WebErrorStatus(&status);
                DEBUG_LOG("Navigation failed with error status: ", status);

                LPWSTR uri;
                sender->get_Source(&uri);
                std::string uriStr = palantir::utils::StringUtils::wToStr(uri);
                DEBUG_LOG("Failed navigation URL: ", uriStr);
                CoTaskMemFree(uri);
            } else {
                DEBUG_LOG("Navigation completed successfully");

                // Load and execute all JavaScript files in the post-nav directory
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
        });
}

Microsoft::WRL::ComPtr<ICoreWebView2SourceChangedEventHandler>
WebViewCallbacks::getSourceChangedHandler() {
    return Microsoft::WRL::Callback<ICoreWebView2SourceChangedEventHandler>(
        [](ICoreWebView2* sender, ICoreWebView2SourceChangedEventArgs* args) -> HRESULT {
            LPWSTR uri;
            sender->get_Source(&uri);
            std::string uriStr = palantir::utils::StringUtils::wToStr(uri);
            DEBUG_LOG("WebView2 source changed to: ", uriStr);
            CoTaskMemFree(uri);
            return S_OK;
        });
}

Microsoft::WRL::ComPtr<ICoreWebView2ExecuteScriptCompletedHandler>
WebViewCallbacks::getExecuteScriptCompletedHandler() {
    return Microsoft::WRL::Callback<ICoreWebView2ExecuteScriptCompletedHandler>(
        [](HRESULT error, LPCWSTR result) -> HRESULT {
            if (FAILED(error)) {
                DEBUG_LOG("Failed to execute script: ", error);
            } else {
                std::string resultStr = palantir::utils::StringUtils::wToStr(result);
                DEBUG_LOG("Script executed successfully, result: ", resultStr);
            }
            return S_OK;
        });
}

} // namespace palantir::window::component::webview 