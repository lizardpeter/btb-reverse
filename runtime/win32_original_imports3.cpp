#include "btb/win32_original_imports3.hpp"
#if defined(_WIN32)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#define DIRECTINPUT_VERSION 0x0800
#include <windows.h>
#include <ddraw.h>
#include <dinput.h>
#include <dsound.h>
#include <cstring>
namespace btb::original_win32 {
using namespace original_imports3;
namespace {
GUID guid(const Guid& source){GUID out;static_assert(sizeof(out)==sizeof(source));std::memcpy(&out,source.data(),sizeof(out));return out;}
}
I32 NativeImports3::direct_draw(const Guid* adapter,Handle& result,const Guid& iid,Handle outer){
 GUID device{};if(adapter)device=guid(*adapter);const GUID id=guid(iid);void* out=reinterpret_cast<void*>(result);
 const HRESULT hr=::DirectDrawCreateEx(adapter?&device:nullptr,&out,id,reinterpret_cast<IUnknown*>(outer));result=reinterpret_cast<Handle>(out);return hr;
}
I32 NativeImports3::direct_input(Handle instance,U32 version,const Guid& iid,Handle& result,Handle outer){
 // Keep the required input import dynamically bound, as with an original IAT.
 static HMODULE module=::LoadLibraryA("dinput8.dll");
 if(!module)return static_cast<I32>(HRESULT_FROM_WIN32(ERROR_MOD_NOT_FOUND));
 const auto create=reinterpret_cast<decltype(&::DirectInput8Create)>(::GetProcAddress(module,"DirectInput8Create"));
 if(!create)return static_cast<I32>(HRESULT_FROM_WIN32(ERROR_PROC_NOT_FOUND));
 void* out=reinterpret_cast<void*>(result);const GUID id=guid(iid);const HRESULT hr=create(reinterpret_cast<HINSTANCE>(instance),version,id,&out,reinterpret_cast<IUnknown*>(outer));result=reinterpret_cast<Handle>(out);return hr;
}
I32 NativeImports3::direct_sound(const Guid* device,Handle& result,Handle outer){
 GUID id{};if(device)id=guid(*device);auto* out=reinterpret_cast<IDirectSound8*>(result);const HRESULT hr=::DirectSoundCreate8(device?&id:nullptr,&out,reinterpret_cast<IUnknown*>(outer));result=reinterpret_cast<Handle>(out);return hr;
}
}
#endif
