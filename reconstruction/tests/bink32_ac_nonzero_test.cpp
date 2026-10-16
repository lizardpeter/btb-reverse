#include "btb/bink32_ac_nonzero.hpp"
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <span>
using namespace btb::bink32_recovered;
#define CHECK(x) do{if(!(x)){std::fprintf(stderr,"Bink AC branch test line %d: %s\n",__LINE__,#x);std::abort();}}while(0)
struct Bits {std::array<std::uint8_t,8> bytes{};unsigned count{};
 void write(unsigned value,unsigned width){for(unsigned i=0;i<width;++i){
  if((value>>i)&1U)bytes[count/8]|=std::uint8_t(1U<<(count%8));
  ++count;
 }}
 LsbBitReader reader()const{return LsbBitReader(std::span<const std::uint8_t>(bytes).first((count+7)/8));}
};
int main() {
 // Subcommand type 1: low two bits 01, upper six bits retained;
 // original 0x30011C61 appends three more template commands.
 const auto four=expand_bink_ac_control1(0x25);
 CHECK(four&&(*four==std::array<std::uint8_t,4>{0x26,0x36,0x46,0x56}));
 CHECK(!expand_bink_ac_control1(0x27));
 const auto zero=expand_bink_ac_control1(0x01);
 CHECK(zero&&(*zero==std::array<std::uint8_t,4>{2,0x12,0x22,0x32}));
 const auto wrap=expand_bink_ac_control1(0xfd);
 CHECK(wrap&&(*wrap==std::array<std::uint8_t,4>{0xfe,0x0e,0x1e,0x2e}));

 for(unsigned width=0;width<=14;++width){
  const unsigned high=1U<<width;
  const auto low=(1U<<width)-1U;
  for(unsigned neg=0;neg<2;++neg){
   Bits encoded;encoded.write(low,width);encoded.write(neg,1);
   auto bits=encoded.reader();
   std::array<std::int16_t,64> coeff{};coeff.fill(99);
   std::uint8_t code=static_cast<std::uint8_t>((37<<2)|3U);
   const auto result=decode_bink_ac_direct3(bits,code,width,static_cast<std::uint16_t>(high),coeff);
   const auto expected=neg?-static_cast<int>(high+low):static_cast<int>(high+low);
   CHECK(result&&result->slot==37&&result->value==expected);
   CHECK(result->low_magnitude_bits==width&&result->consumed_bits==width+1);
   CHECK(coeff[37]==expected&&coeff[0]==99&&coeff[63]==99&&code==0);
   CHECK(bits.bit_position()==width+1);
  }
 }
 {
  Bits encoded;encoded.write(6,4);encoded.write(1,1);
  auto bits=encoded.reader();
  std::array<std::int16_t,64> coeff{};coeff.fill(29);
  std::uint8_t cmd=3; // slot 0 direct, high class bit 16
  auto item=decode_bink_ac_direct3(bits,cmd,4,16,coeff);
  CHECK(item&&item->value==-22&&coeff[0]==-22);
 }
 {
  Bits encoded;encoded.write(1,1);
  auto bits=encoded.reader();
  std::array<std::int16_t,64> coeff{};coeff.fill(13);
  std::uint8_t cmd=(63<<2)|3U;
  CHECK(!decode_bink_ac_direct3(bits,cmd,8,256,coeff));
  CHECK(bits.bit_position()==0&&coeff[63]==13&&cmd==255);
  CHECK(!decode_bink_ac_direct3(bits,cmd,15,16384,coeff));
  CHECK(!decode_bink_ac_direct3(bits,cmd,2,3,coeff));
  CHECK(!decode_bink_ac_direct3(bits,cmd,14,4096,coeff));
  CHECK(!decode_bink_ac_direct3(bits,cmd,32,32768,coeff));
  CHECK(!decode_bink_ac_direct3(bits,cmd,0,1,std::span<std::int16_t>(coeff).first(63)));
  cmd=5;
  CHECK(!decode_bink_ac_direct3(bits,cmd,0,1,coeff));
  CHECK(bits.bit_position()==0&&cmd==5);
 }
 {
  // Actual 30011BF4 makes EAX = (1 << (mode-1))-1 for the
  // outer AC coefficient pass. Mode 4 has EAX=7 and class bit=8;
  // the previous fake restriction (1 << 7)<=8 rejected it.
  Bits encoded;encoded.write(0x52,7);encoded.write(1,1);
  auto bits=encoded.reader();
  std::array<std::int16_t,64> coeff{};
  std::uint8_t command=(12U<<2)|3U;
  const auto v=decode_bink_ac_direct3(bits,command,7,8,coeff);
  CHECK(v&&v->value==-90&&v->consumed_bits==8);
  CHECK(coeff[12]==-90&&command==0&&bits.bit_position()==8);
 }
 {
  // Original MOVSX EDI, DI truncates a 16-plus-bit magnitude before
  // NEG and MOV WORD. Verify both sign choices on 0xffff low word.
  for(unsigned negative=0;negative<=1;++negative){
   Bits encoded;encoded.write(0xffff,16);encoded.write(negative,1);
   auto bits=encoded.reader();
   std::array<std::int16_t,64> coeff{};
   std::uint8_t command=3;
   const auto v=decode_bink_ac_direct3(bits,command,16,1,coeff);
   CHECK(v&&v->value==(negative?1:-1));
   CHECK(coeff[0]==(negative?1:-1)&&bits.bit_position()==17);
  }
 }
 std::puts("Bink nonzero AC direct coefficient branch and control expansion: PASS");
}
