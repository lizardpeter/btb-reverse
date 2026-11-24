#include "btb/retail_source_batch_32.hpp"
#include <array>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
using namespace btb::retail_source_batch_32;
namespace {
std::string calls{};
void one(){calls+='1';}
void two(){calls+='2';}
bool bad_code(const void* p){return p==nullptr;}
bool bad_region(const void* p,std::size_t size){return p==nullptr||size>8;}
U32 last_input{};void input(U32 v){last_input=v;}
int overflow_calls{};U32 overflow_ok(U32 v){++overflow_calls;return v&255U;}
U32 overflow_fail(U32){++overflow_calls;return 0xffffffffU;}
U32 unwind_a{},unwind_b{};void unwind(U32 a,U32 b){unwind_a=a;unwind_b=b;}
U32 table_selected{};void decoder(U16*,U32*,U32 table){table_selected=table;}
I32 callback_final{-1};void decode(void*,U8*,I32*,void*,I32 v){callback_final=v;}
U32 fpu_value{},fpu_mask{};void set_fpu(U32 a,U32 b){fpu_value=a;fpu_mask=b;}
char* allocated(std::size_t size){return static_cast<char*>(std::malloc(size));}
}
int main(){
    // 00433630, 0043991A, 00439E9E: copy, length, allocate/copy.
    char text[10]{};
    assert(copy_zero_terminated_string(text,"hello")==text);
    assert(std::strcmp(text,"hello")==0);
    static_assert(bounded_string_length("hello",3)==3);
    static_assert(bounded_string_length("hello",9)==5);
    static_assert(bounded_string_length("hello",0)==0);
    char* copy=duplicate_string("hello",allocated);
    assert(copy!=nullptr&&std::strcmp(copy,"hello")==0);
    std::free(copy);
    assert(duplicate_string(nullptr,allocated)==nullptr);
    assert(duplicate_string("hi",nullptr)==nullptr);

    // 004377BA, 0043779F, 004377C6: 96-bit clear/copy/query.
    constexpr Words96 original{0x12345678U,0xabcdef01U,0x0fedcba9U};
    Words96 w{};
    copy_words96(w,original);
    assert(w==original&&!words96_are_zero(w));
    reset_words96(w);
    assert(words96_are_zero(w));

    // 004387C7 carry helper including unsigned wrap.
    U32 out=0;
    assert(add32_with_carry(1,2,out)==0&&out==3);
    assert(add32_with_carry(0xffffffffU,1,out)==1&&out==0);
    assert(add32_with_carry(0xffffffffU,0xffffffffU,out)==1&&
           out==0xfffffffeU);

    // 00438846/00438874 shift 96-bit limbs.
    w={0x80000000U,0x80000000U,0x80000000U};
    shift_words96_left_one(w);
    assert((w==Words96{0,1,1}));
    w={0,1,1};
    shift_words96_right_one(w);
    assert((w==Words96{0x80000000U,0x80000000U,0}));
    w={0,0,1};
    shift_words96_right_one(w);
    assert((w==Words96{0,0x80000000U,0}));

    // 00437674 tests bits below selected bit and higher limbs.
    constexpr Words96 zeros{0,0,0};
    static_assert(words96_high_parts_are_zero(zeros,0));
    static_assert(words96_high_parts_are_zero(zeros,95));
    static_assert(!words96_high_parts_are_zero(zeros,96));
    assert(words96_high_parts_are_zero(Words96{0xffffffffU,0,0},31));
    assert(!words96_high_parts_are_zero(Words96{0xffffffffU,0,0},0));
    assert(!words96_high_parts_are_zero(Words96{0,0,1},0));

    // 004376BD add masked bit and carry toward smaller-index limbs.
    w={0,0,0};
    assert(!increment_words96_masked(w,0));
    assert((w==Words96{0x80000000U,0,0}));
    w={0,0xffffffffU,0};
    assert(!increment_words96_masked(w,63));
    assert((w==Words96{1,0,0}));
    w={0xffffffffU,0xffffffffU,0};
    assert(increment_words96_masked(w,63));
    assert((w==Words96{0,0,0}));
    assert(!increment_words96_masked(w,96));

    // 004387E8 3-limb addition.
    w={0xffffffffU,0xffffffffU,0};
    add_words96(w,Words96{1,0,1});
    assert((w==Words96{0,0,2}));
    w={0xffffffffU,0xffffffffU,0xffffffffU};
    add_words96(w,Words96{1,0,0});
    assert(words96_are_zero(w));

    // 00436640/00436680, multiply/shift x86 64-bit support.
    static_assert(msvc_allmul(3,0,7,0)==21);
    static_assert(msvc_allmul(0,1,3,0)==(U64{3}<<32));
    static_assert(msvc_allmul(0xffffffffU,0xffffffffU,2,0)==
                  0xfffffffffffffffeULL);
    static_assert(msvc_allshl(1,0,32)==(U64{1}<<32));
    static_assert(msvc_allshl(1,0,64)==0);
    static_assert(msvc_allshl(1,0,63)==(U64{1}<<63));
    static_assert(msvc_allshl(0,1,1)==(U64{2}<<32));

    // 00436540 _memset, including original pointer return and zero count.
    std::array<U8,6> array{1,2,3,4,5,6};
    assert(msvc_memset(array.data()+1,0x1ff,3)==array.data()+1);
    assert((array==std::array<U8,6>{1,255,255,255,5,6}));
    assert(msvc_memset(array.data(),7,0)==array.data());
    assert(array[0]==1);

    // 00436EB6: all recognized retail code mappings plus fallback.
    static_assert(map_runtime_code(0x3a4)==0x411);
    static_assert(map_runtime_code(0x3a8)==0x804);
    static_assert(map_runtime_code(0x3b5)==0x412);
    static_assert(map_runtime_code(0x3b6)==0x404);
    static_assert(map_runtime_code(0)==0);

    // 00436745/0D/29 WinAPI pointer validation preserves invert polarity.
    int integer=12;
    assert(executable_pointer_valid(&integer,bad_code));
    assert(!executable_pointer_valid(nullptr,bad_code));
    assert(!executable_pointer_valid(&integer,nullptr));
    assert(readable_pointer_range(&integer,8,bad_region));
    assert(!readable_pointer_range(&integer,9,bad_region));
    assert(writable_pointer_range(&integer,4,bad_region));
    assert(!writable_pointer_range(nullptr,4,bad_region));

    // 00438241: table and class-mask checks, including absent backing tables.
    std::array<U8,4> flags{0,0,0x04,0};
    std::array<U16,3> classes{0,0x10,0};
    assert(char_class_matches(1,0,4,flags,classes));
    assert(char_class_matches(1,0x10,0,flags,classes));
    assert(!char_class_matches(1,0x20,0,flags,classes));
    assert(!char_class_matches(3,0xffffffffU,0,flags,classes));

    // 00434444: contiguous 20-byte header search.
    std::array<std::array<U32,5>,2> records{};
    records[0][3]=0x300000U;
    records[1][3]=0x120000U;
    assert(find_allocation_region_offset(0x120010U,records)==20);
    assert(find_allocation_region_offset(0x300001U,records)==0);
    assert(find_allocation_region_offset(0x80000000U,records)==-1);

    // 0043091D initializer table, skip null, preserve order.
    constexpr std::array<Initializer,3> inits{one,nullptr,two};
    run_initializer_table(inits);
    assert(calls=="12");

    // 00431D8A EOF suppressor.
    last_input=0;
    forward_if_not_eof(0xffffffffU,input);
    assert(last_input==0);
    forward_if_not_eof(23,input);
    assert(last_input==23);

    // 004330B2/004330E7 buffered putc, overflow and early error stop.
    std::array<U8,2> writebuf{};
    WriteCursor cursor{writebuf,0,2,0};
    overflow_calls=0;
    write_stream_byte('A',cursor,overflow_ok);
    assert(writebuf[0]=='A'&&cursor.remaining==1&&cursor.total==1);
    repeat_stream_byte('B',3,cursor,overflow_ok);
    assert(writebuf[1]=='B'&&overflow_calls==2&&cursor.total==4);
    WriteCursor broken{writebuf,0,0,0};
    overflow_calls=0;
    repeat_stream_byte('X',7,broken,overflow_fail);
    assert(broken.total==-1&&overflow_calls==1);

    // 00435FF5 SEH delegation through original +0x18/+0x1C offsets.
    unwind_exception_frame({0,0,0,0,0,0,34,56},unwind);
    assert(unwind_a==34&&unwind_b==56);

    // 0043A0DD/0043A1C5 C++ constructors, vtable address provenance.
    LegacyVptrObject obj{};
    calls.clear();
    assert(construct_runtime_object_a(obj,one)==&obj);
    assert(obj.vtable_address==0x0043bb54U&&calls=="1");
    assert(construct_runtime_object_b(obj,two)==&obj);
    assert(obj.vtable_address==0x0043bb84U&&calls=="12");

    // 004379DA/F0 original table selection.
    U16 decoded{};U32 state{};
    decode_using_first_table(&decoded,&state,decoder);
    assert(table_selected==0x00449f48U);
    decode_using_second_table(&decoded,&state,decoder);
    assert(table_selected==0x00449f60U);

    // 00437EB3 forwards extra argument 0.
    I32 i=0;U8 b=0;
    decode_forward_zero(nullptr,&b,&i,nullptr,decode);
    assert(callback_final==0);

    // 00437478 clears bit 0x80000 from the forwarded control-word mask.
    update_control_word_masked(0xffffU,0xffffffffU,set_fpu);
    assert(fpu_value==0xffffU&&fpu_mask==0xfff7ffffU);
}
