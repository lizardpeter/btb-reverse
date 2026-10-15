#include "btb/original_crt_float3.hpp"
#include <algorithm>
#include <cstddef>
#include <limits>
#include <stdexcept>
#include <string>
namespace btb::original_crt_float3 {
namespace {
void validate(const DecimalRecord& r,I32 precision){
 if(precision<0||precision>1024)
  throw std::out_of_range("original CRT precision out of bounds");
 if(r.decimal_position < -100000||r.decimal_position>100000)
  throw std::out_of_range("original CRT decimal position out of bounds");
 if(r.digits.empty())throw std::invalid_argument("original CRT decimal digits empty");
 for(const char c:r.digits)if(c<'0'||c>'9')
  throw std::invalid_argument("original CRT decimal buffer not numeric");
}
bool zero(const DecimalRecord& r){
 return std::all_of(r.digits.begin(),r.digits.end(),[](char c){return c=='0';});
}
std::string padded_digits(std::string_view digits,std::size_t size){
 std::string result(digits.substr(0,size));
 if(result.size()<size)result.append(size-result.size(),'0');
 return result;
}
std::string exponent_digits(I32 exponent){
 const bool negative=exponent<0;
 const U32 magnitude=static_cast<U32>(negative?-static_cast<std::int64_t>(exponent):exponent);
 std::string d=std::to_string(magnitude);
 if(d.size()<3)d.insert(d.begin(),3-d.size(),'0');
 return std::string(1,negative?'-':'+')+d;
}
FormatResult exponent_from_record(DecimalRecord source,I32 places,bool uppercase,char separator){
 validate(source,places);
 bool carry=false;
 const bool is_zero=zero(source);
 auto r=round_original_digits(std::move(source),places+1,&carry);
 const I32 e=is_zero?0:r.decimal_position-1;
 std::string digits=padded_digits(r.digits,static_cast<std::size_t>(places+1));
 std::string text;
 if(r.negative)text.push_back('-');
 text.push_back(digits.front());
 if(places>0){
  text.push_back(separator);
  text.append(digits.begin()+1,digits.end());
 }
 text.push_back(uppercase?'E':'e');
 text+=exponent_digits(e);
 return {std::move(text),r.decimal_position,carry,true};
}
FormatResult fixed_from_record(DecimalRecord r,I32 places,char separator){
 validate(r,places);
 const I32 significant=r.decimal_position+places;
 bool carry=false;
 if(significant>=0)
  r=round_original_digits(std::move(r),significant,&carry);
 else {
  r.digits="0";
  r.decimal_position=1;
 }
 const auto position=r.decimal_position;
 std::string text;
 if(r.negative)text.push_back('-');
 if(position<=0) {
  text.push_back('0');
  if(places>0){
   text.push_back(separator);
   const I32 prefix=std::min(places,-position);
   text.append(static_cast<std::size_t>(prefix),'0');
   const std::size_t remaining=static_cast<std::size_t>(places-prefix);
   text+=padded_digits(r.digits,remaining);
  }
 } else {
  text+=padded_digits(r.digits,static_cast<std::size_t>(position));
  if(places>0){
   text.push_back(separator);
   const std::size_t from=static_cast<std::size_t>(position);
   const std::string_view tail=r.digits.size()>from
     ? std::string_view(r.digits).substr(from)
     : std::string_view{};
   text+=padded_digits(tail,static_cast<std::size_t>(places));
  }
 }
 return {std::move(text),r.decimal_position,carry,false};
}
void remove_insignificant_zeros(std::string& s,char sep){
 const auto pos=s.find(sep);
 if(pos==std::string::npos)return;
 const auto exponent=s.find_first_of("eE");
 const std::size_t end=exponent==std::string::npos?s.size():exponent;
 std::size_t last=end;
 while(last>pos+1&&s[last-1]=='0')--last;
 if(last==pos+1)--last;
 s.erase(last,end-last);
}
}
DecimalRecord round_original_digits(DecimalRecord r,I32 length,bool* carry){
 validate(r,std::max(0,length));
 if(length>4096)throw std::out_of_range("original CRT significant length too large");
 bool did_carry=false;
 if(r.special){
  if(carry)*carry=false;
  return r;
 }
 if(length<0) {
  r.digits="0";
  if(carry)*carry=false;
  return r;
 }
 const std::size_t needed=static_cast<std::size_t>(length);
 const bool round_up=r.digits.size()>needed&&r.digits[needed]>'4';
 std::string output=padded_digits(r.digits,needed);
 if(round_up){
  if(needed==0){
   output="1"; ++r.decimal_position;did_carry=true;
  }else{
   std::size_t cursor=needed;
   while(cursor>0&&output[cursor-1]=='9'){
    output[cursor-1]='0';--cursor;
   }
   if(cursor==0){
    output[0]='1';
    ++r.decimal_position;
    did_carry=true;
   }else{
    ++output[cursor-1];
   }
  }
 }
 if(output.empty())output="0";
 r.digits=std::move(output);
 if(carry)*carry=did_carry;
 return r;
}
FormatResult exponent_004332f2(U64 raw,I32 precision,bool uppercase,IO& io){
 auto d=io.extract_original_decimal(raw);
 return exponent_from_record(std::move(d),precision,uppercase,io.original_decimal_separator());
}
FormatResult fixed_004333f6(U64 raw,I32 precision,IO& io){
 auto d=io.extract_original_decimal(raw);
 return fixed_from_record(std::move(d),precision,io.original_decimal_separator());
}
FormatResult general_004334d4(U64 raw,I32 precision,bool uppercase,IO& io){
 if(precision<1||precision>1024)throw std::out_of_range("general precision");
 auto d=io.extract_original_decimal(raw);
 validate(d,precision);
 const I32 exp=d.decimal_position-1;
 FormatResult result;
 // Branch condition recovered verbatim in 004334D4:
 // (decimal_position-1 < -4) || (precision <= decimal_position-1).
 if(exp< -4 || precision<=exp) {
  result=exponent_from_record(std::move(d),precision-1,uppercase,io.original_decimal_separator());
 }else{
  const I32 places=std::max<I32>(0,precision-d.decimal_position);
  result=fixed_from_record(std::move(d),places,io.original_decimal_separator());
 }
 // General formatting trims excess fractional zeros. The exact retail
 // general-path DAT_0051C43C underflow/rounding flag remains unverified.
 remove_insignificant_zeros(result.text,io.original_decimal_separator());
 return result;
}
} // namespace btb::original_crt_float3
