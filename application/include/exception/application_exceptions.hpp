#pragma once

#include "exception/base_exception.hpp"
#include "exception/window_exceptions.hpp"
#include "exception/traceable_exception.hpp"

namespace palantir::exception {

// Helper type aliases for common traceable exceptions
using TraceableInvalidHandleException = TraceableException<InvalidHandleException>;
using TraceableWebViewInitializationException = TraceableException<WebViewInitializationException>;
using TraceableNavigationException = TraceableException<NavigationException>;
using TraceableJavaScriptExecutionException = TraceableException<JavaScriptExecutionException>;
using TraceableMessageSendException = TraceableException<MessageSendException>;
using TraceableWindowOperationException = TraceableException<WindowOperationException>;
using TraceableKeyboardHookException = TraceableException<KeyboardHookException>;
} // namespace palantir::exception 