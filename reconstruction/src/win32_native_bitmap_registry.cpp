#include "btb/win32_native_bitmap_registry.hpp"
#if defined(_WIN32)
#include "btb/original_recompiled_bitmaps.hpp"
#include "btb/win32_recompiled_graphics.hpp"
#include <algorithm>
#include <cstring>
#include <filesystem>

namespace btb::original_win32 {

// PE32 00403770: first empty slot below high water, otherwise its
// successor; store surface *pointer address* and 260-byte filename;
// rescan the original 800 slots for the highest live index.
bool NativeBitmapRegistry::register_surface_reference(
    SurfaceReference location,std::string_view filename) noexcept {
    if (reloading_ || !location || filename.empty()) return false;
    const auto end=filename.find('\0');
    if(end!=std::string_view::npos)filename=filename.substr(0,end);
    if(filename.size()>=kFilenameBytes || filename.empty())return false;
    std::size_t index=0;
    while(index<high_water_ && locations_[index]) ++index;
    // Retail can overflow at 800. Deliberately stop before out-of-bounds.
    if(index==kSlots)return false;
    locations_[index]=location;
    keyed_[index]=0;
    filenames_[index].fill('\0');
    std::memcpy(filenames_[index].data(),filename.data(),filename.size());
    high_water_=0;
    for(std::size_t i=0;i<kSlots;++i)
        if(locations_[i])high_water_=i+1;
    return true;
}

// PE32 004036E0: mark first table match, without modifying the surface.
bool NativeBitmapRegistry::mark_registered_surface_color_keyed(
    SurfaceReference location) noexcept {
    if(!location)return false;
    for(std::size_t i=0;i<high_water_;++i) {
        if(locations_[i]==location){
            keyed_[i]=1;
            return true;
        }
    }
    return false;
}

// PE32 00403720: clear pointer, key and first filename byte without
// decrementing the high-water count. The next registration reuses holes.
bool NativeBitmapRegistry::unregister_surface_reference(
    SurfaceReference location) noexcept {
    if(!location)return false;
    for(std::size_t i=0;i<high_water_;++i){
        if(locations_[i]==location){
            locations_[i]=nullptr;
            keyed_[i]=0;
            filenames_[i][0]='\0';
            return true;
        }
    }
    return false;
}

// PE32 00403830: retain the registered address, Release the old COM
// surface, clear its pointer, recreate from its recorded source filename,
// and reapply a magenta key only when the original flagged that slot.
// Entries being reloaded never register again through 00403770.
bool NativeBitmapRegistry::reload_registered_surfaces(
    IDirectDraw7* directdraw) noexcept {
    if(!directdraw || reloading_)return false;
    reloading_=true;
    bool complete=true;
    for(std::size_t i=0;i<high_water_;++i){
        SurfaceReference location=locations_[i];
        if(!location || !*location)continue;
        (*location)->Release();
        *location=nullptr;
        LoadedBitmap loaded{};
        const auto hr=load_bitmap_to_directdraw_surface(
            directdraw,std::filesystem::path(filenames_[i].data()),loaded);
        if(FAILED(hr) || !loaded.surface){
            complete=false;
            continue;
        }
        *location=loaded.surface;
        if(keyed_[i]){
            const auto status=set_surface_transparency_color_key(
                loaded.surface,RGB(255,0,255));
            if(FAILED(status)){
                loaded.surface->Release();
                *location=nullptr;
                complete=false;
            }
        }
    }
    reloading_=false;
    return complete;
}

} // namespace btb::original_win32
#endif
