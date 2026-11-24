#include "btb/original_runtime6.hpp"
#include "btb/original_crt_batch_32.hpp"
#include <bit>
#include <cstring>
#include <stdexcept>
namespace btb::original_runtime6 {
U32 upper_character(State& s,U32 value,NlsServices& io) {
    const I32 signed_value=std::bit_cast<I32>(value);
    if(!s.locale)return signed_value>96 && signed_value<123?value-32:value;
    if(signed_value<256) {
        if(signed_value< -1)throw std::out_of_range("original uppercase CTYPE index");
        const U32 lower=s.maximum_character_size<2?s.classes[static_cast<std::size_t>(signed_value+1)]&2:classify(s,signed_value,2,io);
        if(!lower)return value;
    }
    char input[3]{},output[3]{};I32 count=1;
    if(s.classes[((value>>8)&255)+1]&0x8000){input[0]=static_cast<char>(value>>8);input[1]=static_cast<char>(value);count=2;}
    else input[0]=static_cast<char>(value);
    const I32 result=map_narrow(s,s.locale,0x200,input,count,output,3,0,true,io);
    if(!result)return value;
    return static_cast<std::uint8_t>(output[0])|(result==1?0:static_cast<U32>(static_cast<std::uint8_t>(output[1]))<<8);
}
I32 parse_integer(State& s,const char* text,NlsServices& io) {
    auto is=[&](unsigned char c,U32 mask){return s.maximum_character_size<2?s.classes[c+1]&mask:classify(s,c,mask,io);};
    while(is(static_cast<unsigned char>(*text),8))++text;
    const char sign=*text;if(sign=='-' || sign=='+')++text;
    U32 value=0;
    while(is(static_cast<unsigned char>(*text),4)){value=value*10+static_cast<unsigned char>(*text)-'0';++text;}
    return std::bit_cast<I32>(sign=='-'?0U-value:value);
}
void insert_decimal_point(State& s,std::span<char> text,char decimal,NlsServices& io) {
    const std::size_t length=original_crt32::bounded_length(text.data(),text.size());
    if(!length || length+1>=text.size())throw std::out_of_range("original decimal insertion buffer");
    std::size_t at=0;
    if(lower_character(s,static_cast<U32>(static_cast<signed char>(text[0])),io)!='e') {
        do {++at;}while(at<length && (s.maximum_character_size<2?s.classes[static_cast<std::size_t>(static_cast<unsigned char>(text[at]))+1]&4:classify(s,static_cast<signed char>(text[at]),4,io)));
    }
    std::memmove(text.data()+at+1,text.data()+at,length-at+1);text[at]=decimal;
}
U32 fetch_short_argument(std::span<const std::byte> memory,U32 base,U32& cursor) {
    const U32 offset=cursor-base;cursor+=4;
    if(offset>memory.size() || memory.size()-offset<2)throw std::out_of_range("short guest vararg memory");
    return (cursor&0xffff0000U)|std::to_integer<U32>(memory[offset])|(std::to_integer<U32>(memory[offset+1])<<8);
}
namespace {
struct Known {U32 codepage,a,b,c;std::array<std::uint8_t,32> ranges;};
constexpr std::array<Known,5> known{{
    Known{932,2189001312,33,0,{{166,223,0,0,0,0,0,0,161,165,0,0,0,0,0,0,129,159,224,252,0,0,0,0,64,126,128,252,0,0,0,0}}},
    Known{936,2749014977,32,0,{{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,129,254,0,0,0,0,0,0,64,254,0,0,0,0,0,0}}},
    Known{949,2749014977,32,0,{{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,129,254,0,0,0,0,0,0,65,254,0,0,0,0,0,0}}},
    Known{950,2732892879,2732916762,6005480,{{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,129,254,0,0,0,0,0,0,64,126,161,254,0,0,0,0}}},
    Known{1361,3663649361,3663659040,3332714,{{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,129,211,216,222,224,249,0,0,49,126,129,254,0,0,0,0}}},}};
I32 get_oem(void* context){return static_cast<SystemCodepages*>(context)->oem();}
I32 get_ansi(void* context){return static_cast<SystemCodepages*>(context)->ansi();}
}
I32 set_multibyte_codepage(State& s,original_runtime8::Locale& legacy,U32& system,I32 request,SystemCodepages& codes,NlsServices& io) {
    const U32 cp=static_cast<U32>(original_runtime8::choose_codepage(request,static_cast<I32>(s.codepage),system,get_oem,get_ansi,&codes));
    if(cp==legacy.codepage)return 0;
    auto finish=[&](){s.multibyte_codepage=legacy.codepage;s.multibyte_locale=legacy.locale_id;s.multibyte_flags=legacy.table;build_multibyte_case_table(s,io);legacy.table=s.multibyte_flags;return 0;};
    if(cp) {
        for(const auto& row:known)if(row.codepage==cp) {
            legacy.table.fill(0);
            for(unsigned type=0;type<4;++type)for(unsigned i=type*8;i<type*8+8 && row.ranges[i] && row.ranges[i+1];i+=2)
                for(unsigned c=row.ranges[i];c<=row.ranges[i+1];++c)legacy.table[c+1]|=static_cast<std::uint8_t>(1U<<type);
            legacy.is_multibyte=1;legacy.codepage=cp;legacy.locale_id=original_crt32::map_runtime_code(cp);
            legacy.flags=row.a;legacy.state_a=row.b;legacy.state_b=row.c;return finish();
        }
        CodepageInfo info;
        if(io.codepage_info(cp,info)) {
            legacy.table.fill(0);legacy.codepage=cp;legacy.locale_id=0;
            legacy.is_multibyte=info.maximum_character_size>=2?1:0;
            if(legacy.is_multibyte) {
                for(unsigned i=0;i+1<info.lead_bytes.size() && info.lead_bytes[i] && info.lead_bytes[i+1];i+=2)
                    for(unsigned c=info.lead_bytes[i];c<=info.lead_bytes[i+1];++c)legacy.table[c+1]|=4;
                for(unsigned c=1;c<255;++c)legacy.table[c+1]|=8;
                legacy.locale_id=original_crt32::map_runtime_code(cp);
            }
            legacy.flags=0;legacy.state_a=0;legacy.state_b=0;return finish();
        }
        if(!system)return -1;
    }
    original_runtime8::reset_locale(legacy);return finish();
}
original_runtime16::Stream32* open_stream(const char* path,const char* mode,U32 share,original_runtime16::Stream32& stream,U32 defaults,U32& allocations,OpenServices& io) {
    U32 flags,stream_flags;
    switch(*mode) {
        case 'r':flags=0;stream_flags=defaults|1;break;
        case 'w':flags=0x301;stream_flags=defaults|2;break;
        case 'a':flags=0x109;stream_flags=defaults|2;break;
        default:return nullptr;
    }
    bool commit=false,locality=false,keep=true;
    for(++mode;*mode && keep;++mode) {
        switch(*mode) {
            case '+':if(flags&2){keep=false;break;}flags=(flags&~1U)|2;stream_flags=(stream_flags&~3U)|0x80;break;
            case 'b':case 't':if(flags&0xc000){keep=false;break;}flags|=*mode=='b'?0x8000:0x4000;break;
            case 'c':case 'n':if(commit){keep=false;break;}commit=true;if(*mode=='c')stream_flags|=0x4000;else stream_flags&=~0x4000U;break;
            case 'R':case 'S':if(locality){keep=false;break;}locality=true;flags|=*mode=='R'?0x10:0x20;break;
            case 'T':if(flags&0x1000)keep=false;else flags|=0x1000;break;
            case 'D':if(flags&0x40)keep=false;else flags|=0x40;break;
            default:keep=false;break;
        }
    }
    const I32 descriptor=io.open_file(path,flags,share,0x1a4);if(descriptor<0)return nullptr;
    ++allocations;auto& w=stream.words;w[3]=stream_flags;w[1]=0;w[0]=0;w[2]=0;w[7]=0;w[4]=static_cast<U32>(descriptor);return &stream;
}
}
