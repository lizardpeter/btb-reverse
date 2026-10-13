#include "btb/original_system14.hpp"
#include <array>
namespace btb::original_system14 {
namespace {
struct Probe {
 int unregisters{},releases{},coops{},deleted{},destroyed{},
     wave_closes{},sets{},formats{},creates{},sound_destroyed{},last_slot{-1};
 std::array<int,3> unregister_order{};
};
Probe g{};
void* const kObject=reinterpret_cast<void*>(0x1000);
void* const kObject2=reinterpret_cast<void*>(0x2000);
void* const kObject3=reinterpret_cast<void*>(0x3000);
void unreg(std::size_t slot) noexcept {
 if(slot<3){g.unregister_order[slot]=static_cast<int>(slot);}
 ++g.unregisters;g.last_slot=static_cast<int>(slot);
}
void release(void*) noexcept {++g.releases;}
void normal(void*,std::uintptr_t,U32 flags) noexcept {
 if(flags==8)++g.coops;
}
void deallocate(void*) noexcept {++g.deleted;}
void destroy(void*) noexcept {++g.destroyed;}
void close_wave(void*) noexcept {++g.wave_closes;}
U32 create_ds(void*,void** out) noexcept {
 *out=kObject3;++g.creates;return 0;
}
U32 set_coop(void*,std::uintptr_t,U32 mode) noexcept {
 if(mode==2)++g.sets;
 return 0;
}
void set_format(void*) noexcept {++g.formats;}
void destroy_sound(void*) noexcept {++g.sound_destroyed;}
bool is_playing(void* value) noexcept {return value==kObject;}
}
bool verify_14_original_system_functions() noexcept {
 g={};
 // Original 00402FF0 releases 3 surfaces and original DirectDraw.
 DisplayResources display{};
 display.surfaces={kObject,kObject2,kObject3};
 display.directdraw=kObject3;
 original_system14::release_display_resources(
    display,unreg,release,normal);
 if(g.unregisters!=3||g.last_slot!=2||g.releases!=4||
    g.coops!=1||display.surfaces[0]||display.surfaces[2])return false;
 // Original 00402FE0 sets retail vtable prior to releasing resources.
 display.surfaces={kObject,nullptr,kObject3};
 original_system14::display_manager_destructor(
    display,unreg,release,normal);
 if(display.retail.vtable_ptr32!=btb::display::kDisplayManagerVtable32||
    g.unregisters!=6||g.releases!=7||g.coops!=2)return false;
 // Original 004020A0 is idempotent destroy then free.
 void* current=kObject;
 original_system14::destroy_display_manager(current,destroy,deallocate);
 if(current!=nullptr||g.destroyed!=1||g.deleted!=1)return false;
 original_system14::destroy_display_manager(current,destroy,deallocate);
 if(g.destroyed!=1)return false;
 // 00404E10 bitmap printer destructor restores vtable before freeing.
 btb::printing::RetailBitmapPrinter32 printer{};
 original_system14::bitmap_printer_destructor(printer,kObject,deallocate);
 if(printer.vtable_ptr32!=btb::printing::kBitmapPrinterVtable32||
    g.deleted!=2)return false;
 // 00404530 closes wave and frees owned format only if not memory data.
 btb::dsutil::RetailCWaveFile32 wave{};
 wave.m_pwfx=1;
 original_system14::wave_file_destructor(
     wave,kObject,kObject2,close_wave,deallocate);
 if(g.wave_closes!=1||g.deleted!=3||wave.m_pwfx!=0)return false;
 wave.m_pwfx=1;wave.m_bIsReadingFromMemory=1;
 original_system14::wave_file_destructor(
     wave,kObject,kObject2,close_wave,deallocate);
 if(g.wave_closes!=2||g.deleted!=3)return false;
 // 00409390 manager destruction returns one whether empty or nonempty.
 void* manager=kObject;
 if(original_system14::shutdown_directsound(
       manager,destroy,deallocate)!=1||manager!=nullptr||
    g.destroyed!=2||g.deleted!=4)return false;
 if(original_system14::shutdown_directsound(
       manager,destroy,deallocate)!=1||g.destroyed!=2)return false;
 // 00403BE0 real existing DS release, DirectSoundCreate, cooperative,
 // primary format, and normalized HRESULT.
 void* directsound=kObject;
 if(original_system14::initialize_sound_manager(
       directsound,0,release,create_ds,set_coop,set_format)!=0||
    directsound!=kObject3||g.creates!=1||g.sets!=1||
    g.formats!=1||g.releases!=8)return false;
 // 00404070 scalar deleting dtor calls destroy even without delete flag.
 if(original_system14::sound_scalar_deleting_destructor(
       kObject,0,destroy_sound,deallocate)!=kObject||
    g.sound_destroyed!=1||g.deleted!=4)return false;
 if(original_system14::sound_scalar_deleting_destructor(
       kObject,1,destroy_sound,deallocate)!=kObject||
    g.sound_destroyed!=2||g.deleted!=5)return false;
 // 00404090 releases buffers, owned buffer array and owned CWaveFile.
 std::array<void*,3> buffers{kObject,kObject2,nullptr};
 SoundObject sound{};
 sound.buffers=buffers;
 sound.buffer_allocation=kObject2;sound.wave=kObject3;
 sound.retail.m_apDSBuffer=1;sound.retail.m_pWaveFile=2;
 original_system14::sound_destructor(
       sound,release,deallocate,destroy_sound);
 if(buffers[0]||buffers[1]||g.sound_destroyed!=3||
    g.releases!=10||g.deleted!=7||
    sound.retail.m_apDSBuffer||sound.retail.m_pWaveFile||
    sound.retail.vtable_ptr32!=0x0043B2E4U)return false;
 // 004042F0 chooses first available nonplaying, otherwise rand()%size.
 const std::array<void*,2> choices{kObject,kObject2};
 if(original_system14::sound_get_free_buffer(
       choices,0,is_playing)!=kObject2)return false;
 if(original_system14::sound_get_free_buffer(
       choices,1,nullptr)!=kObject)return false;
 if(original_system14::sound_get_free_buffer(
       std::span<void* const>{},1,is_playing)!=nullptr)return false;
 // 00406780 fixed original constant -3.
 if(original_system14::string_error_code_minus_three()!=-3)return false;
 // 00406440 sets original MSVC string len and terminal NUL.
 std::array<char,8> strbuf{'B','o','b','s','B','a','n','d'};
 MsvcString str{};
 str.chars=strbuf;str.capacity=8;
 if(!original_system14::msvc_string_set_end(str,3)||
    str.length!=3||strbuf[3]!=0)return false;
 if(original_system14::msvc_string_set_end(str,8))return false;
 // 00406350 on shared rep decrements count; 00406110 delegates tidy.
 str.refcount=2;
 auto result=original_system14::msvc_string_tidy(str,true);
 if(!result.decrement_reference||result.free_allocation||
    str.refcount!=1||str.chars.data()!=nullptr)return false;
 str.chars=strbuf;str.capacity=8;str.refcount=255;
 result=original_system14::msvc_string_release(str);
 if(!result.free_allocation||result.decrement_reference||
    str.length!=0||str.capacity!=0)return false;
 return true;
}
}
