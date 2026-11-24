#pragma once
#include "btb/original_environment4.hpp"
#include "btb/original_runtime15.hpp"
#include "btb/original_locale7.hpp"
namespace btb::original_system2 {
using U32=std::uint32_t;using I32=std::int32_t;
struct ZoneInfo {
 I32 bias{};std::array<char16_t,32> standard_name{};original_runtime15::SystemTime standard_date{};I32 standard_bias{};
 std::array<char16_t,32> daylight_name{};original_runtime15::SystemTime daylight_date{};I32 daylight_bias{};
};
static_assert(sizeof(ZoneInfo)==172);
struct ZoneState {original_runtime15::TimeState& time;ZoneInfo info{};char* saved{};std::array<char,64> standard{},daylight{};U32 codepage{};};
class Services {
public:
 virtual ~Services()=default;
 virtual U32 zone_information(ZoneInfo&)=0;
 virtual U32 convert_name(U32 codepage,U32 flags,const char16_t*,char*,U32,bool& used_default)=0;
 virtual bool set_directory(const char*)=0;
 virtual U32 get_directory(U32,char*)=0;
 virtual bool set_variable(const char*,const char*)=0;
 virtual U32 last_error()=0;
};
// 00436789: connected environment lookup and original CTYPE integer parser.
void initialize_timezone(ZoneState&,original_environment4::State&,original_environment4::Services&,original_locale7::State&,original_locale7::NlsServices&,Services&);
// 004300BE: directory change and hidden '=C:' drive entry update.
I32 change_directory(original_runtime16::IoState&,const char*,std::span<const original_runtime8::ErrorPair>,original_locale7::State&,original_locale7::NlsServices&,Services&);
class TimeLink final:public original_runtime15::TimeServices {
public:
 TimeLink(ZoneState& z,original_environment4::State& e,original_environment4::Services& es,original_locale7::State& l,original_locale7::NlsServices& n,Services& s):zone_(z),environment_(e),environment_services_(es),locale_(l),nls_(n),services_(s){}
 void initialize_timezone(original_runtime15::TimeState&) override;
private:ZoneState& zone_;original_environment4::State& environment_;original_environment4::Services& environment_services_;original_locale7::State& locale_;original_locale7::NlsServices& nls_;Services& services_;
};
}
