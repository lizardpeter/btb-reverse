#pragma once
#include "btb/original_locale7.hpp"
#include "btb/original_runtime8.hpp"
#include "btb/original_runtime16.hpp"
namespace btb::original_runtime6 {
using namespace original_locale7;
// 00439212 / 004386BF / 004331F4: direct recovered classification/NLS callers.
U32 upper_character(State&,U32,NlsServices&);
I32 parse_integer(State&,const char*,NlsServices&);
void insert_decimal_point(State&,std::span<char>,char decimal,NlsServices&);
// 0043316D preserves high return bits from the advanced guest argument pointer.
U32 fetch_short_argument(std::span<const std::byte>,U32 guest_base,U32& guest_cursor);
class SystemCodepages {public:virtual ~SystemCodepages()=default;virtual I32 oem()=0;virtual I32 ansi()=0;};
// 00436CD3: original static codepage ranges + 00436F12 case-table connection.
I32 set_multibyte_codepage(State&,original_runtime8::Locale&,U32& use_system,I32,SystemCodepages&,NlsServices&);
class OpenServices {public:virtual ~OpenServices()=default;virtual I32 open_file(const char*,U32 flags,U32 share,U32 permissions)=0;};
// 00433E95: open mode parser; retain words 5 and 6, reset exactly six fields.
original_runtime16::Stream32* open_stream(const char*,const char*,U32 share,
    original_runtime16::Stream32&,U32 default_flags,U32& allocations,OpenServices&);
bool verify_6_original_runtime_functions();
}
