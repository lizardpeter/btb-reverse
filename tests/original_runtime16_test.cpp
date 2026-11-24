#include "btb/original_runtime16.hpp"
#include <cassert>
#include <cstring>
#include <limits>
#include <random>
#include <stdexcept>
#include <vector>
using namespace btb::original_runtime16;
struct Io final:IoServices {
    std::vector<std::array<U32,2>> standard;
    Stream32 fresh{};bool allocate_ok{true},terminal{true},flushed{true};
    U32 guest{0x8000},allocation_calls{},flush_calls{},write_calls{},last_write{};
    U32 result_write{0xffffffffU};
    void set_standard_handle(U32 which,U32 handle) override {standard.push_back({which,handle});}
    bool flush_file(U32) override {++flush_calls;return flushed;}
    U32 last_error() override {return 123;}
    U32 write_descriptor(U32 fd,U32 address,U32 count) override {
        assert(fd==4 && address==0x2000);++write_calls;last_write=count;
        return result_write==0xffffffffU?count:result_write;
    }
    Stream32* allocate_stream(std::size_t bytes) override {
        assert(bytes==32);++allocation_calls;return allocate_ok?&fresh:nullptr;
    }
    bool is_terminal(U32) override {return terminal;}
    U32 allocate_guest_bytes(U32 bytes) override {assert(bytes==4096);++allocation_calls;return guest;}
};
void* failure(std::size_t){return nullptr;}
int main() {
    assert(verify_16_original_runtime_functions());
    // Both original __aullrem paths and the reciprocal correction frontier.
    std::mt19937_64 rng(0x2002);
    for(int i=0;i<100000;++i) {
        const U64 a=rng(),b=(i&1)?(rng()|1):static_cast<U32>(rng()|1);
        assert(unsigned_remainder(a,b)==a%b);
    }
    for(U64 a:{0ULL,1ULL,0xffffffffULL,0x100000000ULL,0xffffffffffffffffULL})
        for(U64 b:{1ULL,2ULL,0xffffffffULL,0x100000000ULL,0xffffffffffffffffULL})
            assert(unsigned_remainder(a,b)==a%b);
    bool rejected=false;try{unsigned_remainder(7,0);}catch(const std::domain_error&){rejected=true;}
    assert(rejected);
    for(int offset=0;offset<4;++offset) {
        char backing[]="xxxxyzyz";char* text=backing+offset;
        assert(find_character(text,'z')==backing+5);
        assert(find_character(text,0x179)==backing+4); // truncates to byte 'y'
        assert(find_character(text,0)==backing+8);
    }
    Io io;std::array<Descriptor32,5> descriptors;
    IoState s{descriptors,71,72,true};
    for(U32 fd=0;fd<3;++fd) {
        assert(assign_os_handle(s,fd,fd+100,io)==0);
        assert((io.standard.back()==std::array<U32,2>{0xfffffff6U-fd,fd+100}));
        descriptors[fd].flags=1;
    }
    assert(assign_os_handle(s,0,999,io)==-1 && s.dos_error==0 && s.error==9);
    assert(assign_os_handle(s,4,104,io)==0 && io.standard.size()==3);
    descriptors[4].flags=1;
    assert(release_os_handle(s,4,io)==0 && io.standard.size()==3 && descriptors[4].flags==1);
    descriptors[3].flags=0;
    assert(release_os_handle(s,3,io)==-1);
    s.dos_error=77;assert(commit_os_handle(s,99,io)==-1 && s.dos_error==77 && io.flush_calls==0);
    io.flushed=false;assert(commit_os_handle(s,0,io)==-1 && s.dos_error==123);
    io.flushed=true;s.error=66;assert(commit_os_handle(s,0,io)==0 && s.error==66 && s.dos_error==123);
    assert(set_descriptor_mode(s,0,0x4000)==0x8000);
    auto before=descriptors[0].flags;
    assert(set_descriptor_mode(s,0,123)==-1 && s.error==22 && descriptors[0].flags==before);
    assert(set_descriptor_mode(s,99,0x4000)==-1 && s.error==9);
    // FILE allocation selects the first vacant or inactive slot, preserving
    // words 5/6. Allocation failure returns immediately without later reuse.
    Stream32 busy{},inactive{};busy.words[3]=0x83;
    inactive.words.fill(0x12345678);inactive.words[3]=0;
    std::array<Stream32*,3> slots{&busy,nullptr,&inactive};io.allocate_ok=false;
    assert(!acquire_stream(slots,io) && slots[1]==nullptr && inactive.words[1]==0x12345678);
    slots[1]=&inactive;
    assert(acquire_stream(slots,io)==&inactive && inactive.words[5]==0x12345678 && inactive.words[6]==0x12345678);
    assert(inactive.words[4]==0xffffffffU && inactive.words[0]==0 && inactive.words[7]==0);
    Stream32 stream{{0x2010,100,0x2000,0x182,4,5,6,7}};
    io.result_write=8;
    assert(flush_stream(stream,io)==-1 && (stream.words[3]&0x20));
    assert(stream.words[0]==0x2000 && stream.words[1]==0 && io.last_write==16);
    // Existing original flush/flush-all callers now run the NEW original
    // flush and descriptor-commit bodies through this connected adapter.
    io.result_write=0xffffffffU;io.flushed=true;
    descriptors[4].handle=104;descriptors[4].flags=1;
    BufferedIoLink linked(s,io,{});
    auto callbacks=linked.callbacks();
    btb::original_crt10::File file{9,0x1c2,4,0x2010,0x2000,0};
    U32 commits=io.flush_calls;
    assert(btb::original_crt10::flush_one(&file,callbacks,nullptr,nullptr)==0);
    assert(file.pointer==0x2000 && file.count==0 && io.flush_calls==commits+1);
    io.flushed=false;file.pointer=0x2010;file.flags=0x1c2;
    assert(btb::original_crt10::flush_one(&file,callbacks,nullptr,nullptr)==-1 && s.dos_error==123);
    file.pointer=0x2010;file.flags=0x182;
    std::array<btb::original_crt10::File*,1> files{&file};
    assert(btb::original_crt10::flush_all(files,1,callbacks)==1);
    stream.words[0]=0x1fff;stream.words[1]=55;U32 calls=io.write_calls;
    assert(flush_stream(stream,io)==0 && io.write_calls==calls && stream.words[1]==0);
    std::array<U32,2> buffers{};U32 attempts=0;stream.words[3]=0;io.guest=0;
    assert(begin_standard_buffering(stream,0x3000,StandardStream::Error,buffers,attempts,io)==1);
    assert(stream.words[0]==0x3014 && stream.words[2]==0x3014 && stream.words[6]==2 && stream.words[1]==2);
    calls=io.allocation_calls;
    assert(begin_standard_buffering(stream,0x3000,StandardStream::Error,buffers,attempts,io)==0);
    assert(attempts==2 && io.allocation_calls==calls);
    stream.words[3]=0;io.terminal=false;
    assert(begin_standard_buffering(stream,0x3000,StandardStream::Out,buffers,attempts,io)==0 && attempts==2);
    io.terminal=true;assert(begin_standard_buffering(stream,0x3000,StandardStream::Other,buffers,attempts,io)==0);
    Type one{"type"},equal{"type"},other{"other"},empty{""};
    for(U32 q=0;q<4;++q)for(unsigned thrown=0;thrown<16;++thrown)for(unsigned caught=0;caught<4;++caught) {
        const bool expected=(!(caught&2)||(thrown&8))&&(!(q&1)||(thrown&1))&&(!(q&2)||(thrown&2));
        assert(catch_type_matches({static_cast<std::uint8_t>(thrown),&one},
            {static_cast<std::uint8_t>(caught),&equal},q)==expected);
    }
    assert(catch_type_matches({0,&empty},{0,&other},3));
    assert(!catch_type_matches({15,&one},{0,&other},0));
    std::array<char,16> decimal{};I32 point=5;
    round_decimal_digits(decimal,4,"12",point);assert(!std::strcmp(decimal.data(),"1200") && point==5);
    round_decimal_digits(decimal,2,"125",point);assert(!std::strcmp(decimal.data(),"13"));
    round_decimal_digits(decimal,2,"124",point);assert(!std::strcmp(decimal.data(),"12"));
    round_decimal_digits(decimal,0,"8",point);assert(!std::strcmp(decimal.data(),"1") && point==6);
    round_decimal_digits(decimal,-1,"8",point);assert(!std::strcmp(decimal.data(),"") && point==6);
    btb::original_crt17::Exception borrowed{0,const_cast<char*>("borrowed"),0},copy{};
    copy_exception(copy,borrowed,nullptr);assert(copy.message==borrowed.message && copy.owned==0);
    construct_exception(copy,"allocation failure",failure);assert(!copy.message && copy.owned==1);
    std::array<std::byte,128> image{};assert(read_linker_version(image)==0);
    image[0]=std::byte{'M'};image[1]=std::byte{'Z'};assert(read_linker_version(image)==0);
    image[60]=std::byte{120};rejected=false;
    try{read_linker_version(image);}catch(const std::out_of_range&){rejected=true;}assert(rejected);
}
