#include "btb/original_stdio2.hpp"
#include <cassert>
#include <cmath>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>
using namespace btb::original_stdio2;
struct PrintIO final:PrintfIO{
 int floats{},wide{},counts{};U64 ptr{};I32 stored{};
 std::string original_float_to_string(long double n,char conv,I32 precision,bool alt) override{
  ++floats;assert(conv=='f'&&precision==2&&!alt);assert(n==1.25L);
  return "1.25"; // real native 004333F6 provider in game
 }
 std::string original_wide_string(U64 handle,I32 precision) override{
  ++wide;assert(handle==1234&&precision==3);return "HELLO";
 }
 void store_output_count(U64 handle,I32 count) override{
  ++counts;ptr=handle;stored=count;
 }
};
struct ScanIO final:ScanfIO{
 std::vector<std::size_t> indices{};
 std::vector<ScanValue> commits{};
 int float_reads{};
 long double original_parse_float(std::string_view token,bool& success) override{
  ++float_reads;
  if(token=="1.25e+2"){success=true;return 125.L;}
  if(token=="-0.5"){success=true;return -0.5L;}
  success=false;return 0;
 }
 void commit_original_assignment(std::size_t idx,const ScanValue& value) override{
  indices.push_back(idx);commits.push_back(value);
 }
};
int main(){
 PrintIO p;
 {
  const auto out=printf_00432971("N=%+08d X=%#06X O=%#o",
   {{std::int64_t(42)},{U64(0x2a)},{U64(8)}},p);
  assert(out.text=="N=+0000042 X=0X002A O=010");
  assert(out.written==static_cast<I32>(out.text.size())&&out.consumed_args==3);
 }
 {
  const auto out=printf_00432971("%-6.3s:%5c:%.*s",
   {{std::string("ORIGINAL")},{U64('Z')},{std::int64_t(2)},{std::string("ABCD")}},p);
  assert(out.text=="ORI   :    Z:AB"&&out.consumed_args==4);
 }
 {
  const auto out=printf_00432971("%08.4d:%p:%.0u",
   {{std::int64_t(-12)},{U64(0x42)},{U64(0)}},p);
  assert(out.text=="   -0012:0x42:");
 }
 {
  const auto out=printf_00432971("%.3S:%7.2f:%n!",
   {{U64(1234)},{1.25L},{U64(9876)}},p);
  assert(out.text=="HEL:   1.25:!"&&p.wide==1&&p.floats==1);
  assert(p.counts==1&&p.ptr==9876&&p.stored==static_cast<I32>(out.text.size()-1));
 }
 {
  const auto out=printf_00432971("%% %d",{{std::int64_t(-1)}},p);
  assert(out.text=="% -1");
  bool fail=false;
  try{(void)printf_00432971("%q",{},p);}
  catch(const std::invalid_argument&){fail=true;}
  assert(fail);
 }
 ScanIO sc;
 {
  const auto result=scanf_00431314("  -12 0x2f 77 abc",
     "%d %i %u %3s",sc);
  assert(result.assignments==4&&result.completed_directives==4);
  assert(result.values[0].signed_value==-12);
  assert(result.values[1].signed_value==47);
  assert(result.values[2].unsigned_value==77);
  assert(result.values[3].bytes=="abc");
  assert(result.consumed==17);
 }
 {
  sc.indices.clear();sc.commits.clear();
  const auto result=scanf_00431314("apple123:Z",
     "%[a-z]%3d:%c%n",sc);
  assert(result.assignments==3);
  assert(result.values[0].bytes=="apple");
  assert(result.values[1].signed_value==123);
  assert(result.values[2].bytes=="Z");
  assert(result.values[3].kind==ScanValue::Kind::Count);
  assert(result.values[3].signed_value==10);
 }
 {
  sc.indices.clear();sc.commits.clear();
  const auto result=scanf_00431314("skip,XYZ,-0.5",
     "%*[^,],%3c,%f",sc);
  assert(result.assignments==2&&result.values.size()==2);
  assert(result.values[0].bytes=="XYZ");
  assert(result.values[1].floating_value==-0.5L);
 }
 {
  sc.indices.clear();sc.commits.clear();
  const auto result=scanf_00431314("1.25e+2 255",
     "%f %x",sc);
  assert(result.assignments==2&&result.values[0].floating_value==125.L);
  assert(result.values[1].unsigned_value==0x255);
 }
 {
  sc.indices.clear();sc.commits.clear();
  const auto r=scanf_00431314("","%d",sc);
  assert(r.assignments==-1&&r.input_failure);
  const auto partial=scanf_00431314("123!","%d,%d",sc);
  assert(partial.assignments==1&&partial.format_failure);
  bool malformed=false;
  try{(void)scanf_00431314("abc","%[abc",sc);}
  catch(const std::invalid_argument&){malformed=true;}
  assert(malformed);
 }
}
