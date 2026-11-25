#include "btb/win32_original_directdraw.hpp"
#include "btb/original_display3.hpp"
#include "btb/win32_original_imports3.hpp"
#include <functional>
#include "btb/bitmap_registry.hpp"
#include "btb/full_game_blt_clip.hpp"
#include "btb/original_recompiled_bitmaps.hpp"
#include "btb/win32_recompiled_graphics.hpp"

#if defined(_WIN32)
#include <cstdint>
#include <limits>
#include <string>
#include <utility>

namespace btb::full_game {
namespace {

std::string hresult_description(const char* op,HRESULT hr) {
    return std::string(op)+" failed (HRESULT "+
           std::to_string(static_cast<std::uint32_t>(hr))+")";
}

void release_surface(IDirectDrawSurface7*& surface) noexcept {
    if (surface) {
        surface->Release();
        surface=nullptr;
    }
}

[[nodiscard]] display::PackedRgbFormat locked_format(const DDSURFACEDESC2& info) noexcept {
    if(!(info.ddpfPixelFormat.dwFlags&DDPF_RGB)||info.ddpfPixelFormat.dwRGBBitCount%8)return {};
    return {info.ddpfPixelFormat.dwRGBBitCount/8,
        info.ddpfPixelFormat.dwRBitMask,info.ddpfPixelFormat.dwGBitMask,
        info.ddpfPixelFormat.dwBBitMask};
}
[[nodiscard]] bool read_surface_pixels(IDirectDrawSurface7* surface,int width,int height,
    display::PackedFrameSnapshot& output,std::string& error) {
    if(!surface||width<=0||height<=0||width>640||height>480){error="invalid snapshot surface";return false;}
    DDSURFACEDESC2 info{};info.dwSize=sizeof(info);
    const auto hr=surface->Lock(nullptr,&info,DDLOCK_WAIT|DDLOCK_READONLY|DDLOCK_SURFACEMEMORYPTR,nullptr);
    if(FAILED(hr)){error=hresult_description("lock original snapshot",hr);return false;}
    bool valid=false;
    // Always unlock, even if allocating the CPU snapshot throws.
    try{
        if(info.lpSurface&&info.lPitch>0&&info.dwWidth>=static_cast<DWORD>(width)&&
           info.dwHeight>=static_cast<DWORD>(height)&&
           static_cast<std::size_t>(info.lPitch)<=std::numeric_limits<std::size_t>::max()/height){
            const auto pitch=static_cast<std::size_t>(info.lPitch);
            valid=display::capture_packed_frame(output,
                {static_cast<const std::uint8_t*>(info.lpSurface),pitch*height},
                pitch,width,height,locked_format(info));
        }
    }catch(...){valid=false;}
    const auto unlock=surface->Unlock(nullptr);
    if(FAILED(unlock)){error=hresult_description("unlock original snapshot",unlock);return false;}
    if(!valid){error="unsupported original snapshot pixel layout";return false;}
    error.clear();return true;
}
[[nodiscard]] bool write_surface_pixels(IDirectDrawSurface7* surface,
    const display::PackedFrameSnapshot& input,int x,int y,std::string& error) {
    if(!surface||!input.valid()||x<0||y<0||x>640||y>480){error="invalid snapshot destination";return false;}
    DDSURFACEDESC2 info{};info.dwSize=sizeof(info);
    const auto hr=surface->Lock(nullptr,&info,DDLOCK_WAIT|DDLOCK_SURFACEMEMORYPTR,nullptr);
    if(FAILED(hr)){error=hresult_description("lock original snapshot destination",hr);return false;}
    bool valid=false;
    const auto format=locked_format(info);
    if(info.lpSurface&&info.lPitch>0&&format.valid()&&
       static_cast<std::size_t>(x)+input.width<=info.dwWidth&&
       static_cast<std::size_t>(y)+input.height<=info.dwHeight){
        const auto pitch=static_cast<std::size_t>(info.lPitch);
        if(info.dwHeight&&pitch<=std::numeric_limits<std::size_t>::max()/info.dwHeight&&
           (static_cast<std::size_t>(x)+input.width)*format.bytes<=pitch){
            const auto offset=static_cast<std::size_t>(y)*pitch+static_cast<std::size_t>(x)*format.bytes;
            valid=display::restore_packed_frame(input,
                {static_cast<std::uint8_t*>(info.lpSurface)+offset,pitch*info.dwHeight-offset},
                pitch,format);
        }
    }
    const auto unlock=surface->Unlock(nullptr);
    if(FAILED(unlock)){error=hresult_description("unlock original snapshot destination",unlock);return false;}
    if(!valid){error="unsupported original snapshot destination pixel layout";return false;}
    error.clear();return true;
}

class DisplayProvider final:public original_display3::Services {
public:
    using Handle=original_display3::Handle;using U32=original_display3::U32;using I32=original_display3::I32;using Guid=original_display3::Guid;
    explicit DisplayProvider(std::function<void()> reset):reset_(std::move(reset)){}
    I32 direct_draw(const Guid* a,Handle& out,const Guid& id,Handle outer) override{return imports_.direct_draw(a,out,id,outer);}
    I32 direct_input(Handle h,U32 v,const Guid& id,Handle& out,Handle outer) override{return imports_.direct_input(h,v,id,out,outer);}
    I32 direct_sound(const Guid* id,Handle& out,Handle outer) override{return imports_.direct_sound(id,out,outer);}
    void reset(original_display3::State& state) override{reset_();state={};}
    I32 cooperative(Handle draw,Handle window,U32 flags) override{return reinterpret_cast<IDirectDraw7*>(draw)->SetCooperativeLevel(reinterpret_cast<HWND>(window),flags);}
    I32 display_mode(Handle draw,U32 w,U32 h,U32 bits,U32 refresh,U32 flags) override{return reinterpret_cast<IDirectDraw7*>(draw)->SetDisplayMode(w,h,bits,refresh,flags);}
    I32 surface(Handle draw,const original_display3::Descriptor& in,Handle& result) override{
        DDSURFACEDESC2 desc{};desc.dwSize=sizeof(desc);desc.dwFlags=in.flags;desc.dwHeight=in.height;desc.dwWidth=in.width;desc.dwBackBufferCount=in.backbuffers;desc.ddsCaps.dwCaps=in.caps;
        auto* out=reinterpret_cast<IDirectDrawSurface7*>(result);const auto hr=reinterpret_cast<IDirectDraw7*>(draw)->CreateSurface(&desc,&out,nullptr);result=reinterpret_cast<Handle>(out);return hr;
    }
    I32 attached(Handle surface,const std::array<U32,4>& in,Handle& result) override{
        DDSCAPS2 caps{};caps.dwCaps=in[0];caps.dwCaps2=in[1];caps.dwCaps3=in[2];caps.dwCaps4=in[3];auto* out=reinterpret_cast<IDirectDrawSurface7*>(result);
        const auto hr=reinterpret_cast<IDirectDrawSurface7*>(surface)->GetAttachedSurface(&caps,&out);result=reinterpret_cast<Handle>(out);return hr;
    }
    void add_ref(Handle object) override{reinterpret_cast<IUnknown*>(object)->AddRef();}
    I32 create_clipper(Handle draw,U32 flags,Handle& result,Handle outer) override{
        auto* out=reinterpret_cast<IDirectDrawClipper*>(result);const auto hr=reinterpret_cast<IDirectDraw7*>(draw)->CreateClipper(flags,&out,reinterpret_cast<IUnknown*>(outer));result=reinterpret_cast<Handle>(out);return hr;
    }
    I32 clipper_window(Handle clip,U32 flags,Handle window) override{return reinterpret_cast<IDirectDrawClipper*>(clip)->SetHWnd(flags,reinterpret_cast<HWND>(window));}
    I32 set_clipper(Handle surface,Handle clip) override{return reinterpret_cast<IDirectDrawSurface7*>(surface)->SetClipper(reinterpret_cast<IDirectDrawClipper*>(clip));}
    void release(Handle object) override{reinterpret_cast<IUnknown*>(object)->Release();}
    U32 window_long(Handle h,I32 index) override{return static_cast<U32>(GetWindowLongA(reinterpret_cast<HWND>(h),index));}
    void window_long(Handle h,I32 index,U32 value) override{SetWindowLongA(reinterpret_cast<HWND>(h),index,static_cast<LONG>(value));}
    Handle menu(Handle h) override{return reinterpret_cast<Handle>(GetMenu(reinterpret_cast<HWND>(h)));}
    void adjust_window(original_display3::Rect& r,U32 style,bool has_menu,U32 ex) override{
        RECT native{r.left,r.top,r.right,r.bottom};AdjustWindowRectEx(&native,style,has_menu,ex);r={native.left,native.top,native.right,native.bottom};
    }
    void position(Handle h,Handle after,I32 x,I32 y,I32 w,I32 height,U32 flags) override{SetWindowPos(reinterpret_cast<HWND>(h),reinterpret_cast<HWND>(after),x,y,w,height,flags);}
    original_display3::Rect work_area() override{RECT r{};SystemParametersInfoA(SPI_GETWORKAREA,0,&r,0);return {r.left,r.top,r.right,r.bottom};}
    original_display3::Rect window_rectangle(Handle h) override{RECT r{};GetWindowRect(reinterpret_cast<HWND>(h),&r);return {r.left,r.top,r.right,r.bottom};}
    void update_destination(original_display3::State& state) override{RECT r{};(void)original_win32::update_display_destination_rect(reinterpret_cast<HWND>(state.window),state.windowed!=0,r);state.destination={r.left,r.top,r.right,r.bottom};}
private:
    original_win32::NativeImports3 imports_;std::function<void()> reset_;
};

} // namespace

Win32OriginalDirectDraw::~Win32OriginalDirectDraw() {
    shutdown();
}

void Win32OriginalDirectDraw::release_cached_bitmaps() noexcept {
    for (auto& [_,entry] : images_) {
        (void)registry_->unregister_surface_reference(&entry.surface);
        release_surface(entry.surface);
    }
    images_.clear();
}

void Win32OriginalDirectDraw::shutdown() noexcept {
    release_cached_bitmaps();
    replay_underlay_.clear();help_underlay_.clear();
    global_underlay_.clear();modal_underlay_.clear();composed_frame_.clear();
    if(extra_back_reference_ && backbuffer_)backbuffer_->Release();
    extra_back_reference_=false;
    release_surface(backbuffer_);
    release_surface(primary_);
    if (clipper_) {
        clipper_->Release();
        clipper_=nullptr;
    }
    if (draw_) {
        if (mode_ == display::DisplayMode::Fullscreen) {
            draw_->RestoreDisplayMode();
            draw_->SetCooperativeLevel(hwnd_,DDSCL_NORMAL);
        }
        draw_->Release();
        draw_=nullptr;
    }
    hwnd_=nullptr;
    surface_lost_during_draw_=false;
}

bool Win32OriginalDirectDraw::initialize(
    HWND hwnd,display::DisplayMode mode,std::string& error) {

    if (!hwnd || !IsWindow(hwnd)) {
        shutdown();error="DirectDraw7 requires a valid original-game HWND";return false;
    }
    original_display3::State state;
    DisplayProvider provider([this]{shutdown();});
    const auto window=reinterpret_cast<original_display3::Handle>(hwnd);
    const auto hr=mode==display::DisplayMode::Fullscreen
        ? original_display3::fullscreen(state,window,640,480,16,provider)
        : original_display3::windowed(state,window,640,480,provider);
    // Adopt partial results as well, so native failure cleanup owns every COM ref.
    draw_=reinterpret_cast<IDirectDraw7*>(state.draw);
    primary_=reinterpret_cast<IDirectDrawSurface7*>(state.primary);
    backbuffer_=reinterpret_cast<IDirectDrawSurface7*>(state.back);
    extra_back_reference_=state.extra_back_reference;hwnd_=hwnd;mode_=mode;
    if(hr<0){error=hresult_description("original display initialization",hr);shutdown();return false;}

    error.clear();
    return true;
}

bool Win32OriginalDirectDraw::recreate(HWND hwnd,display::DisplayMode mode,std::string& error) {
    // initialize's original reset callback releases all COM resources.
    // Move only authentic captured pixels out of that teardown scope.
    auto replay=std::move(replay_underlay_),help=std::move(help_underlay_),
        global=std::move(global_underlay_),modal=std::move(modal_underlay_),
        composed=std::move(composed_frame_);
    shutdown();
    if(!initialize(hwnd,mode,error))return false;
    replay_underlay_=std::move(replay);help_underlay_=std::move(help);
    global_underlay_=std::move(global);modal_underlay_=std::move(modal);
    composed_frame_=std::move(composed);
    return true;
}

bool Win32OriginalDirectDraw::restore_last_composed_frame(std::string& error) {
    if(!composed_frame_.valid()){error.clear();return true;}
    return write_surface_pixels(backbuffer_,composed_frame_,0,0,error);
}

HRESULT Win32OriginalDirectDraw::clear_retail_backbuffer(DWORD color) noexcept {
    return original_win32::clear_backbuffer(backbuffer_,color);
}

bool Win32OriginalDirectDraw::load_bitmap(
    const std::filesystem::path& filename,
    OriginalSurface& out,std::string& error) {

    // Actual recovered retail function call chain:
    // 004038D0 LoadBitmapToDirectDrawSurface
    //      -> 004035F0 CopyBitmapToSurface
    //      -> DirectDraw7::GetDC / native GDI BitBlt / ReleaseDC.
    original_win32::LoadedBitmap loaded{};
    const auto hr=original_win32::load_bitmap_to_directdraw_surface(
        draw_,filename,loaded);
    if(FAILED(hr)){
        error=hresult_description("004038D0 retail bitmap load",hr);
        return false;
    }

    // Registration and keyed-flag changes occur after emplacement, so
    // 00403770 receives the stable address of the cached surface pointer.
    out={loaded.surface,loaded.width,loaded.height,false};
    error.clear();
    return true;
}

bool Win32OriginalDirectDraw::blit(
    const ResolvedDraw& requested,std::string& error) {

    if (!requested.source.found()) {
        error="cannot blit an unresolved original bitmap";
        return false;
    }
    const auto key=requested.source.absolute_path.wstring();
    auto it=images_.find(key);
    if (it == images_.end()) {
        OriginalSurface loaded{};
        if (!load_bitmap(requested.source.absolute_path,loaded,error)) {
            return false;
        }
        it=images_.emplace(key,loaded).first;
        // 00403770 registers the surface address and source filename first.
        // The original keyed flag is independent: opaque backgrounds must
        // not become transparent merely because they were loaded first.
        const auto source_name=requested.source.absolute_path.string();
        if(!registry_->register_surface_reference(
                &it->second.surface,source_name)){
            release_surface(it->second.surface);
            images_.erase(it);
            error="original 800-slot bitmap table cannot register source";
            return false;
        }
    }
    if(requested.original.color_keyed&&!it->second.color_keyed) {
        // 004036E0 marks only a genuinely keyed bitmap; 00403B70 performs
        // the native GDI/DirectDraw conversion and SetColorKey call.
        if(!registry_->mark_registered_surface_color_keyed(
                &it->second.surface)) {
            (void)registry_->unregister_surface_reference(&it->second.surface);
            release_surface(it->second.surface);
            images_.erase(it);
            error="original keyed bitmap could not mark its registered slot";
            return false;
        }
        const auto keyed=original_win32::set_surface_transparency_color_key(
            it->second.surface,RGB(255,0,255));
        if(FAILED(keyed)){
            (void)registry_->unregister_surface_reference(
                &it->second.surface);
            release_surface(it->second.surface);
            images_.erase(it);
            error=hresult_description("00403B70 native surface key",keyed);
            return false;
        }
        it->second.color_keyed=true;
    }

    RECT src{0,0,it->second.width,it->second.height};
    if (requested.original.source_rectangle) {
        const auto& r=*requested.original.source_rectangle;
        src={r.left,r.top,r.right,r.bottom};
    }
    if (src.left<0 || src.top<0 ||
        src.right>it->second.width || src.bottom>it->second.height ||
        src.right<=src.left || src.bottom<=src.top) {
        error="retail sprite source rectangle extends beyond original BMP: "+
            requested.original.source_asset+" ["+std::to_string(src.left)+","+
            std::to_string(src.top)+","+std::to_string(src.right)+","+
            std::to_string(src.bottom)+"] bitmap "+
            std::to_string(it->second.width)+"x"+std::to_string(it->second.height);
        return false;
    }
    const auto clipped=retail_clip_blt_to_rect(
        Rect{src.left,src.top,src.right,src.bottom},
        requested.original.x,requested.original.y,
        requested.original.destination_clip.value_or(
            Rect{0,0,display::kRetailWidth,display::kRetailHeight}));
    if (!clipped.visible) {
        // Fully offscreen is a valid original animation frame; it is
        // not a lost bitmap and must not abort the entire game tick.
        return true;
    }
    src={
        clipped.source.left,clipped.source.top,
        clipped.source.right,clipped.source.bottom
    };
    RECT dest{
        clipped.destination.left,clipped.destination.top,
        clipped.destination.right,clipped.destination.bottom
    };

    // 00415E50 is the keyed path; 00403500 is a direct BltFast
    // COM dispatch for unkeyed, preclipped original sprite surfaces.
    const HRESULT hr=requested.original.color_keyed
        ? original_win32::blit_color_keyed_surface_clipped(
            backbuffer_,it->second.surface,dest.left,dest.top,
            &src,0,0)
        : original_win32::blt_fast_to_backbuffer(
            backbuffer_,static_cast<DWORD>(dest.left),
            static_cast<DWORD>(dest.top),it->second.surface,
            &src,DDBLTFAST_WAIT);
    if (hr==DDERR_SURFACELOST) {
        surface_lost_during_draw_=true;
        error="DirectDraw surface lost during original sprite blit";
        return false;
    }
    if (FAILED(hr)) {
        error=hresult_description("retail sprite Blt",hr)+": "+requested.original.source_asset;
        return false;
    }
    return true;
}

bool Win32OriginalDirectDraw::restore_lost_surfaces(
    std::string& error) {

    auto hr=primary_->Restore();
    if (SUCCEEDED(hr)) hr=backbuffer_->Restore();
    if (SUCCEEDED(hr)) hr=original_win32::restore_all_directdraw_surfaces(draw_);
    if (FAILED(hr)) {
        error=hresult_description("retail Restore surfaces",hr);
        return false;
    }
    // PE32 00403830 reloads live original surface-pointer variables
    // from filenames and restores keyed surfaces after a lost device.
    if(!registry_->reload_registered_surfaces(draw_)){
        release_cached_bitmaps();
        error="00403830 failed to reload original registered bitmap surfaces";
        return false;
    }
    error.clear();
    return true;
}

bool Win32OriginalDirectDraw::render_additional(
    const std::vector<ResolvedDraw>& stream,std::string& error) {
    if(!initialized()){error="post-movie overlay requires live DirectDraw";return false;}
    for(const auto& draw:stream)if(!blit(draw,error))return false;
    error.clear();return true;
}

bool Win32OriginalDirectDraw::capture_surface(display::PackedFrameSnapshot& target,
    std::string& error,IDirectDrawSurface7* source) {
    if(!initialized()){error="snapshot requires live DirectDraw";return false;}
    return read_surface_pixels(source?source:backbuffer_,640,480,target,error);
}

bool Win32OriginalDirectDraw::capture_replay_underlay(std::string& error) {
    return capture_surface(replay_underlay_,error);
}
display::PackedFrameSnapshot& Win32OriginalDirectDraw::underlay_target(CapturedUnderlay kind) noexcept {
    if(kind==CapturedUnderlay::Help)return help_underlay_;
    if(kind==CapturedUnderlay::Modal)return modal_underlay_;
    return global_underlay_;
}
bool Win32OriginalDirectDraw::capture_presented_underlay(CapturedUnderlay kind,std::string& error) {
    // Windowed Blt keeps the 640x480 render buffer; fullscreen Flip
    // swaps it with the front surface. Never capture the stale back
    // buffer after a fullscreen flip or the desktop in windowed mode.
    // The retained composed frame is independent of the fullscreen
    // front surface's lockability and remains valid after a device reset.
    if(composed_frame_.valid()){
        underlay_target(kind)=composed_frame_;
        error.clear();return true;
    }
    return capture_surface(underlay_target(kind),error);
}
bool Win32OriginalDirectDraw::capture_underlay(CapturedUnderlay kind,std::string& error) {
    return capture_surface(underlay_target(kind),error);
}
bool Win32OriginalDirectDraw::render_captured_underlay(CapturedUnderlay kind,
    const std::vector<ResolvedDraw>& stream,std::string& error) {
    return render_ordered_with_underlay(stream,error,
        kind==CapturedUnderlay::Help?UnderlayKind::Help:
        kind==CapturedUnderlay::Modal?UnderlayKind::Modal:UnderlayKind::GlobalMovie);
}

bool Win32OriginalDirectDraw::render_ordered(
    const std::vector<ResolvedDraw>& stream,std::string& error) {
    return render_ordered_with_underlay(stream,error,UnderlayKind::None);
}

bool Win32OriginalDirectDraw::render_replay_ordered(
    const std::vector<ResolvedDraw>& stream,std::string& error) {
    return render_ordered_with_underlay(stream,error,UnderlayKind::Replay);
}

bool Win32OriginalDirectDraw::render_ordered_with_underlay(
    const std::vector<ResolvedDraw>& stream,std::string& error,UnderlayKind kind) {

    if (!initialized()) {
        error="DirectDraw renderer has not created original primary/backbuffer";
        return false;
    }
    const bool replay=kind!=UnderlayKind::None;
    const auto* underlay=kind==UnderlayKind::Replay?&replay_underlay_:
        kind==UnderlayKind::Help?&help_underlay_:
        kind==UnderlayKind::GlobalMovie?&global_underlay_:
        kind==UnderlayKind::Modal?&modal_underlay_:nullptr;
    if(replay&&(!underlay||!underlay->valid())){
        error="original captured pixels unavailable";return false;
    }
    // Retry the ENTIRE ordered frame after losing a surface. A partial
    // retry would omit earlier blits whose pixels were lost on Restore.
    for (int attempt=0;attempt<2;++attempt) {
        surface_lost_during_draw_=false;
        // 00403540 issues the original native DDBLT_COLORFILL.
        HRESULT hr=DD_OK;
        if(replay){
            if(!write_surface_pixels(backbuffer_,*underlay,0,0,error)){
                if(attempt==0&&backbuffer_->IsLost()==DDERR_SURFACELOST&&restore_lost_surfaces(error))continue;
                return false;
            }
        }else hr=original_win32::clear_backbuffer(backbuffer_,0);
        // The original clear omits DDBLT_WAIT; the saved replay
        // snapshot instead uses the original blocking bitmap blit.
        for(int spin=0;!replay&&spin<32&&hr==DDERR_WASSTILLDRAWING;++spin)
            hr=original_win32::clear_backbuffer(backbuffer_,0);
        if (hr==DDERR_SURFACELOST) {
            surface_lost_during_draw_=true;
            error="DirectDraw backbuffer lost during clear";
        } else if (FAILED(hr)) {
            error=hresult_description("original frame color-fill",hr);
            return false;
        } else {
            bool valid=true;
            for (const auto& draw : stream) {
                if (!blit(draw,error)) {
                    valid=false;
                    break;
                }
            }
            if (valid) {
                error.clear();
                return true;
            }
        }
        if (!surface_lost_during_draw_ ||
            !restore_lost_surfaces(error)) {
            return false;
        }
    }
    error="DirectDraw surface was lost twice during one native frame";
    return false;
}

bool Win32OriginalDirectDraw::create_bink_offscreen_surface(
    int width,int height,IDirectDrawSurface7*& output,
    std::string& error) {
    output=nullptr;
    if(!initialized()||width<1||height<1||
       width>640||height>480) {
        error="cannot allocate original Bink movie surface without live DD7";
        return false;
    }
    DDSURFACEDESC2 desc{};
    desc.dwSize=sizeof(desc);
    desc.dwFlags=DDSD_CAPS|DDSD_WIDTH|DDSD_HEIGHT;
    desc.ddsCaps.dwCaps=DDSCAPS_OFFSCREENPLAIN|DDSCAPS_SYSTEMMEMORY;
    desc.dwWidth=static_cast<DWORD>(width);
    desc.dwHeight=static_cast<DWORD>(height);
    const auto hr=draw_->CreateSurface(&desc,&output,nullptr);
    if(FAILED(hr)||!output) {
        output=nullptr;
        error=hresult_description("original DD7 Bink offscreen",hr);
        return false;
    }
    error.clear();
    return true;
}

bool Win32OriginalDirectDraw::blit_bink_offscreen_surface(
    IDirectDrawSurface7* source,int width,int height,
    int x,int y,std::string& error) {
    if(!initialized()||!source||
       width<1||height<1||x<0||y<0||
       x+width>display::kRetailWidth||
       y+height>display::kRetailHeight) {
        error="original Bink source surface or viewport invalid";
        return false;
    }
    RECT rect{0,0,width,height};
    auto hr=backbuffer_->BltFast(
        static_cast<DWORD>(x),static_cast<DWORD>(y),
        source,&rect,DDBLTFAST_WAIT);
    if(FAILED(hr)) {
        // Existing movie banks own system-memory surfaces on the previous
        // DD7 object after display recreation. Cross-device/format BltFast
        // may fail. Transfer their real pixels with checked locks instead.
        display::PackedFrameSnapshot pixels;
        if(source->IsLost()!=DD_OK){
            error="original Bink offscreen pixels lost";return false;
        }
        if(backbuffer_->IsLost()==DDERR_SURFACELOST){
            surface_lost_during_draw_=true;
            // Restoring here would lose all preceding bitmap layers.
            error="original Bink destination lost during composition";return false;
        }
        if(!read_surface_pixels(source,width,height,pixels,error))return false;
        if(!write_surface_pixels(backbuffer_,pixels,x,y,error))return false;
    }
    error.clear();return true;
}

HRESULT Win32OriginalDirectDraw::cooperative_status() const noexcept {
    return draw_?draw_->TestCooperativeLevel():E_POINTER;
}

bool Win32OriginalDirectDraw::restore_for_frame(std::string& error) {
    return initialized() && restore_lost_surfaces(error);
}

bool Win32OriginalDirectDraw::present(std::string& error) {
    if (!initialized()) {
        error="cannot present before original display initialization";
        return false;
    }

    // Execute the original PE32 00402600 -> 00403480 presentation pair,
    // not the formerly hand-authored Flip/Blt approximation.
    const auto try_present=[&](bool& bitmap_reload) -> HRESULT {
        RECT destination{};
        const bool windowed=mode_==display::DisplayMode::Windowed;
        if(windowed && !original_win32::update_display_destination_rect(
                           hwnd_,true,destination))
            return E_FAIL;
        return original_win32::present_game_frame_retail(
            primary_,backbuffer_,draw_,windowed?&destination:nullptr,
            windowed,bitmap_reload);
    };

    // Preserve the COMPLETE composed backbuffer, including independently
    // decoded activity/walkthrough movies, prior to Flip/Blt. Replaying
    // only the bitmap list loses those overlays after device recovery.
    if(!capture_surface(composed_frame_,error))return false;
    bool bitmap_reload=false;
    auto hr=try_present(bitmap_reload);
    if (hr==DDERR_SURFACELOST && bitmap_reload) {
        // 00403480 already Restored both COM surfaces and the DD device.
        // Execute the companion 00403830 bitmap-registry reload (retail's
        // DAT_0051c320 flag), then replay the actual cached source frame.
        if(!registry_->reload_registered_surfaces(draw_)){
            release_cached_bitmaps();
            error="00403480/00403830 failed to reload original bitmaps";
            return false;
        }
        if(!restore_last_composed_frame(error))return false;
        bool lost_again=false;
        hr=try_present(lost_again);
        if(lost_again){
            error="00403480 reported a second lost surface during present";
            return false;
        }
    }
    if (FAILED(hr)) {
        error=hresult_description("DirectDraw7 Blt/Flip present",hr);
        return false;
    }
    error.clear();
    return true;
}

} // namespace btb::full_game
#endif
