#include "btb/original_locale7.hpp"
#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
namespace btb::original_locale7 {
static_assert(sizeof(WCHAR)==sizeof(U16));
I32 Win32NlsServices::multibyte_to_wide(U32 cp,U32 flags,const char* in,I32 n,U16* out,I32 cap){return MultiByteToWideChar(cp,flags,in,n,reinterpret_cast<LPWSTR>(out),cap);}
I32 Win32NlsServices::wide_to_multibyte(U32 cp,U32 flags,const U16* in,I32 n,char* out,I32 cap){return WideCharToMultiByte(cp,flags,reinterpret_cast<LPCWSTR>(in),n,out,cap,nullptr,nullptr);}
I32 Win32NlsServices::map_ansi(U32 locale,U32 flags,const char* in,I32 n,char* out,I32 cap){return LCMapStringA(locale,flags,in,n,out,cap);}
I32 Win32NlsServices::map_wide(U32 locale,U32 flags,const U16* in,I32 n,void* out,I32 cap){return LCMapStringW(locale,flags,reinterpret_cast<LPCWSTR>(in),n,static_cast<LPWSTR>(out),cap);}
bool Win32NlsServices::type_ansi(U32 locale,U32 type,const char* in,I32 n,U16* out){return GetStringTypeA(locale,type,in,n,out)!=0;}
bool Win32NlsServices::type_wide(U32 type,const U16* in,I32 n,U16* out){return GetStringTypeW(type,reinterpret_cast<LPCWSTR>(in),n,out)!=0;}
bool Win32NlsServices::codepage_info(U32 cp,CodepageInfo& out){CPINFO info{};if(!GetCPInfo(cp,&info))return false;out.maximum_character_size=info.MaxCharSize;for(unsigned i=0;i<12;++i)out.lead_bytes[i]=info.LeadByte[i];return true;}
}
#endif
