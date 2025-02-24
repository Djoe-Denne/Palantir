#ifndef INTERVIEW_CHEATER_WINDOW_COMPONENT_WEBVIEW_IWEBVIEW_HPP
#define INTERVIEW_CHEATER_WINDOW_COMPONENT_WEBVIEW_IWEBVIEW_HPP
#include <functional>
#include <string>
#include "core_export.hpp"

namespace interview_cheater::window::component {
/**
 * @class WebView
 * @brief An abstract class representing a web view component.
 *
 * This interface defines the common behavior for WebView components across different platforms.
 * It ensures that both WebView2 (Windows) and WKWebView (macOS) implementations provide similar functionality.
 *
 */
class PALANTIR_CORE_API WebView {
    /**
     * @brief Initializes the web view with a native window handle.
     *
     * @param nativeWindowHandle A pointer to the native window handle.
     */
    virtual void initialize(void* nativeWindowHandle) = 0;

    /**
     * @brief Loads a URL in the web view.
     *
     * @param url The URL to load.
     */
    virtual void loadURL(const std::string& url) = 0;

    /**
     * @brief Executes a JavaScript script in the web view.
     *
     * @param script The JavaScript code to execute.
     */
    virtual void executeJavaScript(const std::string& script) = 0;

    /**
     * @brief Sends a message to the JavaScript context in the web view.
     *
     * @param message The message to send.
     */
    virtual void sendMessageToJS(const std::string& message) = 0;

    /**
     * @brief Sets a handler for messages received from the JavaScript context.
     *
     * @param handler A function to handle messages from JavaScript.
     */
    virtual void setMessageHandler(std::function<void(const std::string&)> handler) = 0;

    /**
     * @brief Reloads the current page in the web view.
     */
    virtual void reload() = 0;

    /**
     * @brief Navigates back to the previous page in the web view history.
     */
    virtual void goBack() = 0;

    /**
     * @brief Navigates forward to the next page in the web view history.
     */
    virtual void goForward() = 0;

    /**
     * @brief Stops the current navigation or loading process.
     */
    virtual void stop() = 0;

    /**
     * @brief Sets the zoom factor for the web view.
     *
     * @param factor The zoom factor to set.
     */
    virtual void setZoomFactor(float factor) = 0;

    /**
     * @brief Opens the developer tools for the web view.
     */
    virtual void openDevTools() = 0;

    /**
     * @brief Enables or disables JavaScript execution in the web view.
     *
     * @param enable True to enable JavaScript, false to disable.
     */
    virtual void enableJavaScript(bool enable) = 0;

    /**
     * @brief Enables or disables cookies in the web view.
     *
     * @param enable True to enable cookies, false to disable.
     */
    virtual void enableCookies(bool enable) = 0;

    /**
     * @brief Clears all cookies in the web view.
     */
    virtual void clearCookies() = 0;

    /**
     * @brief Destroys the web view and releases any associated resources.
     */
    virtual void destroy() = 0;
};
}  // namespace interview_cheater::window::component

#endif  // INTERVIEW_CHEATER_WINDOW_COMPONENT_WEBVIEW_IWEBVIEW_HPP