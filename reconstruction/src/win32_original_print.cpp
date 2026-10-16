#include "btb/win32_original_directdraw.hpp"
#if defined(_WIN32)
#include "btb/printing_system.hpp"
#include <commdlg.h>
#include <fstream>

namespace btb::full_game {
bool Win32OriginalDirectDraw::print_frame(
    const std::filesystem::path& filename,std::string& error) {
    if (!backbuffer_) { error="original print backbuffer unavailable";return false; }
    struct Resources {
        HDC memory{};
        HBITMAP bitmap{};
        HGDIOBJ previous{};
        PRINTDLGW dialog{};
        ~Resources() {
            if (previous) SelectObject(memory,previous);
            if (bitmap) DeleteObject(bitmap);
            if (memory) DeleteDC(memory);
            if (dialog.hDC) DeleteDC(dialog.hDC);
            if (dialog.hDevMode) GlobalFree(dialog.hDevMode);
            if (dialog.hDevNames) GlobalFree(dialog.hDevNames);
        }
    } resources;
    BITMAPINFO info{};
    auto& header=info.bmiHeader;
    header.biSize=sizeof(header);header.biWidth=640;header.biHeight=480;
    header.biPlanes=1;header.biBitCount=24;header.biCompression=BI_RGB;
    header.biSizeImage=640*3*480;
    void* pixels=nullptr;
    resources.memory=CreateCompatibleDC(nullptr);
    if (resources.memory)
        resources.bitmap=CreateDIBSection(resources.memory,&info,DIB_RGB_COLORS,&pixels,nullptr,0);
    if (!resources.bitmap || !pixels) { error="cannot allocate original print bitmap";return false; }
    resources.previous=SelectObject(resources.memory,resources.bitmap);
    HDC source=nullptr;
    const auto hr=backbuffer_->GetDC(&source);
    if (FAILED(hr)) { error="cannot capture original DirectDraw print surface";return false; }
    const BOOL copied=BitBlt(resources.memory,0,0,640,480,source,0,0,SRCCOPY);
    backbuffer_->ReleaseDC(source);
    if (!copied) { error="cannot copy original frame to print bitmap";return false; }
    GdiFlush();
    // 00409460 exports the current render frame as a bottom-up 24-bit BMP.
    BITMAPFILEHEADER file{};
    file.bfType=0x4d42;file.bfOffBits=sizeof(file)+sizeof(header);
    file.bfSize=file.bfOffBits+header.biSizeImage;
    std::ofstream out(filename,std::ios::binary|std::ios::trunc);
    out.write(reinterpret_cast<const char*>(&file),sizeof(file));
    out.write(reinterpret_cast<const char*>(&header),sizeof(header));
    out.write(static_cast<const char*>(pixels),header.biSizeImage);
    out.close();
    if (!out) { error="cannot write original PrintMe.bmp";return false; }

    auto& dialog=resources.dialog;
    dialog.lStructSize=sizeof(dialog);dialog.hwndOwner=hwnd_;
    dialog.Flags=PD_RETURNDC|PD_NOSELECTION|PD_NOPAGENUMS;
    dialog.nMinPage=dialog.nMaxPage=dialog.nCopies=1;
    if (!PrintDlgW(&dialog)) {
        const auto failure=CommDlgExtendedError();
        if (failure) { error="original print dialog failed: "+std::to_string(failure);return false; }
        error.clear();return true; // Cancel is an ordinary source UI action.
    }
    const auto dc=dialog.hDC;
    HDC window_dc=GetDC(hwnd_);
    if (!window_dc) { error="cannot obtain original screen print metrics";return false; }
    const printing::ScreenMetrics screen{GetDeviceCaps(window_dc,LOGPIXELSX),
        GetDeviceCaps(window_dc,LOGPIXELSY)};
    ReleaseDC(hwnd_,window_dc);
    const auto target=printing::compute_target_rect(printing::ScaleMode::RetailScreenScaled,
        {640,480},{GetDeviceCaps(dc,HORZRES),GetDeviceCaps(dc,VERTRES),
                   GetDeviceCaps(dc,LOGPIXELSX),GetDeviceCaps(dc,LOGPIXELSY)},screen);
    const auto& r=target.rect;
    if (!target.changed || r.right<=r.left || r.bottom<=r.top ||
        (GetDeviceCaps(dc,RASTERCAPS)&0x100)) {
        error="unsupported original printer geometry or palette";return false;
    }
    DOCINFOW document{};document.cbSize=sizeof(document);
    document.lpszDocName=L"Bob the Builder - Bob Builds a Park";
    if (StartDocW(dc,&document)<=0) { error="cannot start original print document";return false; }
    if (StartPage(dc)<=0) { EndDoc(dc);error="cannot start original print page";return false; }
    SetStretchBltMode(dc,printing::kStretchMode);
    const int drawn=StretchDIBits(dc,r.left,r.top,r.right-r.left,r.bottom-r.top,
        0,0,640,480,pixels,&info,DIB_RGB_COLORS,SRCCOPY);
    const int page=EndPage(dc),ended=EndDoc(dc);
    if (drawn==GDI_ERROR || page<=0 || ended<=0) {
        error="original certificate print failed";return false;
    }
    error.clear();return true;
}
} // namespace btb::full_game
#endif
