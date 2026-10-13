#include "btb/original_runtime5.hpp"
#include <cstdint>
namespace btb::original_runtime5 {
std::size_t retail_strlen(const char* s) noexcept {
 std::size_t n=0;while(s[n]!=0)++n;return n;
}
I32 retail_strcmp(const char* a,const char* b) noexcept {
 for(std::size_t i=0;;++i){
  const auto x=static_cast<unsigned char>(a[i]);
  const auto y=static_cast<unsigned char>(b[i]);
  if(x!=y)return x<y?-1:1;
  if(x==0)return 0;
 }
}
const char* retail_strstr(const char* haystack,const char* needle) noexcept {
 if(needle[0]==0)return haystack;
 for(std::size_t i=0;haystack[i]!=0;++i){
  std::size_t j=0;
  while(needle[j]!=0&&haystack[i+j]!=0&&needle[j]==haystack[i+j])++j;
  if(needle[j]==0)return haystack+i;
 }
 return nullptr;
}
void retail_write_radix(U32 value,char* output,U32 radix,bool negative) noexcept {
 // The original 00430F5C receives a valid 2..36 radix from the caller.
 // Invalid arguments are trapped rather than dividing by zero in the host.
 if(!output)return;
 if(radix<2||radix>36){output[0]=0;return;}
 std::size_t out=0;
 if(negative){output[out++]='-';value=0U-value;}
 const std::size_t start=out;
 do{
  const U32 digit=value%radix;
  output[out++]=static_cast<char>(digit<10?('0'+digit):('W'+digit));
  value/=radix;
 }while(value);
 output[out]=0;
 for(std::size_t left=start,right=out-1;left<right;++left,--right){
  const char temp=output[left];output[left]=output[right];output[right]=temp;
 }
}
U32 translate_x87_control(U32 flags) noexcept {
 U32 out=0;
 if(flags&1)out|=0x10;
 if(flags&4)out|=8;
 if(flags&8)out|=4;
 if(flags&0x10)out|=2;
 if(flags&0x20)out|=1;
 if(flags&2)out|=0x80000;
 const U32 rounded=flags&0xc00;
 if(rounded==0x400)out|=0x100;
 if(rounded==0x800)out|=0x200;
 if(rounded==0xc00)out|=0x300;
 if((flags&0x300)==0)out|=0x20000;
 else if((flags&0x300)==0x200)out|=0x10000;
 if(flags&0x1000)out|=0x40000;
 return out;
}
} // namespace btb::original_runtime5
