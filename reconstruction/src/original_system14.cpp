#include "btb/original_system14.hpp"
#include <bit>
namespace btb::original_system14 {
void release_display_resources(DisplayResources& d,UnregisterSlot unreg,
    NativeRelease release,CooperativeNormal cooperative) noexcept {
 // The original 00402FF0 releases exactly auxiliary, render, primary.
 for(std::size_t i=0;i<d.surfaces.size();++i){
  if(unreg)unreg(i);
  if(d.surfaces[i]){
   if(release)release(d.surfaces[i]);
   d.surfaces[i]=nullptr;
  }
 }
 d.retail.unused_legacy_surface_ptr32=0;
 d.retail.render_surface_ptr32=0;
 d.retail.primary_surface_ptr32=0;
 if(d.directdraw){
  if(cooperative)cooperative(d.directdraw,d.retail.hwnd32,8);
  if(release)release(d.directdraw);
 }
}
void display_manager_destructor(DisplayResources& d,UnregisterSlot unreg,
    NativeRelease release,CooperativeNormal cooperative) noexcept {
 d.retail.vtable_ptr32=btb::display::kDisplayManagerVtable32;
 // Original 00402FE0 calls same source destructor body as 00402FF0.
 original_system14::release_display_resources(
     d,unreg,release,cooperative);
}
void destroy_display_manager(void*& global,DestroyDisplay destroy,
    NativeFree deallocate) noexcept {
 if(!global)return;
 void* const original=global;
 if(destroy)destroy(original);
 if(deallocate)deallocate(original);
 global=nullptr;
}
void bitmap_printer_destructor(
 btb::printing::RetailBitmapPrinter32& printer,void* pixels,
 NativeFree deallocate) noexcept {
 printer.vtable_ptr32=btb::printing::kBitmapPrinterVtable32;
 if(deallocate)deallocate(pixels);
}
void wave_file_destructor(
 btb::dsutil::RetailCWaveFile32& wave,void* obj,void* format,
 WaveClose close,NativeFree deallocate) noexcept {
 if(close)close(obj);
 if(wave.m_bIsReadingFromMemory==0 && wave.m_pwfx!=0){
  if(deallocate)deallocate(format);
  wave.m_pwfx=0;
 }
}
I32 shutdown_directsound(void*& manager,SoundManagerDestroy destroy,
    NativeFree deallocate) noexcept {
 if(manager){
  void* const old=manager;
  if(destroy)destroy(old);
  if(deallocate)deallocate(old);
  manager=nullptr;
 }
 return 1;
}
U32 initialize_sound_manager(void*& device,std::uintptr_t window,
    NativeRelease release,CreateDS create,SetCoop cooperative,
    SetPrimaryFormat format) noexcept {
 if(device){
  if(release)release(device);
  device=nullptr;
 }
 if(!create)return 0x80004003U;
 U32 hr=create(nullptr,&device);
 if(std::bit_cast<I32>(hr)<0)return hr;
 if(!device||!cooperative)return 0x80004003U;
 hr=cooperative(device,window,2U);
 if(std::bit_cast<I32>(hr)<0)return hr;
 if(format)format(device);
 return 0;
}
void* sound_scalar_deleting_destructor(void* object,U32 flags,
    SoundDestroy destroy,NativeFree deallocate) noexcept {
 if(destroy)destroy(object);
 if((flags&1U)!=0 && deallocate)deallocate(object);
 return object;
}
void sound_destructor(SoundObject& obj,NativeRelease release,
    NativeFree deallocate,SoundDestroy wave_destroy) noexcept {
 obj.retail.vtable_ptr32=0x0043B2E4U;
 for(auto& buffer:obj.buffers){
  if(buffer){
   if(release)release(buffer);
   buffer=nullptr;
  }
 }
 if(obj.retail.m_apDSBuffer!=0){
  if(deallocate)deallocate(obj.buffer_allocation);
  obj.retail.m_apDSBuffer=0;
 }
 if(obj.retail.m_pWaveFile!=0){
  if(wave_destroy)wave_destroy(obj.wave);
  if(deallocate)deallocate(obj.wave);
  obj.retail.m_pWaveFile=0;
 }
}
void* sound_get_free_buffer(std::span<void* const> buffers,
    U32 random_value,QueryPlaying is_playing) noexcept {
 if(buffers.empty())return nullptr;
 for(void* candidate:buffers){
  if(candidate && (!is_playing||!is_playing(candidate)))
   return candidate;
 }
 return buffers[random_value%buffers.size()];
}
I32 string_error_code_minus_three() noexcept {return -3;}
bool msvc_string_set_end(MsvcString& str,I32 end) noexcept {
 if(end<0 || end>=static_cast<I32>(str.chars.size()))return false;
 str.length=end;
 str.chars[static_cast<std::size_t>(end)]='\0';
 return true;
}
StringRelease msvc_string_tidy(MsvcString& str,bool release) noexcept {
 StringRelease result{};
 if(release && str.chars.data()){
  if(str.refcount==0 || str.refcount==255)
   result.free_allocation=true;
  else {result.decrement_reference=true;--str.refcount;}
 }
 str.chars={};str.length=0;str.capacity=0;
 return result;
}
StringRelease msvc_string_release(MsvcString& str) noexcept {
 // Original 00406110 and 00406350 share exact rep-release semantics.
 return original_system14::msvc_string_tidy(str,true);
}
} // namespace btb::original_system14
