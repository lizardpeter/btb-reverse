#include "native_cursor.hpp"
#if defined(_WIN32)
#include <cstring>
namespace btb::enhanced {
void NativeCursor::reset()noexcept{
    if(handle_){
        if(GetCursor()==handle_)SetCursor(nullptr);
        DestroyCursor(handle_);
    }
    handle_=nullptr;width_=height_=0;revision_=0;attempted_=false;
}
bool NativeCursor::ensure(const CursorPixels& pixels,unsigned w,unsigned h,std::uint64_t revision){
    if(attempted_&&w==width_&&h==height_&&revision==revision_)return handle_!=nullptr;
    reset();width_=w;height_=h;revision_=revision;attempted_=true;
    CursorBitmap bitmap;
    if(!make_cursor_bitmap(pixels,w,h,bitmap))return false;
    BITMAPINFO info{};info.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
    info.bmiHeader.biWidth=static_cast<LONG>(w);info.bmiHeader.biHeight=-static_cast<LONG>(h);
    info.bmiHeader.biPlanes=1;info.bmiHeader.biBitCount=32;info.bmiHeader.biCompression=BI_RGB;
    void* data=nullptr;
    HBITMAP color=CreateDIBSection(nullptr,&info,DIB_RGB_COLORS,&data,nullptr,0);
    if(!color||!data){if(color)DeleteObject(color);return false;}
    std::memcpy(data,bitmap.bgra.data(),bitmap.bgra.size()*4);
    HBITMAP mask=CreateBitmap(static_cast<int>(w),static_cast<int>(h),1,1,bitmap.mask.data());
    if(!mask){DeleteObject(color);return false;}
    ICONINFO icon{};icon.fIcon=FALSE;icon.xHotspot=0;icon.yHotspot=0;
    icon.hbmMask=mask;icon.hbmColor=color;
    handle_=reinterpret_cast<HCURSOR>(CreateIconIndirect(&icon));
    DeleteObject(mask);DeleteObject(color);
    return handle_!=nullptr;
}
}
#endif
