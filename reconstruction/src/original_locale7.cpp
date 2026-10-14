#include "btb/original_locale7.hpp"
#include "btb/original_crt_batch_32.hpp"
#include <bit>
#include <stdexcept>
#include <vector>
namespace btb::original_locale7 {
namespace {
U16 flags(const State& s,I32 value) {
    if(value< -1 || value>255)throw std::out_of_range("original CTYPE index");
    return s.classes[static_cast<std::size_t>(value+1)];
}
bool lead(const State& s,U32 value){return (s.classes[(value&255)+1]&0x8000)!=0;}
}
I32 convert_character(State& s,U16* out,const char* input,U32 count,NlsServices& api) {
    if(!input || !count)return 0;
    const auto byte=static_cast<U8>(*input);
    if(!byte){if(out)*out=0;return 0;}
    if(!s.locale){if(out)*out=byte;return 1;}
    if(!lead(s,byte)) {
        if(api.multibyte_to_wide(s.codepage,9,input,1,out,out?1:0))return 1;
    } else {
        if(std::bit_cast<I32>(s.maximum_character_size)>1) {
            if(std::bit_cast<I32>(count)<std::bit_cast<I32>(s.maximum_character_size)){s.error=42;return -1;}
            if(api.multibyte_to_wide(s.codepage,9,input,static_cast<I32>(s.maximum_character_size),out,out?1:0))
                return static_cast<I32>(s.maximum_character_size);
        }
        if(s.maximum_character_size<=count && input[1]!=0)return static_cast<I32>(s.maximum_character_size);
    }
    s.error=42;return -1;
}
bool classify_narrow(State& s,U32 type,const char* input,I32 count,U16* out,U32 cp,U32 locale,bool strict,NlsServices& api) {
    if(!s.type_api) {
        U16 probe[2]{};const U16 empty[1]{};
        if(api.type_wide(1,empty,1,probe))s.type_api=1;
        else if(api.type_ansi(0,1,"",1,probe))s.type_api=2;
        else return false;
    }
    if(s.type_api==2)return api.type_ansi(locale?locale:s.locale,type,input,count,out);
    if(s.type_api!=1)return false;
    if(!cp)cp=s.codepage;
    const I32 size=api.multibyte_to_wide(cp,strict?9:1,input,count,nullptr,0);
    if(size<=0)return false;
    std::vector<U16> wide(static_cast<std::size_t>(size));
    const I32 converted=api.multibyte_to_wide(cp,1,input,count,wide.data(),size);
    return converted>0 && api.type_wide(type,wide.data(),converted,out);
}
U32 classify(State& s,I32 value,U32 mask,NlsServices& api) {
    if(static_cast<U32>(value)+1U<257U)return flags(s,value)&mask;
    const U32 bits=std::bit_cast<U32>(value);
    char input[3]{};I32 count=1;
    if(lead(s,bits>>8)){input[0]=static_cast<char>(bits>>8);input[1]=static_cast<char>(bits);count=2;}
    else input[0]=static_cast<char>(bits);
    U16 result[2]{};
    return classify_narrow(s,1,input,count,result,0,0,true,api)?result[0]&mask:0;
}
I32 map_narrow(State& s,U32 locale,U32 map,const char* input,I32 count,char* out,I32 capacity,U32 cp,bool strict,NlsServices& api) {
    if(!s.mapping_api) {
        const U16 empty[1]{};
        if(api.map_wide(0,0x100,empty,1,nullptr,0))s.mapping_api=1;
        else if(api.map_ansi(0,0x100,"",1,nullptr,0))s.mapping_api=2;
        else return 0;
    }
    if(count>0)count=static_cast<I32>(original_crt32::bounded_length(input,static_cast<std::size_t>(count)));
    if(s.mapping_api==2)return api.map_ansi(locale,map,input,count,out,capacity);
    if(s.mapping_api!=1)return 0;
    if(!cp)cp=s.codepage;
    const I32 size=api.multibyte_to_wide(cp,strict?9:1,input,count,nullptr,0);
    if(size<=0)return 0;
    std::vector<U16> wide(static_cast<std::size_t>(size));
    if(!api.multibyte_to_wide(cp,1,input,count,wide.data(),size))return 0;
    I32 mapped=api.map_wide(locale,map,wide.data(),size,nullptr,0);
    if(mapped<=0)return 0;
    if(map&0x400) {
        if(!capacity)return mapped;
        if(capacity<mapped)return 0;
        return api.map_wide(locale,map,wide.data(),size,out,capacity)?mapped:0;
    }
    std::vector<U16> result(static_cast<std::size_t>(mapped));
    if(!api.map_wide(locale,map,wide.data(),size,result.data(),mapped))return 0;
    mapped=api.wide_to_multibyte(cp,0x220,result.data(),mapped,capacity?out:nullptr,capacity);
    return mapped>0?mapped:0;
}
U32 lower_character(State& s,U32 value,NlsServices& api) {
    const I32 signed_value=std::bit_cast<I32>(value);
    if(!s.locale)return signed_value>64 && signed_value<91?value+32:value;
    if(signed_value<256) {
        const U32 upper=s.maximum_character_size<2?flags(s,signed_value)&1:classify(s,signed_value,1,api);
        if(!upper)return value;
    }
    char input[3]{},output[3]{};I32 count=1;
    if(lead(s,value>>8)){input[0]=static_cast<char>(value>>8);input[1]=static_cast<char>(value);count=2;}
    else input[0]=static_cast<char>(value);
    const I32 result=map_narrow(s,s.locale,0x100,input,count,output,3,0,true,api);
    if(!result)return value;
    return static_cast<U8>(output[0])|(result==1?0:static_cast<U32>(static_cast<U8>(output[1]))<<8);
}
U32 upper_multibyte_character(State& s,U32 value,NlsServices& api) {
    if(value<256)return s.multibyte_flags[value+1]&0x20?s.case_map[value]:value;
    if(!(s.multibyte_flags[((value>>8)&255)+1]&4))return value;
    const char input[2]{static_cast<char>(value>>8),static_cast<char>(value)};char output[2]{};
    if(!map_narrow(s,s.multibyte_locale,0x200,input,2,output,2,s.multibyte_codepage,true,api))return value;
    return (static_cast<U32>(static_cast<U8>(output[0]))<<8)|static_cast<U8>(output[1]);
}
void build_multibyte_case_table(State& s,NlsServices& api) {
    CodepageInfo info;
    if(api.codepage_info(s.multibyte_codepage,info)) {
        std::array<char,256> input{},lower{},upper{};std::array<U16,256> classes{};
        for(unsigned i=0;i<256;++i)input[i]=static_cast<char>(i);
        input[0]=' ';
        for(unsigned pair=0;pair+1<info.lead_bytes.size() && info.lead_bytes[pair];pair+=2)
            for(unsigned i=info.lead_bytes[pair];i<=info.lead_bytes[pair+1];++i)input[i]=' ';
        // The original ignores NLS failure and reads uninitialized stack data.
        // Reject this invalid domain rather than silently produce invented tables.
        if(!classify_narrow(s,1,input.data(),256,classes.data(),s.multibyte_codepage,s.multibyte_locale,false,api) ||
           !map_narrow(s,s.multibyte_locale,0x100,input.data(),256,lower.data(),256,s.multibyte_codepage,false,api) ||
           !map_narrow(s,s.multibyte_locale,0x200,input.data(),256,upper.data(),256,s.multibyte_codepage,false,api))
            throw std::runtime_error("original multibyte case-table NLS failure");
        for(unsigned i=0;i<256;++i) {
            if(classes[i]&1){s.multibyte_flags[i+1]|=0x10;s.case_map[i]=static_cast<U8>(lower[i]);}
            else if(classes[i]&2){s.multibyte_flags[i+1]|=0x20;s.case_map[i]=static_cast<U8>(upper[i]);}
            else s.case_map[i]=0;
        }
    } else {
        for(unsigned i=0;i<256;++i) {
            if(i>=65 && i<=90){s.multibyte_flags[i+1]|=0x10;s.case_map[i]=static_cast<U8>(i+32);}
            else if(i>=97 && i<=122){s.multibyte_flags[i+1]|=0x20;s.case_map[i]=static_cast<U8>(i-32);}
            else s.case_map[i]=0;
        }
    }
}
U32 ClassificationLink::classify_callback(U32 value,U32 mask,void* context) {
    auto& self=*static_cast<ClassificationLink*>(context);
    return classify(self.state_,std::bit_cast<I32>(value),mask,self.services_);
}
U32 ClassificationLink::class_eight(U32 value) {
    std::array<U8,257> low{};for(unsigned i=0;i<257;++i)low[i]=static_cast<U8>(state_.classes[i]);
    return original_msvc30::class_eight(value,static_cast<I32>(state_.maximum_character_size),std::span<const U8>(low).subspan(1),classify_callback,this);
}
U32 ClassificationLink::hex_fold(U32 value) {
    std::array<U8,257> low{};for(unsigned i=0;i<257;++i)low[i]=static_cast<U8>(state_.classes[i]);
    return original_msvc30::hex_fold(value,static_cast<I32>(state_.maximum_character_size),std::span<const U8>(low).subspan(1),classify_callback,this);
}
}
