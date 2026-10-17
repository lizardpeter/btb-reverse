#include "btb/win32_retail_print_provider.hpp"
#if defined(_WIN32)
#include <commdlg.h>
#include <algorithm>
#include <array>
#include <cstdint>
#include <cstring>
#include <bit>
#include <cstdlib>
#include <cstddef>
#include <limits>
#include <vector>

namespace btb::full_game {
namespace {
struct CapturedFrame {
    BITMAPINFO info{};
    std::vector<std::uint8_t> pixels{};
};
[[nodiscard]] std::uint8_t channel(
    std::uint32_t value,std::uint32_t mask) noexcept {
    if(!mask)return 0;
    const unsigned shift=std::countr_zero(mask);
    const std::uint32_t max=mask>>shift;
    if(max==0)return 0;
    return static_cast<std::uint8_t>(
        (((value&mask)>>shift)*255ULL + max/2)/max);
}

[[nodiscard]] bool capture_source(
    IDirectDrawSurface7* surface,CapturedFrame& result,std::string& error) {
    if(!surface) {
        error="original DD7 certificate backbuffer unavailable";
        return false;
    }
    DDSURFACEDESC2 desc{};
    desc.dwSize=sizeof(desc);
    const HRESULT status=surface->Lock(
        nullptr,&desc,DDLOCK_WAIT|DDLOCK_SURFACEMEMORYPTR,nullptr);
    if(FAILED(status)) {
        error="cannot lock original DD7 certificate pixels";
        return false;
    }
    // The retail game window/backbuffer is 640x480. Enforce the actual
    // DD7 bounds to avoid reading a surface with invalid dimensions/pitch.
    const auto width=desc.dwWidth,height=desc.dwHeight;
    const auto& format=desc.ddpfPixelFormat;
    const bool indexed=(format.dwFlags&DDPF_PALETTEINDEXED8)!=0;
    const bool rgb=(format.dwFlags&DDPF_RGB)!=0;
    const auto bpp=format.dwRGBBitCount;
    const bool supported=indexed?(bpp==8):
        (rgb&&(bpp==16||bpp==24||bpp==32));
    const unsigned bytes_per_pixel=(bpp+7)/8;
    bool valid=width>0&&height>0&&width<=4096&&height<=4096&&
        desc.lpSurface&&supported&&
        bytes_per_pixel>0&&
        static_cast<std::uint64_t>(std::abs(
            static_cast<long long>(desc.lPitch)))>=
                static_cast<std::uint64_t>(width)*bytes_per_pixel;
    std::array<PALETTEENTRY,256> palette{};
    if(valid&&indexed) {
        IDirectDrawPalette* entries=nullptr;
        if(FAILED(surface->GetPalette(&entries))||!entries)
            valid=false;
        else {
            valid=SUCCEEDED(entries->GetEntries(
                0,0,static_cast<DWORD>(palette.size()),palette.data()));
            entries->Release();
        }
    }
    if(valid) {
        result.info={};
        result.info.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
        result.info.bmiHeader.biWidth=static_cast<LONG>(width);
        // top-down BI_RGB 32 bpp allows exact source row order to be printed
        result.info.bmiHeader.biHeight=-static_cast<LONG>(height);
        result.info.bmiHeader.biPlanes=1;
        result.info.bmiHeader.biBitCount=32;
        result.info.bmiHeader.biCompression=BI_RGB;
        result.pixels.resize(
            static_cast<std::size_t>(width)*height*4);
        for(std::uint32_t y=0;y<height;++y) {
            const auto* row=static_cast<const std::uint8_t*>(
                desc.lpSurface)+static_cast<std::ptrdiff_t>(y)*desc.lPitch;
            auto* out=result.pixels.data()+
                static_cast<std::size_t>(y)*width*4;
            for(std::uint32_t x=0;x<width;++x) {
                const auto* pixel=row+x*bytes_per_pixel;
                std::uint32_t value=0;
                for(unsigned b=0;b<bytes_per_pixel;++b)
                    value|=static_cast<std::uint32_t>(pixel[b])<<(b*8);
                std::uint8_t red=0,green=0,blue=0;
                if(indexed) {
                    const auto& entry=palette[value&0xff];
                    red=entry.peRed;green=entry.peGreen;blue=entry.peBlue;
                } else {
                    red=channel(value,format.dwRBitMask);
                    green=channel(value,format.dwGBitMask);
                    blue=channel(value,format.dwBBitMask);
                }
                out[4*x]=blue;
                out[4*x+1]=green;
                out[4*x+2]=red;
                out[4*x+3]=0;
            }
        }
    }
    const HRESULT unlocked=surface->Unlock(nullptr);
    if(!valid || FAILED(unlocked)) {
        result.pixels.clear();
        error="original DD7 certificate format, palette, or pixel capture unsupported";
        return false;
    }
    error.clear();return true;
}
} // namespace

bool Win32RetailPrintProvider::print_original_backbuffer(
    IDirectDrawSurface7* source,HWND window,std::string& error) {
    CapturedFrame frame{};
    if(!capture_source(source,frame,error))return false;

    PRINTDLGA dlg{};
    dlg.lStructSize=sizeof(dlg);
    dlg.hwndOwner=window;
    dlg.Flags=PD_RETURNDC|PD_NOSELECTION|PD_NOPAGENUMS;
    dlg.nCopies=1;
    const BOOL selected=PrintDlgA(&dlg);
    const DWORD dialog_error=selected?0:CommDlgExtendedError();
    // Handles are allocated even on some cancel paths. Never leak either
    // the printer DC or the retained DEVMODE/DEVNAMES global buffers.
    const auto release=[&] {
        if(dlg.hDC)DeleteDC(dlg.hDC);
        if(dlg.hDevMode)GlobalFree(dlg.hDevMode);
        if(dlg.hDevNames)GlobalFree(dlg.hDevNames);
    };
    if(!selected) {
        release();
        if(dialog_error) {
            error="original Windows PrintDlg failed with error "+
                std::to_string(dialog_error);
            return false;
        }
        // User cancellation is not a printed page or a game completion.
        error.clear();return true;
    }
    if(!dlg.hDC) {
        release();error="native PrintDlg returned no printer DC";
        return false;
    }

    const int paper_w=GetDeviceCaps(dlg.hDC,HORZRES);
    const int paper_h=GetDeviceCaps(dlg.hDC,VERTRES);
    if(paper_w<=0||paper_h<=0) {
        release();error="printer reports no valid printable dimensions";
        return false;
    }
    const int w=frame.info.bmiHeader.biWidth;
    const int h=-frame.info.bmiHeader.biHeight;
    const double scale=std::min(
        static_cast<double>(paper_w)/w,
        static_cast<double>(paper_h)/h);
    const int draw_w=static_cast<int>(w*scale);
    const int draw_h=static_cast<int>(h*scale);
    if(draw_w<=0||draw_h<=0) {
        release();error="printer image scaling generated invalid dimensions";
        return false;
    }
    const int left=(paper_w-draw_w)/2;
    const int top=(paper_h-draw_h)/2;
    DOCINFOA doc{};
    doc.cbSize=sizeof(doc);
    doc.lpszDocName="Bob the Builder - Bob Builds a Park";
    if(StartDocA(dlg.hDC,&doc)<=0) {
        release();error="native printer rejected StartDoc";
        return false;
    }
    if(StartPage(dlg.hDC)<=0) {
        AbortDoc(dlg.hDC);release();
        error="native printer rejected StartPage";
        return false;
    }
    SetStretchBltMode(dlg.hDC,HALFTONE);
    const int written=StretchDIBits(
        dlg.hDC,left,top,draw_w,draw_h,
        0,0,w,h,frame.pixels.data(),&frame.info,
        DIB_RGB_COLORS,SRCCOPY);
    if(written==GDI_ERROR||written==0) {
        AbortDoc(dlg.hDC);release();
        error="original certificate bitmap could not be drawn to printer";
        return false;
    }
    if(EndPage(dlg.hDC)<=0) {
        AbortDoc(dlg.hDC);release();
        error="native printer rejected EndPage";
        return false;
    }
    if(EndDoc(dlg.hDC)<=0) {
        release();error="native printer rejected EndDoc";
        return false;
    }
    release();
    error.clear();return true;
}
}
#endif
