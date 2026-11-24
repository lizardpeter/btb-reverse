#include "btb/original_runtime16.hpp"
#include <cstdlib>
#include <cstring>
#include <vector>

namespace btb::original_runtime16 {
namespace {
struct FixtureIo final:IoServices {
    std::vector<std::array<U32,2>> standard_calls;
    U32 flush_handle{},write_count{},allocation_size{},guest_allocation{0x8000};
    bool flush_success{true},terminal{true};
    Stream32 allocated{};
    void set_standard_handle(U32 id,U32 handle) override {standard_calls.push_back({id,handle});}
    bool flush_file(U32 handle) override {flush_handle=handle;return flush_success;}
    U32 last_error() override {return 87;}
    U32 write_descriptor(U32,U32,U32 count) override {write_count=count;return count;}
    Stream32* allocate_stream(std::size_t size) override {
        allocation_size=static_cast<U32>(size);return &allocated;
    }
    bool is_terminal(U32) override {return terminal;}
    U32 allocate_guest_bytes(U32 size) override {allocation_size=size;return guest_allocation;}
};
std::vector<int> initializer_calls;
void hook(){initializer_calls.push_back(0);}
void first(){initializer_calls.push_back(1);}
void second(){initializer_calls.push_back(2);}
void release(void* pointer) noexcept {std::free(pointer);}
}
bool verify_16_original_runtime_functions() {
    FixtureIo io;
    std::array<Descriptor32,4> descriptors;
    IoState state{descriptors,0,0,true};
    if(assign_os_handle(state,1,42,io)!=0 || descriptors[1].handle!=42 ||
       io.standard_calls!=std::vector<std::array<U32,2>>{{0xfffffff5U,42}})return false;
    descriptors[1].flags=1;
    if(commit_os_handle(state,1,io)!=0 || io.flush_handle!=42)return false;
    if(set_descriptor_mode(state,1,0x4000)!=0x8000 || descriptors[1].flags!=0x81)return false;
    if(set_descriptor_mode(state,1,0x8000)!=0x4000 || descriptors[1].flags!=1)return false;
    if(release_os_handle(state,1,io)!=0 || descriptors[1].handle!=0xffffffffU)return false;
    std::array<Stream32*,2> streams{};
    if(acquire_stream(streams,io)!=&io.allocated || io.allocation_size!=32 ||
       io.allocated.words[4]!=0xffffffffU)return false;
    Stream32 output{{0x1010,7,0x1000,0x182,1,0xabcdef,0x1234,0x9876}};
    if(flush_stream(output,io)!=0 || output.words[0]!=0x1000 ||
       output.words[1]!=0 || (output.words[3]&2) || io.write_count!=16)return false;
    output.words[3]=0;std::array<U32,2> buffers{};U32 attempts=0;
    if(begin_standard_buffering(output,0x2000,StandardStream::Out,buffers,attempts,io)!=1 ||
       output.words[2]!=0x8000 || output.words[6]!=4096 || attempts!=1)return false;
    if(unsigned_remainder(0xffffffffffffffffULL,0x100000007ULL)!=
       0xffffffffffffffffULL%0x100000007ULL)return false;
    char text[]="original";
    if(find_character(text,'g')!=text+3 || find_character(text,0)!=text+8 ||
       find_character(text,'x'))return false;
    const std::array<ActionRecord,3> actions{{{10,7,30},{20,9,40},{30,7,50}}};
    if(find_action_record(actions,7)!=actions.data() || find_action_record(actions,3))return false;
    std::array<std::byte,128> image{};
    image[0]=std::byte{'M'};image[1]=std::byte{'Z'};image[60]=std::byte{64};
    image[90]=std::byte{6};image[91]=std::byte{1};
    if(read_linker_version(image)!=0x106)return false;
    const Type t{".?AVexample@@"},same{".?AVexample@@"};
    if(!catch_type_matches({11,&t},{2,&same},3) ||
       catch_type_matches({1,&t},{2,&same},3))return false;
    std::array<char,8> decimal{};I32 point=3;
    round_decimal_digits(decimal,3,"9995",point);
    if(std::strcmp(decimal.data(),"1000") || point!=4)return false;
    btb::original_crt17::Exception ex{},copy{};
    construct_exception(ex,"source message",&std::malloc);
    copy_exception(copy,ex,&std::malloc);
    const bool copied=ex.owned==1 && copy.owned==1 && ex.message && copy.message &&
        ex.message!=copy.message && !std::strcmp(static_cast<const char*>(copy.message),"source message");
    btb::original_crt17::exception_destruct(ex,0x43b5bc,release);
    btb::original_crt17::exception_destruct(copy,0x43b5bc,release);
    if(!copied)return false;
    initializer_calls.clear();
    const std::array<Initializer,2> a{first,nullptr},b{nullptr,second};
    run_runtime_initializers(hook,a,b);
    return initializer_calls==std::vector<int>{0,1,2};
}
} // namespace btb::original_runtime16
