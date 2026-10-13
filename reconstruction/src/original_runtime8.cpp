#include "btb/original_runtime8.hpp"
#include "btb/original_crt17.hpp"
#include <limits>
namespace btb::original_runtime8 {
void reset_locale(Locale& state) noexcept {state=Locale{};}
I32 choose_codepage(I32 requested,I32 current,U32& use_system,
                    Codepage oem,Codepage ansi,void* ctx){
 if(requested==-2){use_system=1;return oem?oem(ctx):0;}
 if(requested==-3){use_system=1;return ansi?ansi(ctx):0;}
 use_system=static_cast<U32>(requested==-4);
 return requested==-4?current:requested;
}
I32 map_win_error(I32 number,std::span<const ErrorPair> mappings) noexcept {
 for(const auto& x:mappings)if(x.win==number)return x.err;
 if(number>18&&number<37)return 13;
 if(number>=188&&number<=202)return 8;
 return 22;
}
const U8* multibyte_chr(const U8* text,U32 needle,bool multibyte,
                        std::span<const U8> flags) noexcept {
 if(!text)return nullptr;
 const U32 target=needle&0xffffU;
 for(std::size_t i=0;;){
  const U8 b=text[i];
  if(!b)return target==0?text+i:nullptr;
  if(multibyte&&static_cast<std::size_t>(b)+1<flags.size() &&
     (flags[static_cast<std::size_t>(b)+1]&4U)!=0){
   if(!text[i+1])return nullptr;
   if(target==((static_cast<U32>(b)<<8U)|text[i+1]))return text+i;
   i+=2;
  } else {
   if(target==b)return text+i;
   ++i;
  }
 }
}
I32 mbsnbicoll(std::string_view a,std::string_view b,std::size_t n,
               Compare native,void* ctx){
 if(n==0)return 0;
 const I32 value=native?native(a,b,n,ctx):0;
 return value==0?std::numeric_limits<I32>::max():value-2;
}
I32 environment_find(std::span<const std::string_view> entries,
                     std::string_view name,Compare native,void* ctx){
 I32 index=0;
 for(const auto entry:entries){
  if(entry.size()>=name.size()&&
     mbsnbicoll(name,entry.substr(0,name.size()),name.size(),native,ctx)==0 &&
     (entry.size()==name.size()||entry[name.size()]=='='))
     return index;
  ++index;
 }
 return -index;
}
std::size_t argv_token_end(std::string_view raw,std::size_t start,
                           LeadByte lead,void* ctx) noexcept {
 if(start>=raw.size())return start;
 std::size_t pos=start;
 if(raw[pos]=='"'){
  ++pos;
  while(pos<raw.size() && raw[pos]!='"'){
   if(lead && lead(static_cast<U8>(raw[pos]),ctx) &&
      pos+1<raw.size())pos+=2;
   else ++pos;
  }
 }else{
  while(pos<raw.size()&&static_cast<U8>(raw[pos])>32)++pos;
 }
 if(pos<raw.size()&&raw[pos]=='"')++pos;
 while(pos<raw.size()&&static_cast<U8>(raw[pos])<=32)++pos;
 return pos;
}
void selection_sort_bytes(std::span<std::byte> bytes,std::size_t item_size,
                          Comparator compare,void* ctx){
 if(item_size==0||!compare||bytes.size()%item_size!=0)return;
 const std::size_t elements=bytes.size()/item_size;
 for(std::size_t n=elements;n>1;--n){
  std::size_t maximum=0;
  for(std::size_t candidate=1;candidate<n;++candidate){
   if(compare(bytes.data()+candidate*item_size,
              bytes.data()+maximum*item_size,ctx)>0)
       maximum=candidate;
  }
  // Internal recovered PE32 00430CFE dependency, not std::sort.
  original_crt17::swap_forward_bytes(
      bytes.subspan(maximum*item_size,item_size),
      bytes.subspan((n-1)*item_size,item_size),item_size);
 }
}
}
