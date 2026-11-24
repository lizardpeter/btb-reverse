#pragma once
#if defined(_WIN32)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <ddraw.h>
#include <string>

namespace btb::full_game {

// Original ExportAndPrintGameImage (0x00409730) captures the current DD7
// certificate frame and displays a modal native Windows PrintDlg before
// rendering a bitmap to the returned printer DC. No export-to-disk step.
// User cancellation is a no-op, not a successful printed certificate.
class Win32RetailPrintProvider final {
public:
    [[nodiscard]] static bool print_original_backbuffer(
        IDirectDrawSurface7* source,HWND window,std::string& error);
};
}
#endif
