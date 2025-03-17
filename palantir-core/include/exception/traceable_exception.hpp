#pragma once

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

// Modern C++ stacktrace (preferred when available)
#if _GLIBCXX_HAVE_STACKTRACE
#include <stacktrace>
#define HAS_STD_STACKTRACE
#endif

// OS detection
#if !defined(_GLIBCXX_HAVE_STACKTRACE) && (defined(_WIN32) || defined(_WIN64))
#define OS_WINDOWS
// clang-format off
#include <windows.h>
#include <DbgHelp.h>
#pragma comment(lib, "DbgHelp.lib")
// clang-format on
#elif !defined(_GLIBCXX_HAVE_STACKTRACE) && (defined(__APPLE__) || defined(__MACH__))
#define OS_MACOS
#include <cxxabi.h>
#include <dlfcn.h>
#include <execinfo.h>
#elif !defined(_GLIBCXX_HAVE_STACKTRACE) && (defined(__linux__) || defined(__unix__))
#define OS_LINUX
#include <cxxabi.h>
#include <execinfo.h>
#endif

#include "core_export.hpp"

namespace palantir::exception {
#pragma warning(push)
#pragma warning(disable : 4251)
class PALANTIR_CORE_API TraceableBaseException : public std::exception {
public:
    ~TraceableBaseException() override = default;
    virtual std::string getStackTraceString() const = 0;
};

template <typename E>
    requires std::is_base_of_v<std::exception, E>
class PALANTIR_CORE_API TraceableException : public E, public TraceableBaseException {
#pragma warning(pop)
public:
    ~TraceableException() override = default;
#if defined(HAS_STD_STACKTRACE)
    // Modern C++23 implementation
    explicit TraceableException(const std::string &what) : E(what) { stackTrace = std::stacktrace::current(); }

    const std::stacktrace &getStackTrace() const { return stackTrace; }

    std::string getStackTraceString() const override { return format_stacktrace(stackTrace); }

private:
#pragma warning(push)
#pragma warning(disable : 4251)
    std::stacktrace stackTrace;
#pragma warning(pop)

    std::string format_stacktrace(const std::stacktrace &st) const {
        std::ostringstream oss;
        for (const auto &entry : st) {
            oss << entry << '\n';
        }
        return oss.str();
    }
#elif defined(OS_WINDOWS)
    // Windows-specific implementation
    explicit TraceableException(const std::string& what) : E(what) { captureStackTrace(); }

    std::string getStackTraceString() const override { return stackTraceStr; }

private:
    static const int MAX_STACK_FRAMES = 64;
    std::string stackTraceStr;

    void captureStackTrace() {
        HANDLE process = GetCurrentProcess();
        SymInitialize(process, NULL, TRUE);

        void* stack[MAX_STACK_FRAMES];
        WORD frames = CaptureStackBackTrace(0, MAX_STACK_FRAMES, stack, NULL);

        SYMBOL_INFO* symbol = (SYMBOL_INFO*)calloc(sizeof(SYMBOL_INFO) + 256 * sizeof(char), 1);
        symbol->MaxNameLen = 255;
        symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

        std::ostringstream trace;
        trace << "Stack trace:\n";

        for (int i = 0; i < frames; i++) {
            SymFromAddr(process, (DWORD64)(stack[i]), 0, symbol);
            IMAGEHLP_LINE64 line;
            line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
            DWORD displacement;

            if (SymGetLineFromAddr64(process, (DWORD64)(stack[i]), &displacement, &line)) {
                trace << "\t" << symbol->Name << " at " << line.FileName << ":" << line.LineNumber << "\n";
            } else {
                trace << "\t" << symbol->Name << " at unknown location\n";
            }
        }

        free(symbol);
        stackTraceStr = trace.str();
    }
#elif defined(OS_LINUX) || defined(OS_MACOS)
    // Linux/macOS implementation using execinfo
    explicit TraceableException(const std::string& what) : E(what) { captureStackTrace(); }

    std::string getStackTraceString() const override { return stackTraceStr; }

private:
    static const int MAX_STACK_FRAMES = 64;
    std::string stackTraceStr;

    void captureStackTrace() {
        void* callstack[MAX_STACK_FRAMES];
        int frames = backtrace(callstack, MAX_STACK_FRAMES);
        char** strs = backtrace_symbols(callstack, frames);

        std::ostringstream trace;
        trace << "Stack trace:\n";

        for (int i = 0; i < frames; i++) {
#if defined(OS_MACOS)
            // Parse macOS style: frame number, binary name, address, offset, function name
            char* begin_name = nullptr;
            char* begin_offset = nullptr;
            char* end_offset = nullptr;

            // Find function name and offset
            for (char* p = strs[i]; *p; ++p) {
                if (*p == '(') {
                    begin_name = p;
                } else if (*p == '+') {
                    begin_offset = p;
                } else if (*p == ')' && begin_offset) {
                    end_offset = p;
                    break;
                }
            }

            if (begin_name && begin_offset && end_offset && begin_name < begin_offset) {
                *begin_name++ = '\0';
                *begin_offset++ = '\0';
                *end_offset = '\0';

                // Demangle the function name using abi::__cxa_demangle
                int status;
                char* demangled = abi::__cxa_demangle(begin_name, nullptr, nullptr, &status);
                if (status == 0 && demangled) {
                    trace << "\t" << strs[i] << ": " << demangled << " + " << begin_offset << "\n";
                    free(demangled);
                } else {
                    trace << "\t" << strs[i] << ": " << begin_name << " + " << begin_offset << "\n";
                }
            } else {
                // Could not parse the line, print as is
                trace << "\t" << strs[i] << "\n";
            }
#else
            // Linux style parsing
            char* mangled_name = nullptr;
            char* offset_begin = nullptr;
            char* offset_end = nullptr;

            // Find function name between parentheses
            for (char* p = strs[i]; *p; ++p) {
                if (*p == '(') {
                    mangled_name = p + 1;
                } else if (*p == '+') {
                    offset_begin = p;
                } else if (*p == ')') {
                    offset_end = p;
                    break;
                }
            }

            if (mangled_name && offset_begin && offset_end && mangled_name < offset_begin) {
                *offset_begin++ = '\0';
                *offset_end = '\0';

                // Demangle the function name
                int status;
                char* demangled = abi::__cxa_demangle(mangled_name, nullptr, nullptr, &status);
                if (status == 0 && demangled) {
                    trace << "\t" << demangled << " [" << strs[i] << "]\n";
                    free(demangled);
                } else {
                    trace << "\t" << mangled_name << " [" << strs[i] << "]\n";
                }
            } else {
                // Couldn't parse, print as is
                trace << "\t" << strs[i] << "\n";
            }
#endif
        }

        free(strs);
        stackTraceStr = trace.str();
    }
#else
    // Fallback implementation for unsupported platforms
    explicit TraceableException(const std::string &what) : E(what) {}

    std::string getStackTraceString() const override { return "Stack trace not available on this platform"; }
#endif
};

}  // namespace palantir::exception
