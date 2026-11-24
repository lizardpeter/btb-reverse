#include "btb/original_startup11.hpp"
#include <cstdlib>
#include <stdexcept>
#include <cstring>
#include <unordered_map>
#include <unordered_set>
namespace btb::original_startup11 {
namespace {
struct Process final:ProcessServices {
    std::unordered_set<void*> live;U32 initialized{},fatal{},terminated{},exited{};bool fail{};
    ~Process(){for(auto p:live)std::free(p);}
    void initialize_multibyte() override{++initialized;}
    std::string module_file_name(U32 n) override{if(n!=260)throw std::logic_error("module limit");return "bob.exe";}
    void* allocate(U32 n) override{if(fail)return nullptr;auto* p=std::malloc(n);if(p)live.insert(p);return p;}
    void release(void* p) noexcept override{live.erase(p);std::free(p);}
    [[noreturn]] void fatal_runtime_error(U32 n) override{fatal=n;throw std::runtime_error("fatal CRT");}
    void terminate_process(U32 n) override{terminated=n;}
    void exit_process(U32 n) override{exited=n;}
};
std::vector<unsigned> order;
void first(){order.push_back(1);}void second(){order.push_back(2);}void third(){order.push_back(3);}
struct Memory final:GuestMemory {
    std::unordered_map<U32,U32> words;
    U32 read32(U32 at) override{return words.at(at);}
};
struct Allocator final:AllocationServices {
    std::array<unsigned char,128> data{};U32 small3{},small2{},normal{},handlers{};bool succeed{},retry{},small_success{};
    void* small_heap3(U32 n) override{small3=n;return small_success?data.data():nullptr;}
    void* small_heap2(U32 n) override{small2=n;return small_success?data.data():nullptr;}
    void* zero_heap(U32 n) override {normal=n;if(!succeed)return nullptr;data.fill(0);return data.data();}
    bool new_handler(U32) override{++handlers;if(retry){retry=false;succeed=true;return true;}return false;}
};
struct Descriptors final:DescriptorServices {
    std::vector<std::unique_ptr<DescriptorBlock>> owned;std::vector<U32> closed;
    U32 count{},seek_result{88},seek_origin{},error{5};I32 seek_offset{};bool fail{},close_ok{true};
    DescriptorBlock* allocate_block() override{if(fail)return nullptr;owned.push_back(std::make_unique<DescriptorBlock>());return owned.back().get();}
    U32 standard_handle(U32 which) override{return 0xfffffff6U-which+100;}
    U32 file_type(U32 h) override{return h==100?2:h==101?3:h==102?0:1;}
    void set_handle_count(U32 n) override{count=n;}
    bool close_handle(U32 h) override{closed.push_back(h);return close_ok;}
    U32 seek_handle(U32,I32 offset,U32 origin) override{seek_offset=offset;seek_origin=origin;return seek_result;}
    U32 last_error() override{return error;}
};
struct Io final:IoServices {
    std::vector<std::array<U32,2>> standard;
    void set_standard_handle(U32 which,U32 handle) override{standard.push_back({which,handle});}
    bool flush_file(U32) override{throw std::logic_error("unexpected commit");}
    U32 last_error() override{throw std::logic_error("unexpected I/O error");}
    U32 write_descriptor(U32,U32,U32 n) override{return n;}
    Stream32* allocate_stream(std::size_t) override{throw std::logic_error("unexpected stream allocation");}
    bool is_terminal(U32) override{throw std::logic_error("unexpected terminal probe");}
    U32 allocate_guest_bytes(U32) override{throw std::logic_error("unexpected guest allocation");}
};
}
bool verify_11_original_startup_functions() {
    try {
        std::array<std::uint8_t,257> flags{};
        const auto parsed=parse_command_line("\"bob game.exe\" a \\\"b\\\" \"c d\"",flags);
        if(parsed.values!=std::vector<std::string>{"bob game.exe","a","\"b\"","c d"} || parsed.pointer_count!=5)return false;
        Process p;Arguments args;initialize_arguments(args,"bob.exe one two",false,flags,p);
        if(args.argc!=3 || args.argv.back() || std::strcmp(args.argv[2],"two") || p.initialized!=1)return false;
        p.release(args.allocation);
        initialize_arguments(args,"",true,flags,p);if(args.argc!=1 || std::strcmp(args.argv[0],"bob.exe"))return false;
        p.release(args.allocation);p.fail=true;
        try{initialize_arguments(args,"bob",true,flags,p);return false;}catch(const std::runtime_error&){}
        if(p.fatal!=8)return false;p.fail=false;
        constexpr char raw[]="=C:=secret\0A=one\0B=two\0\0";
        Environment env;env.raw=static_cast<char*>(p.allocate(sizeof(raw)));std::memcpy(env.raw,raw,sizeof(raw));
        initialize_environment(env,true,p);
        if(env.raw || !env.initialized || std::strcmp(env.values[0],"A=one") || std::strcmp(env.values[1],"B=two") || env.values[2])return false;
        p.release(env.values[0]);p.release(env.values[1]);p.release(env.values);
        constexpr Initializer on[]{first,nullptr,second},pre[]{third},final[]{first};
        ExitState exit;order.clear();process_exit(exit,7,false,1,on,pre,final,p);
        if(order!=std::vector<unsigned>{2,1,3,1} || exit.already_exiting || !exit.cleanup_started)return false;
        order.clear();process_exit(exit,9,true,0,on,pre,final,p);
        if(order!=std::vector<unsigned>{1} || p.exited!=9 || !exit.already_exiting)return false;
        process_exit(exit,11,true,1,{},{},{},p);if(p.terminated!=11)return false;
        char buf[32]="Bob";if(concatenate(buf," builds")!=buf || std::strcmp(buf,"Bob builds"))return false;
        Memory mem;mem.words={{0x1010,0x2000},{0x2004,0x30}};
        if(adjust_pointer(0x1000,{8,16,4},mem)!=0x1048 || adjust_pointer(0x1000,{8,-1,4},mem)!=0x1008)return false;
        Allocator a;a.succeed=true;
        if(allocate_zero(3,7,0,0,false,a)!=a.data.data() || a.normal!=32)return false;
        a.small_success=true;a.data.fill(0xcc);
        if(allocate_zero(3,7,3,100,false,a)!=a.data.data() || a.small3!=21 || a.data[20]!=0 || a.data[21]!=0xcc)return false;
        a.data.fill(0xcc);if(!allocate_zero(3,7,2,100,false,a) || a.small2!=2 || a.data[31] || a.data[32]!=0xcc)return false;
        a.small_success=false;a.succeed=false;a.retry=true;
        if(!allocate_zero(1,12,0,0,true,a) || a.handlers!=1)return false;
        if(allocate_zero(1,0xffffffe1U,0,0,true,a) || a.handlers!=2)return false;
        a.succeed=true;if(!allocate_zero(0x80000000U,2,0,0,false,a) || a.normal!=16)return false;
        DescriptorTable table;Descriptors d;
        if(allocate_descriptor(table,d)!=0 || table.limit!=32 || (*table.blocks[0])[0].reserved[0]!=10)return false;
        (*table.blocks[0])[0].flags=1;(*table.blocks[0])[1].flags=0x82;
        if(allocate_descriptor(table,d)!=1 || (*table.blocks[0])[1].flags!=0x82)return false;
        for(auto& row:*table.blocks[0])row.flags=1;
        d.fail=true;if(allocate_descriptor(table,d)!=-1)return false;d.fail=false;
        if(allocate_descriptor(table,d)!=32 || table.limit!=64)return false;
        DescriptorTable initial;initialize_descriptors(initial,{},d);
        if(d.count!=32 || (*initial.blocks[0])[0].flags!=0xc1 || (*initial.blocks[0])[1].flags!=0x89 || (*initial.blocks[0])[2].flags!=0xc1)return false;
        std::vector<std::byte> inherited(4+40+40*4);
        auto set=[&](std::size_t at,U32 v){for(unsigned i=0;i<4;++i)inherited[at+i]=std::byte((v>>(8*i))&255);};
        set(0,40);inherited[4+35]=std::byte{9};set(44+35*4,222);
        DescriptorTable inherited_table;initialize_descriptors(inherited_table,inherited,d);
        if(inherited_table.limit!=64 || (*inherited_table.blocks[1])[3].handle!=222)return false;
        std::array<Descriptor32,5> rows{};IoState state{rows,0,0,true};Io io;
        constexpr ErrorPair errors[]{{5,13}};
        rows[1]={400,1,{}};rows[2]={400,1,{}};
        if(close_descriptor(state,1,io,d,errors) || !d.closed.empty() || rows[1].flags)return false;
        rows[4]={500,3,{}};
        if(seek_descriptor(state,4,-20,2,d,errors)!=88 || rows[4].flags!=1 || d.seek_offset!=-20)return false;
        d.seek_result=0xffffffffU;
        if(seek_descriptor(state,4,0,0,d,errors)!=0xffffffffU || state.error!=13 || state.dos_error!=5)return false;
        d.close_ok=false;
        if(close_descriptor(state,4,io,d,errors)!=-1 || rows[4].flags || rows[4].handle!=0xffffffffU || state.error!=13)return false;
        if(close_descriptor(state,999,io,d,errors)!=-1 || state.error!=9 || state.dos_error)return false;
        rows[4]={501,1,{}};d.close_ok=true;
        BufferedIoLink link(state,io,{});auto callbacks=connect_close(link,state,io,d,errors);
        original_crt10::File file{};file.flags=2;file.file_index=4;
        if(original_crt10::fclose_original(file,callbacks)!=0 || rows[4].flags || file.flags || d.closed.back()!=501)return false;
        return p.live.empty();
    } catch(...) {return false;}
}
}
