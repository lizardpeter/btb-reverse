#pragma once
#include "btb/original_msvc30.hpp"
#include <array>
#include <cstdint>
namespace btb::original_locale7 {
using U8=std::uint8_t;using U16=std::uint16_t;using U32=std::uint32_t;using I32=std::int32_t;
struct CodepageInfo { U32 maximum_character_size{};std::array<U8,12> lead_bytes{}; };
// Ordinary CTYPE locale and multibyte locale are distinct original globals.
struct State {
    U32 locale{},codepage{},maximum_character_size{1},mapping_api{},type_api{};
    std::array<U16,257> classes{}; // index 0 is EOF, byte N lives at N+1
    U32 multibyte_locale{},multibyte_codepage{};
    std::array<U8,257> multibyte_flags{};
    std::array<U8,256> case_map{};
    I32 error{};
};
// Exact NLS calls are required. Output capacities are bytes for sort keys,
// UTF-16 units otherwise. Negative input lengths retain Win32 NUL semantics.
class NlsServices {
public:
    virtual ~NlsServices()=default;
    virtual I32 multibyte_to_wide(U32,U32,const char*,I32,U16*,I32)=0;
    virtual I32 wide_to_multibyte(U32,U32,const U16*,I32,char*,I32)=0;
    virtual I32 map_ansi(U32,U32,const char*,I32,char*,I32)=0;
    virtual I32 map_wide(U32,U32,const U16*,I32,void*,I32)=0;
    virtual bool type_ansi(U32,U32,const char*,I32,U16*)=0;
    virtual bool type_wide(U32,const U16*,I32,U16*)=0;
    virtual bool codepage_info(U32,CodepageInfo&)=0;
};
// 0043646F, 004365C0, 004370B3, 00438404, 004375A9, 004327E1, 00436F12
I32 convert_character(State&,U16*,const char*,U32,NlsServices&);
U32 classify(State&,I32,U32,NlsServices&);
I32 map_narrow(State&,U32,U32,const char*,I32,char*,I32,U32,bool,NlsServices&);
bool classify_narrow(State&,U32,const char*,I32,U16*,U32,U32,bool,NlsServices&);
U32 lower_character(State&,U32,NlsServices&);
U32 upper_multibyte_character(State&,U32,NlsServices&);
void build_multibyte_case_table(State&,NlsServices&);
// Connect the recovered 004365C0 to existing 00436598/00431D39 callers.
class ClassificationLink {
public:
    ClassificationLink(State& state,NlsServices& services):state_(state),services_(services){}
    U32 class_eight(U32 value);
    U32 hex_fold(U32 value);
private:
    static U32 classify_callback(U32,U32,void*);
    State& state_;NlsServices& services_;
};
#ifdef _WIN32
class Win32NlsServices final:public NlsServices {
public:
    I32 multibyte_to_wide(U32,U32,const char*,I32,U16*,I32) override;
    I32 wide_to_multibyte(U32,U32,const U16*,I32,char*,I32) override;
    I32 map_ansi(U32,U32,const char*,I32,char*,I32) override;
    I32 map_wide(U32,U32,const U16*,I32,void*,I32) override;
    bool type_ansi(U32,U32,const char*,I32,U16*) override;
    bool type_wide(U32,const U16*,I32,U16*) override;
    bool codepage_info(U32,CodepageInfo&) override;
};
#endif
bool verify_7_original_locale_functions();
}
