#include "btb/retail_system_closure.hpp"
#include <array>
#include <cassert>
#include <cstdint>
#include <vector>

using namespace btb::retail_system;
namespace {
std::vector<int> history;
int attempts=0;
U32 win(void*,I32 mode) noexcept {history.push_back(mode);return 1;}
U32 present_call(void*) noexcept {history.push_back(3);return 0x80004005U;}
void release(void*) noexcept {history.push_back(4);}
void destroy(void*) noexcept {history.push_back(5);}
void free_object(void*) noexcept {history.push_back(6);}
void unregister(std::size_t slot) noexcept {history.push_back(10+static_cast<int>(slot));}
void coop(void*,std::uintptr_t hwnd,U32 flags) noexcept {
    assert(hwnd==77&&flags==8);history.push_back(20);
}
void wave_close(void*) noexcept {history.push_back(21);}
void mark(void*) noexcept {history.push_back(22);}
U32 pixel(void*,U32 rgb) noexcept {history.push_back(23);return rgb^0xffU;}
U32 color_key(void*,U32 flags,U32 low,U32 high) noexcept {
    history.push_back(24);assert(flags==8&&low==high&&low==0xAA55U);return 0;
}
U32 present_native(void*,bool windowed) noexcept {
    history.push_back(windowed?25:26);
    if(++attempts==1)return static_cast<U32>(btb::display::kDdErrWasStillDrawing);
    if(attempts==2)return static_cast<U32>(btb::display::kDdErrSurfaceLost);
    return 0;
}
void restore(void*) noexcept {history.push_back(27);}
U32 restore_all(void*) noexcept {history.push_back(28);return 0;}
U32 create_ds(void*,void** out) noexcept {
    history.push_back(29);*out=reinterpret_cast<void*>(0x9);return 0;
}
U32 sound_coop(void*,std::uintptr_t,U32 flags) noexcept {
    history.push_back(30);assert(flags==2);return 0;
}
void format(void*) noexcept {history.push_back(31);}
bool playing(void* p) noexcept {history.push_back(32);return p==reinterpret_cast<void*>(0x1);}
bool eq(std::initializer_list<int> steps) {return history==std::vector<int>(steps);}
}
int main() {
    static_assert(retail_hresult(1)==0);
    static_assert(retail_hresult(0x80004005U)==0x80004005U);
    history.clear();
    assert(initialize_display_for_game_window(nullptr,7,win)==0);
    assert(present_game_frame(reinterpret_cast<void*>(0x1),present_call)==0x80004005U);
    assert(eq({7,3})); // 00401F80, 00402600

    // 00402FF0 / 00402FE0: unregister and release exactly in slot order;
    // restore coop level then DirectDraw COM release; vtable restored.
    DisplayResources display{};
    display.retail.hwnd32=77;
    display.retail.vtable_ptr32=0;
    display.retail.unused_legacy_surface_ptr32=7;
    display.retail.render_surface_ptr32=8;
    display.retail.primary_surface_ptr32=9;
    display.surfaces={reinterpret_cast<void*>(0x10),
                      reinterpret_cast<void*>(0x11),
                      reinterpret_cast<void*>(0x12)};
    display.directdraw=reinterpret_cast<void*>(0x13);
    history.clear();
    display_manager_destructor(display,unregister,release,coop);
    assert(eq({10,4,11,4,12,4,20,4}));
    assert(display.retail.vtable_ptr32==btb::display::kDisplayManagerVtable32);
    assert(display.retail.primary_surface_ptr32==0);
    assert(display.retail.render_surface_ptr32==0);
    // 004020A0 manager wrapper destroys, frees, clears.
    history.clear();void* global=reinterpret_cast<void*>(0x16);
    destroy_display_manager(global,destroy,free_object);
    assert(eq({5,6})&&global==nullptr);
    history.clear();destroy_display_manager(global,destroy,free_object);
    assert(history.empty());

    // 00404E10 always resets vtable and frees DIB.
    history.clear();
    btb::printing::RetailBitmapPrinter32 printer{};
    printer.vtable_ptr32=0;
    bitmap_printer_destructor(printer,reinterpret_cast<void*>(0x1),free_object);
    assert(printer.vtable_ptr32==btb::printing::kBitmapPrinterVtable32);
    assert(eq({6}));

    // 00404530: wave destructor always Close, frees format only file-backed.
    history.clear();
    btb::dsutil::RetailCWaveFile32 wave{};
    wave.m_pwfx=100;
    wave.m_bIsReadingFromMemory=0;
    wave_file_destructor(wave,reinterpret_cast<void*>(0x1),
                         reinterpret_cast<void*>(0x2),wave_close,free_object);
    assert(eq({21,6})&&wave.m_pwfx==0);
    history.clear();wave.m_pwfx=100;wave.m_bIsReadingFromMemory=1;
    wave_file_destructor(wave,reinterpret_cast<void*>(0x1),
                         reinterpret_cast<void*>(0x2),wave_close,free_object);
    assert(eq({21})&&wave.m_pwfx==100);

    // 00409390 returns 1 even when no native COM object existed.
    history.clear();
    void* ds=reinterpret_cast<void*>(0x123);
    assert(shutdown_directsound(ds,destroy,free_object)==1);
    assert(ds==nullptr&&eq({5,6}));
    history.clear();
    assert(shutdown_directsound(ds,destroy,free_object)==1);
    assert(history.empty());

    // 00403B70: marking before pixel conversion before SetColorKey(8).
    history.clear();
    set_surface_transparency_color_key(reinterpret_cast<void*>(0x1),
        reinterpret_cast<void*>(0x2),0xAAAAU,mark,pixel,color_key);
    assert(eq({22,23,24}));

    // 00403480: DDERR_WASSTILLDRAWING retry, SURFACELOST restores,
    // all sources restored, plus global reload flag.
    history.clear();attempts=0;
    PresentState p{};
    p.primary=reinterpret_cast<void*>(0x1);
    p.render=reinterpret_cast<void*>(0x2);
    p.directdraw=reinterpret_cast<void*>(0x3);
    p.windowed=true;
    assert(present_display(p,present_native,restore,restore_all,4)==
           static_cast<U32>(btb::display::kDdErrSurfaceLost));
    assert(eq({25,25,27,27,28})&&p.bitmap_reload);

    // 00403BE0: old COM release, create, cooperative flags2, format.
    history.clear();void* current=reinterpret_cast<void*>(0x1);
    assert(initialize_sound_manager(current,77,release,create_ds,sound_coop,format)==0);
    assert(eq({4,29,30,31}));
    assert(current==reinterpret_cast<void*>(0x9));

    // 00404070 scalar deletion flag low bit; always calls destructor.
    history.clear();void* sound=reinterpret_cast<void*>(0x3);
    assert(sound_scalar_deleting_destructor(sound,0,destroy,free_object)==sound);
    assert(eq({5}));
    history.clear();
    assert(sound_scalar_deleting_destructor(sound,1,destroy,free_object)==sound);
    assert(eq({5,6}));

    // 00404090 releases all buffers, allocated list, then wave.
    history.clear();
    SoundObject obj{};
    std::array<void*,3> list{reinterpret_cast<void*>(1),nullptr,reinterpret_cast<void*>(2)};
    obj.buffers=list;
    obj.buffer_allocation=reinterpret_cast<void*>(0x3);
    obj.retail.m_apDSBuffer=0x4;obj.retail.m_pWaveFile=0x5;
    obj.wave=reinterpret_cast<void*>(0x5);
    sound_destructor(obj,release,free_object,destroy);
    assert(eq({4,4,6,5,6}));
    assert(list[0]==nullptr&&list[2]==nullptr);
    assert(obj.retail.m_apDSBuffer==0&&obj.retail.m_pWaveFile==0);

    // 004042F0: first nonplaying buffer, random if all playing.
    history.clear();
    std::array<void*,3> buffers{reinterpret_cast<void*>(0x1),
        reinterpret_cast<void*>(0x2),reinterpret_cast<void*>(0x1)};
    assert(sound_get_free_buffer(buffers,2,playing)==reinterpret_cast<void*>(0x2));
    assert(eq({32,32}));
    std::array<void*,2> all_busy{reinterpret_cast<void*>(0x1),
                                 reinterpret_cast<void*>(0x1)};
    assert(sound_get_free_buffer(all_busy,1,playing)==all_busy[1]);
    assert(sound_get_free_buffer(std::span<void* const>{},2,playing)==nullptr);

    // 00406780 / 00406440: fixed leaf error and MSVC string terminator.
    static_assert(string_error_code_minus_three()==-3);
    std::array<char,5> text{'H','e','l','l','o'};
    MsvcString str{};
    str.chars=text;str.length=5;str.capacity=5;
    assert(msvc_string_set_end(str,3));
    assert(str.length==3&&text[3]=='\0');
    assert(!msvc_string_set_end(str,5));
    // 00406110 / 00406350: release refcount and clear all metadata.
    str.refcount=3;
    const auto release_ref=msvc_string_release(str);
    assert(release_ref.decrement_reference&&!release_ref.free_allocation);
    assert(str.refcount==2&&str.length==0&&str.chars.empty());
    str.chars=text;str.length=2;str.capacity=5;str.refcount=255;
    const auto free_ref=msvc_string_tidy(str,true);
    assert(free_ref.free_allocation&&!free_ref.decrement_reference);
    str.chars=text;str.length=2;str.capacity=5;
    const auto skipped=msvc_string_tidy(str,false);
    assert(!skipped.free_allocation&&!skipped.decrement_reference);
}
