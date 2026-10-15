#include "btb/original_stdio2.hpp"
#include <algorithm>
#include <array>
#include <charconv>
#include <cctype>
#include <cstddef>
#include <limits>
#include <stdexcept>
#include <string>
#include <utility>
namespace btb::original_stdio2 {
namespace {
bool digit(char c){return c>='0'&&c<='9';}
bool space(char c){return c==' '||c=='\t'||c=='\r'||c=='\n'||c=='\f'||c=='\v';}
int digit_value(char c){
 if(c>='0'&&c<='9')return c-'0';
 if(c>='a'&&c<='f')return 10+c-'a';
 if(c>='A'&&c<='F')return 10+c-'A';
 return -1;
}
I32 decimal_number(std::string_view s,std::size_t& at){
 I32 v=0;
 while(at<s.size()&&digit(s[at])){
  if(v>100000)throw std::out_of_range("original CRT precision/width too large");
  v=v*10+(s[at++]-'0');
 }
 return v;
}
std::string unsigned_digits(U64 n,int base,bool uppercase){
 std::string out;
 const char* alphabet=uppercase?"0123456789ABCDEF":"0123456789abcdef";
 do {out.push_back(alphabet[n%static_cast<U64>(base)]);n/=static_cast<U64>(base);}while(n!=0);
 std::reverse(out.begin(),out.end());return out;
}
std::int64_t as_signed(const PrintfArg& arg){
 if(auto p=std::get_if<std::int64_t>(&arg.value))return *p;
 if(auto p=std::get_if<U64>(&arg.value))return static_cast<std::int64_t>(*p);
 throw std::invalid_argument("original printf integer argument kind mismatch");
}
U64 as_unsigned(const PrintfArg& arg){
 if(auto p=std::get_if<U64>(&arg.value))return *p;
 if(auto p=std::get_if<std::int64_t>(&arg.value))return static_cast<U64>(*p);
 throw std::invalid_argument("original printf unsigned argument kind mismatch");
}
const PrintfArg& next(const std::vector<PrintfArg>& args,std::size_t& cursor){
 if(cursor>=args.size())throw std::out_of_range("original printf vararg exhausted");
 return args[cursor++];
}
std::string justify(std::string text,I32 width,bool left,bool pad_zero,
                    std::size_t sign_and_prefix){
 if(width<0||width>100000)throw std::out_of_range("printf original field width");
 if(text.size()>=static_cast<std::size_t>(width))return text;
 const std::size_t amount=static_cast<std::size_t>(width)-text.size();
 if(left){text.append(amount,' ');return text;}
 if(pad_zero){
  sign_and_prefix=std::min(sign_and_prefix,text.size());
  text.insert(sign_and_prefix,amount,'0');return text;
 }
 text.insert(0,amount,' ');return text;
}
struct Spec {
 bool left{},plus{},blank{},alternative{},zero{};
 I32 width{},precision{-1};
 char length{},conv{};
};
Spec directive(std::string_view fmt,std::size_t& pos,
               const std::vector<PrintfArg>& args,std::size_t& consumed){
 Spec s;
 for(;pos<fmt.size();++pos){
  const char c=fmt[pos];
  if(c=='-')s.left=true;
  else if(c=='+')s.plus=true;
  else if(c==' ')s.blank=true;
  else if(c=='#')s.alternative=true;
  else if(c=='0')s.zero=true;
  else break;
 }
 if(pos<fmt.size()&&fmt[pos]=='*'){
  const auto width=as_signed(next(args,consumed));++pos;
  if(width<std::numeric_limits<I32>::min()||width>std::numeric_limits<I32>::max())
   throw std::out_of_range("original width vararg");
  s.width=static_cast<I32>(width);
  if(s.width<0){s.left=true;s.width=-s.width;}
 }else s.width=decimal_number(fmt,pos);
 if(pos<fmt.size()&&fmt[pos]=='.'){
  ++pos;s.precision=0;
  if(pos<fmt.size()&&fmt[pos]=='*'){
   const auto p=as_signed(next(args,consumed));++pos;
   if(p>=0){
    if(p>100000)throw std::out_of_range("precision vararg");
    s.precision=static_cast<I32>(p);
   }else s.precision=-1;
  }else s.precision=decimal_number(fmt,pos);
 }
 if(pos<fmt.size()&&(fmt[pos]=='h'||fmt[pos]=='l'||fmt[pos]=='L'||
                     fmt[pos]=='I'||fmt[pos]=='z'||fmt[pos]=='t')){
  s.length=fmt[pos++];
  if(pos<fmt.size()&&fmt[pos]==s.length&&(s.length=='h'||s.length=='l'))++pos;
  if(s.length=='I'&&fmt.substr(pos,2)=="64")pos+=2;
 }
 if(pos>=fmt.size())throw std::invalid_argument("unterminated source printf directive");
 s.conv=fmt[pos++];
 return s;
}
void validate_output(std::size_t n){
 if(n>static_cast<std::size_t>(std::numeric_limits<I32>::max()))
  throw std::length_error("source printf byte count exceeds INT32_MAX");
}
std::string numeric(const PrintfArg& arg,const Spec& s,std::size_t& prefixlen){
 const bool signed_conv=s.conv=='d'||s.conv=='i';
 int base=10;
 if(s.conv=='x'||s.conv=='X'||s.conv=='p')base=16;
 if(s.conv=='o')base=8;
 bool neg=false;
 U64 n;
 if(signed_conv){
  const auto v=as_signed(arg);
  neg=v<0;
  n=neg?0U-static_cast<U64>(v):static_cast<U64>(v);
 }else n=as_unsigned(arg);
 std::string chars=(s.precision==0&&n==0)?"":unsigned_digits(n,base,s.conv=='X');
 if(s.precision>0&&chars.size()<static_cast<std::size_t>(s.precision))
  chars.insert(0,static_cast<std::size_t>(s.precision)-chars.size(),'0');
 std::string prefix;
 if(neg)prefix="-";
 else if(signed_conv&&s.plus)prefix="+";
 else if(signed_conv&&s.blank)prefix=" ";
 if(s.conv=='p')prefix+="0x";
 else if(s.alternative&&n!=0){
  if(s.conv=='x')prefix+="0x";
  else if(s.conv=='X')prefix+="0X";
  else if(s.conv=='o'&&(chars.empty()||chars.front()!='0'))prefix+="0";
 }
 prefixlen=prefix.size();
 return prefix+chars;
}
struct ScanDirective {
 bool suppress{},invert{};
 I32 width{},length_width{};
 char conversion{};
 std::array<bool,256> scan_set{};
};
ScanDirective scan_directive(std::string_view fmt,std::size_t& p){
 ScanDirective out{};
 if(p<fmt.size()&&fmt[p]=='*'){out.suppress=true;++p;}
 if(p<fmt.size()&&digit(fmt[p]))out.width=decimal_number(fmt,p);
 else out.width=std::numeric_limits<I32>::max();
 if(p<fmt.size()&&(fmt[p]=='h'||fmt[p]=='l'||fmt[p]=='L'||fmt[p]=='I')){
  const char length=fmt[p++];out.length_width=length=='h'?16:length=='l'?32:64;
  if(p<fmt.size()&&fmt[p]==length&&(length=='h'||length=='l')){out.length_width=length=='h'?8:64;++p;}
  if(length=='I'&&fmt.substr(p,2)=="64"){p+=2;out.length_width=64;}
 }
 if(p>=fmt.size())throw std::invalid_argument("original scanf incomplete directive");
 out.conversion=fmt[p++];
 if(out.conversion=='['){
  if(p<fmt.size()&&fmt[p]=='^'){out.invert=true;++p;}
  if(p<fmt.size()&&fmt[p]==']'){out.scan_set[static_cast<unsigned char>(']')]=true;++p;}
  while(p<fmt.size()&&fmt[p]!=']'){
   const unsigned char c=static_cast<unsigned char>(fmt[p++]);
   if(p+1<fmt.size()&&fmt[p]=='-'&&fmt[p+1]!=']'){
    ++p;const unsigned char high=static_cast<unsigned char>(fmt[p++]);
    for(unsigned v=std::min<unsigned>(c,high);v<=std::max<unsigned>(c,high);++v)
     out.scan_set[v]=true;
   }else out.scan_set[c]=true;
  }
  if(p>=fmt.size()||fmt[p]!=']')throw std::invalid_argument("original scanf unclosed scanset");
  ++p;
 }
 return out;
}
bool accept_scan_byte(const ScanDirective& d,char ch){
 bool in=d.scan_set[static_cast<unsigned char>(ch)];
 return d.invert?!in:in;
}
bool integer_token(std::string_view in,std::size_t& cursor,std::size_t limit,
                   char conv,ScanValue& v){
 std::size_t p=cursor;
 bool neg=false;
 if(p<limit&&(in[p]=='+'||in[p]=='-')){neg=in[p]=='-';++p;}
 int base=10;
 if(conv=='x'||conv=='X'||conv=='p')base=16;
 if(conv=='o')base=8;
 if(conv=='i')base=0;
 if(p<limit&&in[p]=='0'){
  if(p+1<limit&&(in[p+1]=='x'||in[p+1]=='X')&&(base==0||base==16)){
   base=16;p+=2;
  }else if(base==0)base=8;
 }
 if(base==0)base=10;
 const std::size_t first=p;
 U64 n=0;bool overflow=false;
 while(p<limit){
  const int digitval=digit_value(in[p]);
  if(digitval<0||digitval>=base)break;
  const U64 digit64=static_cast<U64>(digitval);
  if(n>(std::numeric_limits<U64>::max()-digit64)/static_cast<U64>(base))
   overflow=true;
  else n=n*static_cast<U64>(base)+digit64;
  ++p;
 }
 if(p==first)return false;
 if(overflow)throw std::out_of_range("original scanf integer overflow");
 cursor=p;
 if(conv=='d'||conv=='i'){
  v.kind=ScanValue::Kind::Signed;
  v.signed_value=static_cast<std::int64_t>(neg?0U-n:n);
 }else{
  v.kind=ScanValue::Kind::Unsigned;
  v.unsigned_value=neg?0U-n:n;
 }
 return true;
}
bool floating_token(std::string_view in,std::size_t& cursor,std::size_t limit,
                    ScanValue& v,ScanfIO& io){
 std::size_t p=cursor;
 if(p<limit&&(in[p]=='-'||in[p]=='+'))++p;
 bool any=false;
 while(p<limit&&digit(in[p])){++p;any=true;}
 if(p<limit&&in[p]=='.'){
  ++p;while(p<limit&&digit(in[p])){++p;any=true;}
 }
 if(!any)return false;
 const std::size_t save=p;
 if(p<limit&&(in[p]=='e'||in[p]=='E'||in[p]=='d'||in[p]=='D')){
  ++p;
  if(p<limit&&(in[p]=='+'||in[p]=='-'))++p;
  const std::size_t start=p;
  while(p<limit&&digit(in[p]))++p;
  if(p==start)p=save;
 }
 bool success=false;
 v.kind=ScanValue::Kind::Float;
 v.floating_value=io.original_parse_float(in.substr(cursor,p-cursor),success);
 if(success)cursor=p;
 return success;
}
}
PrintfResult printf_00432971(std::string_view fmt,
                             const std::vector<PrintfArg>& args,PrintfIO& io){
 PrintfResult out{};
 std::size_t p=0,arg=0;
 while(p<fmt.size()){
  if(fmt[p]!='%'){out.text.push_back(fmt[p++]);continue;}
  ++p;
  if(p<fmt.size()&&fmt[p]=='%'){out.text.push_back('%');++p;continue;}
  const Spec s=directive(fmt,p,args,arg);
  std::string value;std::size_t prefix=0;bool use_zero=s.zero&&!s.left;
  if(s.conv=='d'||s.conv=='i'||s.conv=='u'||s.conv=='o'||
     s.conv=='x'||s.conv=='X'||s.conv=='p'){
   value=numeric(next(args,arg),s,prefix);
   if(s.precision>=0)use_zero=false;
  }else if(s.conv=='c'||s.conv=='C'){
   const auto& a=next(args,arg);
   value=std::string(1,static_cast<char>(as_unsigned(a)&0xffU));
   use_zero=false;
  }else if(s.conv=='s'||s.conv=='S'){
   const auto& a=next(args,arg);
   if(auto v=std::get_if<std::string>(&a.value))value=*v;
   else value=io.original_wide_string(as_unsigned(a),s.precision);
   if(s.precision>=0&&value.size()>static_cast<std::size_t>(s.precision))
    value.resize(static_cast<std::size_t>(s.precision));
   use_zero=false;
  }else if(s.conv=='n'){
   io.store_output_count(as_unsigned(next(args,arg)),static_cast<I32>(out.text.size()));
   continue;
  }else if(s.conv=='f'||s.conv=='F'||s.conv=='e'||s.conv=='E'||
           s.conv=='g'||s.conv=='G'){
   const auto& a=next(args,arg);
   if(auto v=std::get_if<long double>(&a.value))
    value=io.original_float_to_string(*v,s.conv,s.precision>=0?s.precision:6,s.alternative);
   else throw std::invalid_argument("original printf float vararg mismatch");
   if(!value.empty()&&value[0]!='-'){
    if(s.plus)value.insert(value.begin(),'+');
    else if(s.blank)value.insert(value.begin(),' ');
   }
   if(!value.empty()&&(value[0]=='+'||value[0]=='-'||value[0]==' '))prefix=1;
  }else{
   throw std::invalid_argument("unsupported original printf conversion");
  }
  out.text+=justify(std::move(value),s.width,s.left,use_zero,prefix);
  validate_output(out.text.size());
 }
 validate_output(out.text.size());
 out.written=static_cast<I32>(out.text.size());
 out.consumed_args=static_cast<I32>(arg);
 return out;
}
ScanResult scanf_00431314(std::string_view input,std::string_view fmt,ScanfIO& io){
 ScanResult r{};
 std::size_t pos=0,fp=0,dest=0;
 bool attempted_input=false;
 while(fp<fmt.size()){
  if(space(fmt[fp])){
   while(fp<fmt.size()&&space(fmt[fp]))++fp;
   while(pos<input.size()&&space(input[pos]))++pos;
   continue;
  }
  if(fmt[fp]!='%'){
   if(pos>=input.size()){r.input_failure=true;break;}
   if(input[pos]!=fmt[fp]){r.format_failure=true;break;}
   ++pos;++fp;continue;
  }
  ++fp;
  if(fp<fmt.size()&&fmt[fp]=='%'){
   if(pos>=input.size()){r.input_failure=true;break;}
   if(input[pos]!='%'){r.format_failure=true;break;}
   ++fp;++pos;continue;
  }
  const ScanDirective d=scan_directive(fmt,fp);
  ++r.attempted_directives;
  const char conv=d.conversion;
  if(conv=='n'){
   if(!d.suppress){
    ScanValue v{};v.kind=ScanValue::Kind::Count;v.signed_value=static_cast<std::int64_t>(pos);
    io.commit_original_assignment(dest++,v);r.values.push_back(v);
   }
   ++r.completed_directives;continue;
  }
  if(conv!='c'&&conv!='['){
   while(pos<input.size()&&space(input[pos]))++pos;
  }
  if(pos==input.size()){
   r.input_failure=true;break;
  }
  const std::size_t limit=std::min(input.size(),
   pos+static_cast<std::size_t>(d.width));
  ScanValue v{};bool ok=false;
  if(conv=='d'||conv=='i'||conv=='u'||conv=='o'||conv=='x'||
     conv=='X'||conv=='p'){
   ok=integer_token(input,pos,limit,conv,v);
  }else if(conv=='f'||conv=='F'||conv=='e'||conv=='E'||conv=='g'||conv=='G'){
   ok=floating_token(input,pos,limit,v,io);
  }else if(conv=='c'){
   const auto n=d.width==std::numeric_limits<I32>::max()?1U:static_cast<std::size_t>(d.width);
   if(pos+n<=input.size()){
    v.kind=ScanValue::Kind::Bytes;v.bytes=std::string(input.substr(pos,n));
    pos+=n;ok=true;
   }
  }else if(conv=='s'||conv=='['){
   const std::size_t before=pos;
   if(conv=='s')while(pos<limit&&!space(input[pos]))++pos;
   else while(pos<limit&&accept_scan_byte(d,input[pos]))++pos;
   if(pos>before){
    v.kind=ScanValue::Kind::Bytes;v.bytes=std::string(input.substr(before,pos-before));
    ok=true;
   }
  }else throw std::invalid_argument("unsupported original scanf conversion");
  attempted_input=true;
  if(!ok){r.format_failure=true;break;}
  ++r.completed_directives;
  if(!d.suppress){
   io.commit_original_assignment(dest++,v);
   r.values.push_back(v);
   ++r.assignments;
  }
 }
 r.consumed=static_cast<I32>(pos);
 if(r.input_failure&&r.assignments==0&&attempted_input==false)r.assignments=-1;
 return r;
}
} // namespace btb::original_stdio2
