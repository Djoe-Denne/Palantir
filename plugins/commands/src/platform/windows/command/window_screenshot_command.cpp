#ifdef _WIN32

#include "command/window_screenshot_command.hpp"
#include <Windows.h>
#include <objidl.h>  // Required for GDI+
#include <gdiplus.h>
#include <iostream>
#include <codecvt>
#include <locale>

#pragma comment(lib, "Gdiplus.lib")

namespace palantir::command {

auto stringToWString(const std::string& str) -> std::wstring {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.from_bytes(str);
}


auto GetEncoderClsid(const WCHAR* format, CLSID* pClsid) -> int {
    using namespace Gdiplus;
    UINT num = 0;
    UINT size = 0;
    GetImageEncodersSize(&num, &size);
    if (size == 0) return -1;
    
    auto* pImageCodecInfo = static_cast<ImageCodecInfo*>(malloc(size));
    if (!pImageCodecInfo) return -1;
    
    GetImageEncoders(num, size, pImageCodecInfo);
    for (UINT j = 0; j < num; ++j) {
        if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0) {
            *pClsid = pImageCodecInfo[j].Clsid;
            free(pImageCodecInfo);
            return j;
        }
    }
    free(pImageCodecInfo);
    return -1;
}

auto WindowScreenshotCommand::captureScreenshot() const -> void {
    using namespace Gdiplus;

    HWND hwnd = GetForegroundWindow();
    if (!hwnd) return;

    RECT rect;
    GetWindowRect(hwnd, &rect);
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;

    HDC hdcScreen = GetDC(nullptr);
    HDC hdcMem = CreateCompatibleDC(hdcScreen);
    HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, width, height);

    if (!hBitmap) {  // 🚨 Check if bitmap creation failed
        ReleaseDC(nullptr, hdcScreen);
        DeleteDC(hdcMem);
        return;
    }

    SelectObject(hdcMem, hBitmap);
    BitBlt(hdcMem, 0, 0, width, height, hdcScreen, rect.left, rect.top, SRCCOPY);

    // ✅ Initialize GDI+ Before using Bitmap
    GdiplusStartupInput gdiStartupInput;
    ULONG_PTR gdiToken;
    GdiplusStartup(&gdiToken, &gdiStartupInput, nullptr);

    // ✅ Proper way to create Bitmap from HBITMAP
    std::unique_ptr<Bitmap> bitmap(Bitmap::FromHBITMAP(hBitmap, nullptr));
    if (!bitmap || bitmap->GetLastStatus() != Ok) {
        std::cerr << "Failed to create GDI+ Bitmap!" << std::endl;
        GdiplusShutdown(gdiToken);
        DeleteObject(hBitmap);
        ReleaseDC(nullptr, hdcScreen);
        DeleteDC(hdcMem);
        return; // Exit if Bitmap initialization failed
    }

    CLSID pngClsid;
    GetEncoderClsid(L"image/png", &pngClsid);
    std::wstring wFilePath = stringToWString(generateFilePath());

    // ✅ Ensure Bitmap is saved BEFORE GdiplusShutdown
    if (bitmap->Save(wFilePath.c_str(), &pngClsid, nullptr) != Ok) {
        std::cerr << "Failed to save screenshot!" << std::endl;
    }

    // ✅ Destroy Bitmap before GDI+ shutdown
    bitmap.reset();

    // ✅ Now it's safe to shutdown GDI+
    GdiplusShutdown(gdiToken);

    // ✅ Clean up GDI resources
    DeleteObject(hBitmap);
    ReleaseDC(nullptr, hdcScreen);
    DeleteDC(hdcMem);
}

} // namespace palantir::command

#endif
