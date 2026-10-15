#include "btb/original_crt_float3.hpp"
#include <cassert>
#include <stdexcept>
#include <string>
#include <unordered_map>
using namespace btb::original_crt_float3;
struct IOHost:IO {
 std::unordered_map<U64,DecimalRecord> decimal{};
 int calls{};
 char separator{'.'};
 DecimalRecord extract_original_decimal(U64 bits) override{
  ++calls;return decimal.at(bits);
 }
 char original_decimal_separator() override{return separator;}
};
int main(){
 {
  bool carry=false;
  const auto r=round_original_digits({false,1,"99995",false},3,&carry);
  assert(carry&&r.decimal_position==2&&r.digits=="100");
  const auto same=round_original_digits({false,1,"24999",false},2,&carry);
  assert(!carry&&same.digits=="25");
  const auto zero=round_original_digits({false,0,"9",false},0,&carry);
  assert(carry&&zero.digits=="1"&&zero.decimal_position==1);
  const auto preserve=round_original_digits({false,1,"123",true},1,&carry);
  assert(preserve.digits=="123"&&!carry);
 }
 IOHost io;
 io.decimal[1]={false,1,"123456",false};
 io.decimal[2]={true,1,"99995",false};
 io.decimal[3]={false,4,"123456",false};
 io.decimal[4]={false,-4,"123456",false};
 io.decimal[5]={false,1,"0",false};
 io.decimal[6]={false,-1,"123456",false};
 {
  auto r=exponent_004332f2(1,2,false,io);
  assert(r.text=="1.23e+000"&&r.scientific);
  r=exponent_004332f2(2,2,true,io);
  assert(r.text=="-1.00E+001"&&r.rounded_carry);
  r=exponent_004332f2(5,3,false,io);
  assert(r.text=="0.000e+000");
 }
 {
  auto r=fixed_004333f6(1,2,io);
  assert(r.text=="1.23"&&!r.scientific);
  r=fixed_004333f6(2,2,io);
  assert(r.text=="-10.00");
  r=fixed_004333f6(3,2,io);
  assert(r.text=="1234.56");
  r=fixed_004333f6(6,3,io);
  assert(r.text=="0.012");
  r=fixed_004333f6(6,0,io);
  assert(r.text=="0");
 }
 {
  auto r=general_004334d4(3,3,false,io);
  assert(r.text=="1.23e+003"&&r.scientific);
  r=general_004334d4(4,4,true,io);
  assert(r.text=="1.234E-005"&&r.scientific);
  r=general_004334d4(1,4,false,io);
  assert(r.text=="1.235"&&!r.scientific);
  r=general_004334d4(2,3,false,io);
  assert(r.text=="-10"&&!r.scientific);
 }
 {
  io.separator=',';
  const auto r=fixed_004333f6(1,2,io);
  assert(r.text=="1,23");
  io.separator='.';
 }
 {
  bool threw=false;
  try{(void)general_004334d4(1,0,false,io);}
  catch(const std::out_of_range&){threw=true;}
  assert(threw);
  threw=false;
  try{(void)fixed_004333f6(1,1025,io);}
  catch(const std::out_of_range&){threw=true;}
  assert(threw);
  assert(io.calls>=11);
 }
}
