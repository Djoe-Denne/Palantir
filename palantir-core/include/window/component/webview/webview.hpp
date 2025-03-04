#ifndef INTERVIEW_CHEATER_WINDOW_COMPONENT_WEBVIEW_IWEBVIEW_HPP
#define INTERVIEW_CHEATER_WINDOW_COMPONENT_WEBVIEW_IWEBVIEW_HPP
#include <functional>
#include <string>
#include <memory>
#include "core_export.hpp"

namespace interview_cheater::window::component::webview {
/**
 * @class WebView
 * @brief An abstract class representing a web view component.
 *
 * This interface defines the common behavior for WebView components across different platforms.
 * It ensures that both WebView2 (Windows) and WKWebView (macOS) implementations provide similar functionality.
 *
 */
class PALANTIR_CORE_API WebView {
public:
    WebView();
    virtual ~WebView();

    WebView(const WebView&) = delete;
    auto operator=(const WebView&) -> WebView& = delete;
    WebView(WebView&&) noexcept = delete;
    auto operator=(WebView&&) noexcept -> WebView& = delete;

    /**
     * @brief Initializes the web view with a native window handle.
     *
     * @param nativeWindowHandle A pointer to the native window handle.
     * @param initCallback A callback function to be called when the web view is initialized.
     */
    virtual void initialize(void* nativeWindowHandle, std::function<void()> initCallback);

    /**
     * @brief Loads a URL in the web view.
     *
     * @param url The URL to load.
     */
    virtual void loadURL(const std::string& url);

    /**
     * @brief Executes a JavaScript script in the web view.
     *
     * @param script The JavaScript code to execute.
     */
    virtual void executeJavaScript(const std::string& script);

    /**
     * @brief Sends a message to the JavaScript context in the web view.
     *
     * @param message The message to send.
     */
    virtual void sendMessageToJS(const std::string& message);

    /**
     * @brief Sets a handler for messages received from the JavaScript context.
     *
     * @param handler A function to handle messages from JavaScript.
     */
    virtual void setMessageHandler(std::function<void(const std::string&)> handler);

    /**
     * @brief Reloads the current page in the web view.
     */
    virtual void reload();

    /**
     * @brief Navigates back to the previous page in the web view history.
     */
    virtual void goBack();

    /**
     * @brief Navigates forward to the next page in the web view history.
     */
    virtual void goForward();

    /**
     * @brief Stops the current navigation or loading process.
     */
    virtual void stop();

    /**
     * @brief Sets the zoom factor for the web view.
     *
     * @param factor The zoom factor to set.
     */
    virtual void setZoomFactor(float factor);

    /**
     * @brief Opens the developer tools for the web view.
     */
    virtual void openDevTools();

    /**
     * @brief Enables or disables JavaScript execution in the web view.
     *
     * @param enable True to enable JavaScript, false to disable.
     */
    virtual void enableJavaScript(bool enable);

    /**
     * @brief Enables or disables cookies in the web view.
     *
     * @param enable True to enable cookies, false to disable.
     */
    virtual void enableCookies(bool enable);

    /**
     * @brief Clears all cookies in the web view.
     */
    virtual void clearCookies();

    /**
     * @brief Resizes the web view.
     *
     * @param width The width of the web view.
     * @param height The height of the web view.
     */
    virtual void resize(int width, int height);

    /**
     * @brief Destroys the web view and releases any associated resources.
     */
    virtual void destroy();

private:
    class WebViewImpl;
#pragma warning(push)
#pragma warning(disable: 4251)
    std::shared_ptr<WebViewImpl> pimpl_;
#pragma warning(pop)

};
}  // namespace interview_cheater::window::component::webview

#endif  // INTERVIEW_CHEATER_WINDOW_COMPONENT_WEBVIEW_IWEBVIEW_HPP