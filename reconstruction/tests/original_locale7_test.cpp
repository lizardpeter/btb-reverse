#include "btb/original_locale7.hpp"
#include <algorithm>
#include <cassert>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string_view>
#include <vector>
using namespace btb::original_locale7;
// Controlled NLS provider tests wrapper protocol, not platform collation parity.
struct Nls final:NlsServices {
    bool wide_supported{true},ansi_supported{true},info_ok{true},convert_ok{true},map_ok{true},type_ok{true};
    std::vector<U32> trace;
    static U16 type(U16 ch){return ch>='A' && ch<='Z'?1:ch>='a' && ch<='z'?2:ch>='0' && ch<='9'?4:ch==' '?8:0;}
    static U16 map(U16 ch,U32 flags){if(flags&0x100 && ch>='A' && ch<='Z')return ch+32;if(flags&0x200 && ch>='a' && ch<='z')return ch-32;return ch;}
    I32 multibyte_to_wide(U32 cp,U32 flags,const char* in,I32 n,U16* out,I32 capacity) override {
        trace.insert(trace.end(),{1,cp,flags,static_cast<U32>(n),static_cast<U32>(capacity)});
        if(!convert_ok)return 0;
        if(n<0)n=static_cast<I32>(std::strlen(in)+1);
        I32 size=n;
        if(cp==932 && n==2 && static_cast<U8>(in[0])==0x81)size=1;
        if(!capacity)return size;
        if(capacity<size)return 0;
        if(size!=n)out[0]=static_cast<U16>(0x8100|static_cast<U8>(in[1]));
        else for(I32 i=0;i<n;++i)out[i]=static_cast<U8>(in[i]);
        return size;
    }
    I32 wide_to_multibyte(U32 cp,U32 flags,const U16* in,I32 n,char* out,I32 capacity) override {
        trace.insert(trace.end(),{2,cp,flags,static_cast<U32>(n),static_cast<U32>(capacity)});
        if(!convert_ok)return 0;const I32 size=cp==932 && n==1 && in[0]>=0x8100?2:n;
        if(!capacity)return size;if(capacity<size)return 0;
        if(size!=n){out[0]=static_cast<char>(in[0]>>8);out[1]=static_cast<char>(in[0]);}
        else for(I32 i=0;i<n;++i)out[i]=static_cast<char>(in[i]);return size;
    }
    I32 map_ansi(U32 locale,U32 flags,const char* in,I32 n,char* out,I32 capacity) override {
        trace.insert(trace.end(),{3,locale,flags,static_cast<U32>(n),static_cast<U32>(capacity)});
        if(!ansi_supported || !map_ok)return 0;if(n<0)n=static_cast<I32>(std::strlen(in)+1);
        if(!capacity)return n;if(capacity<n)return 0;for(I32 i=0;i<n;++i)out[i]=static_cast<char>(map(static_cast<U8>(in[i]),flags));return n;
    }
    I32 map_wide(U32 locale,U32 flags,const U16* in,I32 n,void* out,I32 capacity) override {
        trace.insert(trace.end(),{4,locale,flags,static_cast<U32>(n),static_cast<U32>(capacity)});
        if(!wide_supported || !map_ok)return 0;
        if(n<0){n=0;while(in[n++]){}}
        if(flags&0x400){if(!capacity)return 4;if(capacity<4)return 0;std::memcpy(out,"KEY\0",4);return 4;}
        if(!capacity)return n;if(capacity<n)return 0;auto* dest=static_cast<U16*>(out);for(I32 i=0;i<n;++i)dest[i]=map(in[i],flags);return n;
    }
    bool type_ansi(U32 locale,U32 flags,const char* in,I32 n,U16* out) override {
        trace.insert(trace.end(),{5,locale,flags,static_cast<U32>(n),0});if(!ansi_supported || !type_ok)return false;
        if(n<0)n=static_cast<I32>(std::strlen(in)+1);for(I32 i=0;i<n;++i)out[i]=type(static_cast<U8>(in[i]));return true;
    }
    bool type_wide(U32 flags,const U16* in,I32 n,U16* out) override {
        trace.insert(trace.end(),{6,0,flags,static_cast<U32>(n),0});if(!wide_supported || !type_ok)return false;
        for(I32 i=0;i<n;++i)out[i]=type(in[i]);return true;
    }
    bool codepage_info(U32 cp,CodepageInfo& info) override {trace.insert(trace.end(),{7,cp,0,0,0});if(!info_ok)return false;info.maximum_character_size=cp==932?2:1;info.lead_bytes={0x81,0x82};return true;}
};
int main() {
    assert(verify_7_original_locale_functions());Nls api;State s;s.codepage=1252;s.multibyte_codepage=1252;s.multibyte_locale=0x409;
    for(unsigned i=0;i<256;++i)s.classes[i+1]=Nls::type(static_cast<U16>(i));
    for(unsigned i=0;i<256;++i) {
        assert(classify(s,static_cast<I32>(i),15,api)==Nls::type(static_cast<U16>(i)));
        assert(lower_character(s,i,api)==Nls::map(static_cast<U16>(i),0x100));
        U16 wide=123;const char input[2]{static_cast<char>(i),0};assert(convert_character(s,&wide,input,1,api)==(i?1:0) && wide==i);
    }
    assert(api.trace.empty());build_multibyte_case_table(s,api);
    for(unsigned i=0;i<256;++i)assert(upper_multibyte_character(s,i,api)==Nls::map(static_cast<U16>(i),0x200));
    assert(s.case_map['A']=='a' && (s.multibyte_flags['a'+1]&0x20));
    s.locale=0x409;s.mapping_api=0;char output[16]{};api.trace.clear();
    assert(map_narrow(s,s.locale,0x100,"Ab\0C",4,output,16,0,true,api)==2 && std::string_view(output,2)=="ab");
    // Probe then strict sizing, non-strict conversion, wide sizing/output, byte output.
    assert((api.trace==std::vector<U32>{4,0,0x100,1,0,1,1252,9,2,0,1,1252,1,2,2,4,0x409,0x100,2,0,4,0x409,0x100,2,2,2,1252,0x220,2,16}));
    assert(map_narrow(s,s.locale,0x100,"Ab",2,nullptr,0,0,false,api)==2);
    assert(map_narrow(s,s.locale,0x100,"Ab",2,output,1,0,false,api)==0);
    assert(map_narrow(s,s.locale,0x400,"Ab",2,nullptr,0,0,true,api)==4);
    assert(map_narrow(s,s.locale,0x400,"Ab",2,output,3,0,true,api)==0);
    assert(map_narrow(s,s.locale,0x400,"Ab",2,output,4,0,true,api)==4 && std::memcmp(output,"KEY\0",4)==0);
    assert(lower_character(s,'A',api)=='a' && lower_character(s,'b',api)=='b');
    s.maximum_character_size=2;s.codepage=932;s.classes[0x81+1]|=0x8000;
    U16 wide=0;const char dbcs[3]{static_cast<char>(0x81),'A',0};
    assert(convert_character(s,&wide,dbcs,1,api)==-1 && s.error==42);
    assert(convert_character(s,&wide,dbcs,2,api)==2 && wide==0x8141);
    api.convert_ok=false;wide=222;assert(convert_character(s,&wide,dbcs,2,api)==2 && wide==222);
    assert(convert_character(s,&wide,"A",1,api)==-1);
    const char bad[2]{static_cast<char>(0x81),0};assert(convert_character(s,&wide,bad,2,api)==-1);api.convert_ok=true;
    assert(lower_character(s,0x8141,api)==0x4181); // Original ordinary tolower packs little-endian bytes.
    s.multibyte_codepage=932;s.multibyte_flags[0x81+1]|=4;
    assert(upper_multibyte_character(s,0x8141,api)==0x8141); // _mbctoupper packs big-endian bytes.
    ClassificationLink link(s,api);assert(link.class_eight(' ')==8 && link.hex_fold('8')=='8');
    api.map_ok=false;assert(lower_character(s,'A',api)=='A');api.map_ok=true;
    api.type_ok=false;assert(classify(s,0x8141,1,api)==0);api.type_ok=true;
    // ANSI API fallback and failure of both availability probes.
    State ansi;ansi.locale=0x409;ansi.codepage=1252;Nls fallback;fallback.wide_supported=false;
    assert(map_narrow(ansi,0x409,0x200,"ab",2,output,16,0,true,fallback)==2 && ansi.mapping_api==2 && std::string_view(output,2)=="AB");
    U16 classes[2]{};assert(classify_narrow(ansi,1,"Ab",2,classes,0,0,true,fallback) && ansi.type_api==2 && classes[0]==1 && classes[1]==2);
    fallback.ansi_supported=false;ansi.mapping_api=0;ansi.type_api=0;
    assert(map_narrow(ansi,0,0x100,"A",1,output,16,0,true,fallback)==0 && ansi.mapping_api==0);
    assert(!classify_narrow(ansi,1,"A",1,classes,0,0,true,fallback) && ansi.type_api==0);
    // GetCPInfo failure retains unrelated flag bits and initializes ASCII cases.
    api.info_ok=false;s.multibyte_flags.fill(4);build_multibyte_case_table(s,api);
    assert(s.multibyte_flags['A'+1]==0x14 && s.multibyte_flags['a'+1]==0x24 && s.case_map[128]==0);
#ifdef _WIN32
    Win32NlsServices win;State native;native.locale=0x409;native.codepage=1252;native.multibyte_codepage=1252;native.multibyte_locale=0x409;
    assert(map_narrow(native,0x409,0x100,"BOB",3,output,16,1252,true,win)==3 && std::string_view(output,3)=="bob");
    assert(classify_narrow(native,1,"Aa",2,classes,1252,0x409,true,win) && (classes[0]&1) && (classes[1]&2));
    build_multibyte_case_table(native,win);assert(upper_multibyte_character(native,'a',win)=='A');
#endif
    std::cout<<"Seven locale bodies, NLS call protocols, byte cases and connected classification callers passed\n";
}
