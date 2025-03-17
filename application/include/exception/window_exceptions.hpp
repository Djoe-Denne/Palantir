#pragma once

#include "exception/base_exception.hpp"

namespace palantir::exception {

/**
 * @brief Thrown when an invalid window handle is encountered
 */
class InvalidHandleException : public BaseException {
public:
    explicit InvalidHandleException(const std::string& message = "Invalid window handle")
        : BaseException(message) {}
};

/**
 * @brief Thrown when WebView2 initialization fails
 */
class WebViewInitializationException : public BaseException {
public:
    explicit WebViewInitializationException(const std::string& message = "Failed to initialize WebView2")
        : BaseException(message) {}
};

/**
 * @brief Thrown when navigation within WebView fails
 */
class NavigationException : public BaseException {
public:
    explicit NavigationException(const std::string& message = "Failed to navigate to URL")
        : BaseException(message) {}
};

/**
 * @brief Thrown when JavaScript execution fails
 */
class JavaScriptExecutionException : public BaseException {
public:
    explicit JavaScriptExecutionException(const std::string& message = "Failed to execute JavaScript")
        : BaseException(message) {}
};

/**
 * @brief Thrown when sending a message to JavaScript fails
 */
class MessageSendException : public BaseException {
public:
    explicit MessageSendException(const std::string& message = "Failed to send message to JavaScript")
        : BaseException(message) {}
};

/**
 * @brief Thrown when a window operation fails
 */
class WindowOperationException : public BaseException {
public:
    explicit WindowOperationException(const std::string& message = "Window operation failed")
        : BaseException(message) {}
};

/**
 * @brief Thrown when setting up a keyboard hook fails
 */
class KeyboardHookException : public BaseException {
public:
    explicit KeyboardHookException(const std::string& message = "Failed to install keyboard hook")
        : BaseException(message) {}
};

} // namespace palantir::exception 