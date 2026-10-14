#include "btb/original_locale7.hpp"
#include <stdexcept>
namespace btb::original_locale7 {
bool verify_7_original_locale_functions() {
    struct Services final:NlsServices {
        I32 multibyte_to_wide(U32,U32,const char*,I32,U16*,I32) override {throw std::logic_error("unexpected NLS call");}
        I32 wide_to_multibyte(U32,U32,const U16*,I32,char*,I32) override {throw std::logic_error("unexpected NLS call");}
        I32 map_ansi(U32,U32,const char*,I32,char*,I32) override{return 0;}
        I32 map_wide(U32,U32,const U16*,I32,void*,I32) override{return 0;}
        bool type_ansi(U32,U32,const char*,I32,U16*) override{return false;}
        bool type_wide(U32,const U16*,I32,U16*) override{return false;}
        bool codepage_info(U32,CodepageInfo&) override{return false;}
    } api;
    State s;s.classes['A'+1]=1;U16 out=0;
    if(convert_character(s,&out,"B",1,api)!=1 || out!='B')return false;
    if(classify(s,'A',1,api)!=1 || lower_character(s,'A',api)!='a')return false;
    if(map_narrow(s,0,0x100,"A",1,nullptr,0,0,true,api)!=0)return false;
    U16 classes[2]{};
    if(classify_narrow(s,1,"A",1,classes,0,0,true,api))return false;
    build_multibyte_case_table(s,api);
    return upper_multibyte_character(s,'b',api)=='B' && s.case_map['Z']=='z';
}
}
