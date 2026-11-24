#include "btb/original_ftol1.hpp"
namespace btb::original_ftol1 {
std::uint64_t truncate_integer(const original_float6::Extended10& value) noexcept {
    using U64=std::uint64_t;
    constexpr U64 invalid=U64{1}<<63;
    const U64 mantissa=U64(value.high)<<32|value.low;
    const unsigned exponent=value.sign_exponent&0x7fffU;
    const bool negative=(value.sign_exponent&0x8000U)!=0;
    if(exponent==0x7fffU || (exponent!=0 && !(mantissa>>63)))return invalid;
    if(exponent<0x3fffU)return 0;
    const unsigned power=exponent-0x3fffU;
    if(power>63)return invalid;
    const U64 magnitude=mantissa>>(63-power);
    if(magnitude>invalid || (!negative && magnitude==invalid))return invalid;
    return negative?U64{0}-magnitude:magnitude;
}
}
