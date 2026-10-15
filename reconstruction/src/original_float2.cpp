#include "btb/original_float2.hpp"
#include <bit>
#include <limits>
#include <stdexcept>
namespace btb::original_float2 {
namespace {
U32 pair(const Extended12& w,unsigned i){return U32(w[i])|(U32(w[i+1])<<16);}
void pair(Extended12& w,unsigned i,U32 n){w[i]=static_cast<U16>(n);w[i+1]=static_cast<U16>(n>>16);}
Words96 pack(const Extended12& w){return {pair(w,0),pair(w,2),pair(w,4)};}
void unpack(Extended12& w,const Words96& p){for(unsigned i=0;i<3;++i)pair(w,i*2,p[i]);}
}
void multiply(Extended12& a,const Extended12& b){
 const U16 ea=a[5]&0x7fff,eb=b[5]&0x7fff,sign=(a[5]^b[5])&0x8000;U16 sum=static_cast<U16>(ea+eb);
 const auto infinite=[&]{a={0,0,0,0,0x8000,static_cast<U16>(0x7fff|sign)};};
 if(ea>=0x7fff || eb>=0x7fff || sum>=0xbffe){infinite();return;}
 if(sum<0x3fc0){a={};return;}
 if(!ea){++sum;if(!a[0]&&!a[1]&&!a[2]&&!a[3]&&!a[4]){a[5]=0;return;}}
 if(!eb){++sum;if(!b[0]&&!b[1]&&!b[2]&&!b[3]&&!b[4]){a={};return;}}
 Extended12 product{};
 for(unsigned i=0;i<5;++i)for(unsigned j=0;j<5-i;++j){U32 value;const U32 carry=original_crt32::add32_with_carry(pair(product,i),U32(a[i+j])*b[4-j],value);pair(product,i,value);if(carry)++product[i+2];}
 U16 exponent=static_cast<U16>(sum+0xc002U);
 if(std::bit_cast<std::int16_t>(exponent)>0){while(!(product[5]&0x8000) && std::bit_cast<std::int16_t>(exponent)>0){auto p=pack(product);original_crt32::shift_left_one(p);unpack(product,p);--exponent;}}
 if(std::bit_cast<std::int16_t>(exponent)<1){--exponent;if(std::bit_cast<std::int16_t>(exponent)<0){const I32 shifts=-std::bit_cast<std::int16_t>(exponent);exponent=0;bool sticky=false;for(I32 i=0;i<shifts;++i){sticky|=(product[0]&1)!=0;auto p=pack(product);original_crt32::shift_right_one(p);unpack(product,p);}if(sticky)product[0]|=1;}}
 if(product[0]>0x8000 || (pair(product,0)&0x1ffffU)==0x18000U){U32 carry=1;for(unsigned i=1;i<6 && carry;++i){const U32 value=U32(product[i])+carry;product[i]=static_cast<U16>(value);carry=value>>16;}if(carry){++exponent;product[5]=0x8000;}}
 if(exponent>=0x7fff){infinite();return;}for(unsigned i=0;i<5;++i)a[i]=product[i+1];a[5]=exponent|sign;
}
void scale_decimal(Extended12& value,I32 requested,bool retain_guard){
 if(!requested)return;if(requested==std::numeric_limits<I32>::min())throw std::out_of_range("original decimal exponent would not terminate");
 U32 power=static_cast<U32>(requested<0?-requested:requested),offset=requested<0?0x160:0;
 if(!retain_guard)value[0]=0;
 while(power){offset+=0x54;const U32 digit=power&7;power>>=3;if(!digit)continue;const U32 at=offset+digit*12;
  if(at+12>power_bytes.size())throw std::out_of_range("original decimal power table");Extended12 factor;for(unsigned i=0;i<6;++i)factor[i]=static_cast<U16>(power_bytes[at+i*2]|(U16(power_bytes[at+i*2+1])<<8));
  if(factor[0]>=0x8000)pair(factor,1,pair(factor,1)-1);multiply(value,factor);
 }
}
}
