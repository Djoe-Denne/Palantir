#pragma once

#include <Windows.h>

#include <core_export.hpp>

#include "signal/keyboard_api.hpp"

namespace palantir::signal {

/**
 * @brief Windows-specific specialization of KeyboardHookTypes
 */
struct KeyboardHookTypes {
    using HookHandle = HHOOK;
    using HookProcedure = HOOKPROC;
    using ModuleHandle = HINSTANCE;
    using ThreadId = DWORD;
    using HookResult = LRESULT;
    using HookCode = int;
    using MessageParam = WPARAM;
    using LongParam = LPARAM;
    using BoolResult = BOOL;
    using ModuleStringType = LPCSTR;
};

/**
 * @brief Windows implementation of the KeyboardApi
 *
 */
class PALANTIR_CORE_API KeyboardApi {
public:
    /**
     * @brief Set a Windows keyboard hook
     * @param idHook The type of hook to set
     * @param lpfn Pointer to the hook procedure
     * @param hMod Handle to the DLL containing the hook procedure
     * @param dwThreadId Thread ID to associate with the hook
     * @return Hook handle if successful, nullptr otherwise
     */
    HHOOK SetHook(int idHook, HOOKPROC lpfn, HINSTANCE hMod, DWORD dwThreadId) const {
        return ::SetWindowsHookEx(idHook, lpfn, hMod, dwThreadId);
    }

    /**
     * @brief Remove a Windows hook
     * @param hhk Handle to the hook to remove
     * @return True if successful, false otherwise
     */
    BOOL UnhookKeyboard(HHOOK hhk) const { return ::UnhookWindowsHookEx(hhk); }

    /**
     * @brief Call the next hook in the hook chain
     * @param hhk Hook handle
     * @param nCode The hook code
     * @param wParam The WPARAM value
     * @param lParam The LPARAM value
     * @return Result of the hook chain
     */
    LRESULT CallNextHook(HHOOK hhk, int nCode, WPARAM wParam, LPARAM lParam) const {
        return ::CallNextHookEx(hhk, nCode, wParam, lParam);
    }

    /**
     * @brief Get module handle for the current process
     * @param lpModuleName Module name or nullptr for current process
     * @return Handle to the module if successful, nullptr otherwise
     */
    HINSTANCE GetModuleOSHandle(LPCSTR lpModuleName) const { return ::GetModuleHandleA(lpModuleName); }
};
}  // namespace palantir::signal
