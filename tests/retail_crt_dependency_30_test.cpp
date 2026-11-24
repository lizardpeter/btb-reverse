#include "btb/retail_crt_dependency_30.hpp"
#include <array>
#include <cassert>
#include <cstring>
#include <optional>
#include <string>
#include <vector>
using namespace btb::crt_batch_30;
namespace {
struct Tracking {
    std::string events;
    U32 code{},cleanup{},reserved{},classification{},table{},fpu{};
    I32 argument{};
    U32 throwinfo{};
    void* address{};
    I32 read_errors{},writes{};
};
void exit_call(U32 code,U32 cleanup,U32 reserved,void* ptr){
    auto& t=*static_cast<Tracking*>(ptr);
    t.events+='X';t.code=code;t.cleanup=cleanup;t.reserved=reserved;
}
U32 class_call(U32 a,U32 b,U32 c,void* ptr){
    auto& t=*static_cast<Tracking*>(ptr);t.events+='C';
    assert(b==0&&c==4);return a==9?4:0;
}
void precision(void* object,U32 a,U32 b,void* ptr){
    auto& t=*static_cast<Tracking*>(ptr);t.events+='P';
    assert(object==t.address&&a==0x10000&&b==0x30000);
}
void init(I32 a,void* ptr){
    auto& t=*static_cast<Tracking*>(ptr);t.events+='I';t.argument=a;
}
void notice(U32 a,void* ptr){
    auto& t=*static_cast<Tracking*>(ptr);t.events+='N';t.code=a;
}
void diagnostic(U32 a,void* ptr){
    auto& t=*static_cast<Tracking*>(ptr);t.events+='D';t.code=a;
}
U32 refill(void* ptr){
    auto& t=*static_cast<Tracking*>(ptr);t.events+='R';
    return 0xc0ffeeU;
}
I32 predicate(U32 a,void* ptr){
    auto& t=*static_cast<Tracking*>(ptr);t.events+='p';return a==19?-5:0;
}
void destruct(void* object,void* ptr){
    auto& t=*static_cast<Tracking*>(ptr);t.events+='d';t.address=object;
}
void free_obj(void* object,void* ptr){
    auto& t=*static_cast<Tracking*>(ptr);t.events+='f';t.address=object;
}
void make_exception(void* data,void* ptr){
    auto& t=*static_cast<Tracking*>(ptr);t.events+='m';
    static_cast<U32*>(data)[0]=0xfeedU;
}
void native_throw(void* data,U32 info,void* ptr){
    auto& t=*static_cast<Tracking*>(ptr);t.events+='t';
    t.throwinfo=info;assert(static_cast<U32*>(data)[0]==0xfeedU);
}
U32 letter_class(U32 c,void* ctx){
    auto& t=*static_cast<Tracking*>(ctx);t.events+='Q';return c=='a'?8:0;
}
void conversion(U32 a,void* b,I32 c,I32 d,void* ctx){
    auto& t=*static_cast<Tracking*>(ctx);
    t.events+='V';assert(a==42&&b==t.address&&c==7&&d==13);
}
U32 mask_class(U32 code,U32 mask,void* ctx){
    auto& t=*static_cast<Tracking*>(ctx);t.events+='M';
    assert(mask==4||mask==8);return code==0x41?mask:0;
}
void format_prep(void*,std::array<U16,6>& bytes,U8*,void* ctx){
    auto& t=*static_cast<Tracking*>(ctx);t.events+='S';bytes[0]=123;
}
void format_finish(std::array<U16,6>& bytes,U32* out,U32 table,void* ctx){
    auto& t=*static_cast<Tracking*>(ctx);t.events+='F';
    assert(bytes[0]==123);t.table=table;*out=456;
}
U32 read_control(void* ctx){auto& t=*static_cast<Tracking*>(ctx);t.events+='r';return 0xaaaaU;}
void write_control(U32 v,void* ctx){
    auto& t=*static_cast<Tracking*>(ctx);t.events+='w';t.fpu=v;
}
void c1(){}void c2(){}void c3(){}void c4(){}void c5(){}void c6(){}
void write_byte(U32 byte,I32& count,void* ctx){
    auto& t=*static_cast<Tracking*>(ctx);t.events+=static_cast<char>(byte);
    ++count;if(byte=='!')count=-1;
}
void flush(void*,void* ctx){static_cast<Tracking*>(ctx)->events+='L';}
}
int main(){
    Tracking t{};
    exit_immediately(17,exit_call,&t);
    assert(t.code==17&&t.cleanup==0&&t.reserved==0&&t.events=="X");
    exit_with_cleanup(21,exit_call,&t);
    assert(t.code==21&&t.cleanup==1&&t.events=="XX");
    assert(classify_code_category_four(9,class_call,&t)==4);
    assert(classify_code_category_four(5,class_call,&t)==0);
    assert(classify_code_category_four(5,nullptr,&t)==0);

    int object=88;t.address=&object;
    set_precision_control(&object,precision,&t);
    assert(t.events.ends_with('P'));

    U32 once=0;
    assert(initialize_latch_zero(once,init,&t));
    assert(!initialize_latch_zero(once,init,&t)&&once==1);
    assert(t.argument==0);
    once=0;
    assert(initialize_minus_three(once,init,&t));
    assert(once==1&&t.argument==-3);
    assert(!initialize_minus_three(once,init,&t));
    t.events.clear();
    fatal_runtime_error(notice,diagnostic,exit_call,&t);
    assert(t.events=="NDX"&&t.code==3&&t.cleanup==0);

    const std::array<U8,2> buffer{0x80,0x9f};
    InputCursor cursor{buffer.data(),2,&t};
    assert(read_stream_byte(cursor,refill)==128);
    assert(cursor.remaining==1&&cursor.current==buffer.data()+1);
    assert(read_stream_byte(cursor,refill)==159);
    assert(read_stream_byte(cursor,refill)==0xc0ffeeU);
    assert(cursor.remaining==-1);
    assert(predicate_to_bool(19,predicate,&t)==1);
    assert(predicate_to_bool(12,predicate,&t)==0);
    assert(predicate_to_bool(19,nullptr,&t)==0);

    t.events.clear();
    assert(destroy_exception(&object,0,destruct,free_obj,&t)==&object);
    assert(t.events=="d");
    assert(destroy_auxiliary(&object,1,destruct,free_obj,&t)==&object);
    assert(t.events=="ddf");
    assert(destroy_format_exception(&object,3,destruct,free_obj,&t)==&object);
    assert(t.events=="ddfdf");

    std::array<U32,7> ex{};
    t.events.clear();
    throw_format_exception(ex,make_exception,native_throw,&t);
    assert(t.events=="mt"&&t.throwinfo==0x0043c158U);
    t.events.clear();throw_exception_a(ex,make_exception,native_throw,&t);
    assert(t.events=="mt"&&t.throwinfo==0x0043c0e8U);
    t.events.clear();throw_exception_b(ex,make_exception,native_throw,&t);
    assert(t.events=="mt"&&t.throwinfo==0x0043c210U);

    const std::array<U8,3> stream{'a','a','z'};
    cursor={stream.data(),3,&t};
    U32 width=0;t.events.clear();
    assert(scan_until_class_fails(width,cursor,refill,letter_class,&t)=='z');
    assert(width==3&&cursor.remaining==0&&t.events=="QQQ");

    std::array<char,24> dest{};
    std::memcpy(dest.data(),"hello",6);
    shift_terminated_string(dest.data(),3);
    assert(std::strcmp(dest.data()+3,"hello")==0);
    shift_terminated_string(dest.data()+3,-2);
    assert(std::strcmp(dest.data()+1,"hello")==0);
    shift_terminated_string(dest.data()+1,0);
    assert(std::strcmp(dest.data()+1,"hello")==0);

    std::array<FileHandleRow,3> rows{{{123,0x41,{}},{456,0x40,{}},{789,0x01,{}}}};
    assert(file_handle_has_bit40(rows,0,3)==64);
    assert(file_handle_has_bit40(rows,1,3)==64);
    assert(file_handle_has_bit40(rows,2,3)==0);
    assert(file_handle_has_bit40(rows,3,99)==0);

    U32 conversion_flag=0;t.events.clear();t.address=&object;
    convert_with_global_flag(conversion_flag,42,&object,7,13,conversion,&t);
    assert(conversion_flag==0&&t.events=="V");
    std::array<U8,256> classes{};
    classes[0x41]=12;classes[0x42]=0;
    assert(is_class_eight(0x41,1,classes,mask_class,&t)==8);
    assert(is_class_eight(0x42,1,classes,mask_class,&t)==0);
    assert(is_class_eight(0x41,2,classes,mask_class,&t)==8);
    assert(is_class_eight(0x42,2,classes,mask_class,&t)==0);

    U32 result=0;U8 ch=0;t.events.clear();
    format_first_table(&object,&result,&ch,format_prep,format_finish,&t);
    assert(result==456&&t.table==0x00449f48U&&t.events=="SF");
    t.events.clear();
    format_second_table(&object,&result,&ch,format_prep,format_finish,&t);
    assert(result==456&&t.table==0x00449f60U&&t.events=="SF");

    t.events.clear();
    assert(merge_fpu_control(0x5555U,0xff00U,read_control,write_control,&t)==0x55aaU);
    assert(t.fpu==0x55aaU&&t.events=="rw");
    assert(fold_hex_style_character(0x41,1,classes,mask_class,&t)==0x41U);
    assert(fold_hex_style_character(0x42,1,classes,mask_class,&t)==((0x42U&~0x20U)-7U));
    assert(fold_hex_style_character(0x41,2,classes,mask_class,&t)==0x41U);
    assert(fold_hex_style_character(0x42,2,classes,mask_class,&t)==((0x42U&~0x20U)-7U));

    ConversionFunctionTable table{};
    const std::array<RuntimeFunction,6> funcs{c1,c2,c3,c4,c5,c6};
    install_conversion_functions(table,funcs);
    assert(table.entries==funcs);

    t.events.clear();
    I32 byte_count=0;
    write_string_bytes("a!xyz",5,byte_count,write_byte,&t);
    assert(t.events=="a!"&&byte_count==-1);
    t.events.clear();byte_count=0;
    write_string_bytes("xyz",0,byte_count,write_byte,&t);
    assert(t.events.empty()&&byte_count==0);
    write_string_bytes("xyz",3,byte_count,write_byte,&t);
    assert(t.events=="xyz"&&byte_count==3);

    StreamReset stream_state{9,10,11,0x19,100};
    t.events.clear();
    reset_stream_if_buffered(0,stream_state,&object,flush,&t);
    assert(t.events=="L"&&stream_state.cursor==9&&stream_state.buffer_size==100);
    reset_stream_if_buffered(1,stream_state,&object,flush,&t);
    assert(stream_state.cursor==0&&stream_state.count==10);
    assert(stream_state.remaining==0&&stream_state.buffer_size==0);
    assert(stream_state.flags==(0x19U&0xeeU));

    U32 os_error=777,err=888;
    assert(get_native_handle_or_error(rows,0,3,os_error,err)==123);
    assert(os_error==777&&err==888);
    assert(get_native_handle_or_error(rows,1,3,os_error,err)==0xffffffffU);
    assert(os_error==0&&err==9);
    assert(get_native_handle_or_error(rows,2,3,os_error,err)==789);
    assert(get_native_handle_or_error(rows,3,3,os_error,err)==0xffffffffU);

    static_assert(compare_modulo_counter(1,17,5).value());
    static_assert(!compare_modulo_counter(3,17,5).value());
    static_assert(!compare_modulo_counter(1,17,0).has_value());
    const std::array<Triple,3> triples{{{10,1,1},{-7,2,3},{45,4,5}}};
    assert(find_triple(triples,-7)==&triples[1]);
    assert(find_triple(triples,45)==&triples[2]);
    assert(find_triple(triples,99)==nullptr);
}
