#include "btb/original_float6.hpp"
#include <stdexcept>
namespace btb::original_float6 {
U32 control_word_from_runtime(U32 v) noexcept {
    U32 out=(v&0x10)?1:0;
    if(v&8)out|=4;
    if(v&4)out|=8;
    if(v&2)out|=0x10;
    if(v&1)out|=0x20;
    if(v&0x80000)out|=2;
    out|=(v&0x300)<<2;
    if(!(v&0x30000))out|=0x300;
    else if((v&0x30000)==0x10000)out|=0x200;
    if(v&0x40000)out|=0x1000;
    return out;
}
U32 round_mantissa(Words96& w,unsigned bits) {
    // Original callers use 24/53. Reject indices that read outside LD12.
    if(bits==0 || bits>=95)throw std::out_of_range("original mantissa bit index");
    const unsigned limb=bits/32,shift=31-bits%32;
    U32 carry=0;
    if((w[limb]&(U32{1}<<shift)) && !higher_parts_zero(w,bits+1))
        carry=increment_masked(w,bits-1)?1:0;
    w[limb]&=0xffffffffU<<shift;
    for(unsigned i=limb+1;i<3;++i)w[i]=0;
    return carry;
}
void shift_mantissa_right(Words96& w,unsigned count) noexcept {
    // The converter's word order is most significant first. Original
    // shift instructions mask CL to five bits, including count==32.
    const unsigned whole=count/32,part=count%32;
    U32 carry=0;
    for(auto& word:w) {
        const U32 old=word;
        word=(old>>part)|carry;
        carry=(old&~(0xffffffffU<<part))<<((32-part)&31);
    }
    for(int i=2;i>=0;--i)w[i]=static_cast<unsigned>(i)<whole?0:w[i-whole];
}
U32 convert_extended(const Extended12& in,std::span<U32> out,const Format& f) {
    if((f[4]!=32 && f[4]!=64) || out.size()<static_cast<unsigned>(f[4]/32) ||
       f[2]<=0 || f[2]>=95 || f[3]<0 || f[3]>31)
        throw std::invalid_argument("original floating point format");
    Words96 w{U32(in[3])|(U32(in[4])<<16),
              U32(in[1])|(U32(in[2])<<16),U32(in[0])<<16};
    I32 exponent=I32(in[5]&0x7fff)-0x3fff;
    I32 stored=0;
    U32 status=2;
    if(exponent==-0x3fff) {
        if(are_words_zero(w))status=0;
        else zero_words(w);
    } else {
        Words96 saved;
        copy_words(saved,w);
        if(round_mantissa(w,static_cast<unsigned>(f[2])))++exponent;
        if(exponent<f[1]-f[2])zero_words(w);
        else if(exponent<=f[1]) {
            copy_words(w,saved);
            shift_mantissa_right(w,static_cast<unsigned>(f[1]-exponent));
            (void)round_mantissa(w,static_cast<unsigned>(f[2]));
            shift_mantissa_right(w,static_cast<unsigned>(f[3]+1));
        } else if(exponent>=f[0]) {
            zero_words(w);w[0]|=0x80000000;
            shift_mantissa_right(w,static_cast<unsigned>(f[3]));
            stored=f[5]+f[0];status=1;
        } else {
            w[0]&=0x7fffffff;
            stored=f[5]+exponent;
            shift_mantissa_right(w,static_cast<unsigned>(f[3]));
            status=0;
        }
    }
    w[0]|=(U32(stored)<<((31-f[3])&31))|((in[5]&0x8000)?0x80000000U:0);
    if(f[4]==64){out[1]=w[0];out[0]=w[1];}
    else out[0]=w[0];
    return status;
}
Extended10 double_to_extended(U64 bits) noexcept {
    const U32 lo=static_cast<U32>(bits),hi=static_cast<U32>(bits>>32);
    const unsigned exp=(hi>>20)&0x7ff;
    Extended10 out;
    U32 leading=0x80000000;
    I32 extended_exp;
    if(!exp) {
        // Original collapses both signs of zero to positive zero.
        if(!(hi&0xfffff) && !lo)return out;
        extended_exp=0x3c01;leading=0;
    } else extended_exp=exp==0x7ff?0x7fff:static_cast<I32>(exp)+0x3c00;
    out.high=(lo>>21)|((hi&0xfffff)<<11)|leading;
    out.low=lo<<11;
    while(!(out.high&0x80000000)) {
        out.high=(out.low>>31)|(out.high<<1);
        out.low<<=1;--extended_exp;
    }
    out.sign_exponent=static_cast<U16>((hi>>16)&0x8000)|static_cast<U16>(extended_exp);
    return out;
}
Words96 decimal_digits_to_extended(std::span<const U8> digits) {
    Words96 words{};
    for(U8 digit:digits) {
        Words96 saved;
        copy_words(saved,words);
        shift_left_one(words);shift_left_one(words);
        add_words(words,saved);shift_left_one(words);
        add_words(words,Words96{digit,0,0});
    }
    if(are_words_zero(words))
        throw std::domain_error("zero decimal significand would not terminate in retail");
    U16 exponent=0x404e;
    while(words[2]==0) {
        words[2]=words[1]>>16;
        words[1]=(words[0]>>16)|(words[1]<<16);
        words[0]<<=16;
        exponent=static_cast<U16>(exponent-16);
    }
    while(!(words[2]&0x8000)) {
        shift_left_one(words);--exponent;
    }
    words[2]=(words[2]&0xffff)|(U32(exponent)<<16);
    return words;
}
void decode_double(const Extended12& v,std::span<U32> out){
    if(out.size()<2)throw std::invalid_argument("short double output");
    original_crt32::decode_first(v.data(),out.data());
}
void decode_float(const Extended12& v,std::span<U32> out){
    if(out.empty())throw std::invalid_argument("short float output");
    original_crt32::decode_second(v.data(),out.data());
}
bool verify_6_original_float_functions() {
    if(decimal_digits_to_extended(std::array<U8,1>{1})!=Words96{0,0,0x3fff8000})return false;
    if(control_word_from_runtime(0x8001f)!=0x33f)return false;
    const auto one=double_to_extended(0x3ff0000000000000ULL);
    if(one.low || one.high!=0x80000000 || one.sign_exponent!=0x3fff)return false;
    Extended12 in{0,0,0,0,0x8000,0x3fff};
    std::array<U32,2> out{};
    decode_double(in,out);
    if(out[0] || out[1]!=0x3ff00000)return false;
    decode_float(in,out);
    if(out[0]!=0x3f800000)return false;
    Words96 w{0x80000000,0,0};
    if(round_mantissa(w,24)!=0)return false;
    shift_mantissa_right(w,32);
    return w==Words96{0,0x80000000,0};
}
}
