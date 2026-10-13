#include "btb/original_runtime8.hpp"
#include <array>
#include <cstddef>
namespace btb::original_runtime8 {
namespace {
I32 oem(void*){return 437;}
I32 ansi(void*){return 1252;}
I32 compare(std::string_view a,std::string_view b,std::size_t n,void*){
 if(a.substr(0,n)==b.substr(0,n))return 2;
 return a<b?1:3;
}
bool lead(U8 value,void*){return value==0x81;}
I32 compare_bytes(const std::byte* a,const std::byte* b,void*){
 const auto x=std::to_integer<I32>(*a),y=std::to_integer<I32>(*b);
 return x==y?0:x<y?-1:1;
}
}
bool verify_8_original_algorithms() noexcept {
 Locale local{};local.table.fill(255U);local.codepage=55;local.state_a=3;
 reset_locale(local);
 if(local.codepage||local.state_a||local.table[256]!=0||
    local.table[0]!=0)return false;
 U32 system=0;
 if(choose_codepage(-2,800,system,oem,ansi,nullptr)!=437||system!=1)
    return false;
 if(choose_codepage(-3,800,system,oem,ansi,nullptr)!=1252||system!=1)
    return false;
 if(choose_codepage(-4,800,system,oem,ansi,nullptr)!=800||system!=1)
    return false;
 if(choose_codepage(1200,800,system,oem,ansi,nullptr)!=1200||system)
    return false;
 const std::array<ErrorPair,2> codes{{{5,13},{8,12}}};
 if(map_win_error(8,codes)!=12 || map_win_error(19,codes)!=13||
    map_win_error(190,codes)!=8 || map_win_error(900,codes)!=22)
    return false;
 std::array<U8,0x84> flags{};flags[0x82]=4;
 const U8 native[]{0x81,0x40,'X',0};
 if(multibyte_chr(native,0x8140,true,flags)!=native ||
    multibyte_chr(native,'X',true,flags)!=native+2 ||
    multibyte_chr(native,0,true,flags)!=native+3 ||
    multibyte_chr(native,0x81,true,flags)!=nullptr)return false;
 if(mbsnbicoll("PATH","PATH",4,compare,nullptr)!=0 ||
    mbsnbicoll("A","B",1,compare,nullptr)!=-1 ||
    mbsnbicoll("A","B",0,compare,nullptr)!=0)return false;
 const std::array<std::string_view,3> env{"HOME=tmp","PATH=/tmp","PA=3"};
 if(environment_find(env,"PATH",compare,nullptr)!=1 ||
    environment_find(env,"MISSING",compare,nullptr)!=-3 ||
    environment_find(env,"PA",compare,nullptr)!=2)return false;
 if(argv_token_end("\"AB CD\"  NEXT",0,lead,nullptr)!=9 ||
    argv_token_end("TEST another",0,lead,nullptr)!=5)return false;
 std::array<std::byte,4> data{
     std::byte{4},std::byte{1},std::byte{3},std::byte{2}};
 selection_sort_bytes(data,1,compare_bytes,nullptr);
 return data==std::array<std::byte,4>{
     std::byte{1},std::byte{2},std::byte{3},std::byte{4}};
}
}
