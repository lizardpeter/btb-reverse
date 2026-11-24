#include "btb/original_float6.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace btb::original_float6;
int main(int argc,char** argv) {
    if(argc!=2 || !verify_6_original_float_functions())return 1;
    // Safe host rejection of retail nonterminating/out-of-bounds inputs.
    try { (void)decimal_digits_to_extended(std::array<U8,1>{0});return 5; }
    catch(const std::domain_error&) {}
    Words96 invalid{};
    try { (void)round_mantissa(invalid,95);return 6; }
    catch(const std::out_of_range&) {}
    try { std::array<U32,1> short_output{};Extended12 v{};
          (void)convert_extended(v,short_output,double_format);return 7; }
    catch(const std::invalid_argument&) {}
    std::ifstream file(argv[1]);
    if(!file)return 2;
    std::string line,kind;
    unsigned checked=0;
    while(std::getline(file,line)) {
        std::istringstream in(line);in>>kind>>std::hex;
        bool okay=false;
        if(kind=="CTRL") {U32 v,expected;in>>v>>expected;okay=control_word_from_runtime(v)==expected;}
        else if(kind=="ROUND" || kind=="SHIFT") {
            unsigned count;Words96 w,expected;U32 result=0;in>>count>>w[0]>>w[1]>>w[2];
            if(kind=="ROUND")in>>result;
            in>>expected[0]>>expected[1]>>expected[2];
            if(kind=="ROUND")okay=round_mantissa(w,count)==result;
            else {shift_mantissa_right(w,count);okay=true;}
            okay=okay && w==expected;
        } else if(kind=="EXTEND") {
            U64 bits;Extended10 expected;in>>bits>>expected.low>>expected.high>>expected.sign_exponent;
            const auto got=double_to_extended(bits);
            okay=got.low==expected.low && got.high==expected.high && got.sign_exponent==expected.sign_exponent;
        } else if(kind=="CONVERT") {
            unsigned width;Extended12 v;U32 result;std::array<U32,2> expected,got{};
            in>>width;for(auto& word:v)in>>word;
            in>>result>>expected[0]>>expected[1];
            okay=convert_extended(v,got,width==32?float_format:double_format)==result && got==expected;
            std::array<U32,2> wrapper{};
            if(width==32)decode_float(v,wrapper);else decode_double(v,wrapper);
            okay=okay && wrapper==expected;
        }
        else if(kind=="DECIMAL") {
            unsigned size;in>>size;std::vector<U8> digits(size);
            for(auto& d:digits){unsigned v;in>>v;d=static_cast<U8>(v);}
            Words96 expected;in>>expected[0]>>expected[1]>>expected[2];
            okay=decimal_digits_to_extended(digits)==expected;
        }
        if(!in || !okay){std::cerr<<"Mismatch at vector "<<checked<<": "<<line<<'\n';return 3;}
        ++checked;
    }
    if(checked!=34314)return 4;
    std::cout<<checked<<" original x86 numeric comparisons passed\n";
    return 0;
}
