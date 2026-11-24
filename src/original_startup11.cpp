#include "btb/original_startup11.hpp"
#include "btb/original_crt_batch_32.hpp"
#include <bit>
#include <algorithm>
#include <cstring>
#include <limits>
#include <stdexcept>
namespace btb::original_startup11 {
ParsedArguments parse_command_line(const char* input,std::span<const std::uint8_t,257> flags,bool count_only) {
    if(!input)throw std::invalid_argument("missing command line");
    const auto* p=reinterpret_cast<const unsigned char*>(input);
    ParsedArguments out;std::string text;
    auto lead=[&](unsigned char c){return (flags[c+1]&4)!=0;};
    auto emit=[&](unsigned char c){text.push_back(static_cast<char>(c));++out.byte_count;};
    if(*p=='"') {
        ++p;
        while(*p && *p!='"') {
            if(lead(*p)) {
                if(count_only)++out.byte_count;
                else {emit(*p++);if(!*p)throw std::invalid_argument("truncated lead byte");}
            }
            emit(*p++);
        }
        ++out.byte_count;if(*p=='"')++p;
    } else {
        for(;;) {
            const auto c=*p++;emit(c);
            if(lead(c)){if(!*p)throw std::invalid_argument("truncated lead byte");emit(*p++);}
            if(c==' ' || c=='\t'){text.back()='\0';break;}
            if(!c){--p;break;}
        }
        text.pop_back();
    }
    out.values.push_back(text);
    bool quoted=false;
    while(*p) {
        while(*p==' ' || *p=='\t')++p;
        if(!*p)break;
        text.clear();
        for(;;) {
            bool copy=true;unsigned slashes=0;
            while(*p=='\\'){++p;++slashes;}
            if(*p=='"') {
                if(!(slashes&1)) {
                    if(quoted && p[1]=='"')++p;
                    else copy=false;
                    quoted=!quoted;
                }
                slashes/=2;
            }
            while(slashes--)emit('\\');
            if(!*p || (!quoted && (*p==' ' || *p=='\t')))break;
            if(copy) {
                if(lead(*p)){emit(*p++);if(!*p)throw std::invalid_argument("truncated lead byte");}
                emit(*p);
            }
            ++p;
        }
        ++out.byte_count;out.values.push_back(text);
    }
    out.pointer_count=static_cast<U32>(out.values.size()+1);
    return out;
}
void initialize_arguments(Arguments& out,const char* command,bool initialized,
    std::span<const std::uint8_t,257> flags,ProcessServices& svc) {
    if(!initialized)svc.initialize_multibyte();
    out.program=svc.module_file_name(260);
    if(!command || !*command)command=out.program.c_str();
    auto parsed=parse_command_line(command,flags,true);
    const auto bytes=U64(parsed.pointer_count)*sizeof(char*)+parsed.byte_count;
    if(bytes>std::numeric_limits<U32>::max())svc.fatal_runtime_error(8);
    out.allocation=svc.allocate(static_cast<U32>(bytes));
    if(!out.allocation)svc.fatal_runtime_error(8);
    auto** pointers=static_cast<char**>(out.allocation);
    auto* data=reinterpret_cast<char*>(pointers+parsed.pointer_count);
    // Second pass is intentional: preserve original caller construction.
    auto filled=parse_command_line(command,flags);
    if(filled.values.size()+1>parsed.pointer_count)throw std::invalid_argument("original DBCS count/fill disagreement");
    out.argv.clear();
    for(std::size_t i=0;i<filled.values.size();++i) {
        pointers[i]=data;out.argv.push_back(data);
        std::memcpy(data,filled.values[i].c_str(),filled.values[i].size()+1);
        data+=filled.values[i].size()+1;
    }
    pointers[filled.values.size()]=nullptr;out.argv.push_back(nullptr);
    out.argc=static_cast<I32>(parsed.pointer_count)-1;
}
void initialize_environment(Environment& out,bool initialized,ProcessServices& svc) {
    if(!initialized)svc.initialize_multibyte();
    if(!out.raw)throw std::invalid_argument("missing original environment block");
    U32 count=0;
    for(const char* p=out.raw;*p;p+=std::strlen(p)+1)if(*p!='=')++count;
    out.values=static_cast<char**>(svc.allocate(static_cast<U32>((U64(count)+1)*sizeof(char*))));
    if(!out.values)svc.fatal_runtime_error(9);
    U32 at=0;
    for(const char* p=out.raw;*p;p+=std::strlen(p)+1) {
        if(*p=='=')continue;
        const auto bytes=std::strlen(p)+1;
        out.values[at]=static_cast<char*>(svc.allocate(static_cast<U32>(bytes)));
        if(!out.values[at])svc.fatal_runtime_error(9);
        original_crt32::copy_cstr(out.values[at++],p);
    }
    svc.release(out.raw);out.raw=nullptr;out.values[at]=nullptr;out.initialized=true;
}
void process_exit(ExitState& state,U32 code,bool quick,U32 returning,
    std::span<const Initializer> on_exit,std::span<const Initializer> pre,
    std::span<const Initializer> final,ProcessServices& svc) {
    if(state.already_exiting)svc.terminate_process(code);
    state.cleanup_started=true;state.return_to_caller=static_cast<std::uint8_t>(returning);
    if(!quick) {
        for(auto i=on_exit.rbegin();i!=on_exit.rend();++i)if(*i)(*i)();
        original_crt32::run_initializers(pre);
    }
    original_crt32::run_initializers(final);
    if(returning)return;
    state.already_exiting=true;svc.exit_process(code);
}
char* concatenate(char* out,const char* source) noexcept {
    char* at=out;while(*at)++at;
    return original_crt32::copy_cstr(at,source),out;
}
U32 adjust_pointer(U32 object,const std::array<I32,3>& pmd,GuestMemory& memory) {
    U32 result=object+static_cast<U32>(pmd[0]);
    if(pmd[1]>=0)result+=memory.read32(memory.read32(object+static_cast<U32>(pmd[1]))+
        static_cast<U32>(pmd[2]))+static_cast<U32>(pmd[1]);
    return result;
}
void* allocate_zero(U32 count,U32 size,U32 mode,U32 limit,bool retry,AllocationServices& heap) {
    const U32 requested=count*size;
    U32 rounded=requested;
    if(rounded<0xffffffe1U)rounded=((rounded?rounded:1)+15)&0xfffffff0U;
    for(;;) {
        void* out=nullptr;
        if(rounded<0xffffffe1U) {
            if(mode==3 && requested<=limit) {
                out=heap.small_heap3(requested);
                if(out)return std::memset(out,0,requested);
            } else if(mode==2 && rounded<=limit) {
                out=heap.small_heap2(rounded>>4);
                if(out)return std::memset(out,0,rounded);
            }
            out=heap.zero_heap(rounded);
            if(out)return out;
        }
        if(!retry || !heap.new_handler(rounded))return nullptr;
    }
}
namespace {
void initialize_block(DescriptorBlock& block) {
    for(auto& d:block){d.handle=0xffffffffU;d.flags=0;d.reserved[0]=10;}
}
U32 read_word(std::span<const std::byte> data,std::size_t at) {
    if(at>data.size() || data.size()-at<4)throw std::invalid_argument("truncated inherited descriptors");
    U32 v=0;for(unsigned i=0;i<4;++i)v|=std::to_integer<U32>(data[at+i])<<(8*i);return v;
}
I32 invalid(IoState& s){s.error=9;s.dos_error=0;return -1;}
void map_error(IoState& s,U32 error,std::span<const ErrorPair> table){
    s.dos_error=error;s.error=original_runtime8::map_win_error(std::bit_cast<I32>(error),table);
}
}
I32 allocate_descriptor(DescriptorTable& table,DescriptorServices& svc) {
    for(unsigned b=0;b<64;++b) {
        auto*& block=table.blocks[b];
        if(!block) {
            block=svc.allocate_block();
            if(!block)return -1;
            table.limit+=32;initialize_block(*block);return static_cast<I32>(b*32);
        }
        for(unsigned i=0;i<32;++i)if(!((*block)[i].flags&1)) {
            (*block)[i].handle=0xffffffffU;return static_cast<I32>(b*32+i);
        }
    }
    return -1;
}
void initialize_descriptors(DescriptorTable& table,std::span<const std::byte> inherited,DescriptorServices& svc) {
    table.blocks[0]=svc.allocate_block();
    if(!table.blocks[0])throw std::bad_alloc{}; // host equivalent of fatal CRT error 27
    table.limit=32;initialize_block(*table.blocks[0]);
    if(!inherited.empty()) {
        const U32 original_count=read_word(inherited,0);
        const U32 count=std::min(original_count,U32{2048});
        const U64 handles_at=4ULL+original_count;
        if(handles_at+U64(count)*4>inherited.size())throw std::invalid_argument("truncated inherited handle table");
        while(table.limit<count) {
            auto* block=svc.allocate_block();if(!block)break;
            initialize_block(*block);table.blocks[table.limit/32]=block;table.limit+=32;
        }
        for(U32 i=0;i<std::min(count,table.limit);++i) {
            const auto flags=std::to_integer<std::uint8_t>(inherited[4+i]);
            const U32 handle=read_word(inherited,static_cast<std::size_t>(handles_at)+i*4);
            if(handle!=0xffffffffU && (flags&1) && ((flags&8) || svc.file_type(handle))) {
                auto& d=(*table.blocks[i/32])[i%32];d.handle=handle;d.flags=flags;
            }
        }
    }
    for(U32 i=0;i<3;++i) {
        auto& d=(*table.blocks[0])[i];
        if(d.handle!=0xffffffffU){d.flags|=0x80;continue;}
        d.flags=0x81;
        const auto handle=svc.standard_handle(0xfffffff6U-i);
        const auto type=handle==0xffffffffU?0:svc.file_type(handle);
        if(handle!=0xffffffffU && type) {
            d.handle=handle;
            if((type&255)!=2){if((type&255)==3)d.flags|=8;continue;}
        }
        d.flags|=0x40;
    }
    svc.set_handle_count(table.limit);
}
I32 close_descriptor(IoState& s,U32 fd,IoServices& io,DescriptorServices& svc,std::span<const ErrorPair> errors) {
    if(fd>=s.descriptors.size() || !(s.descriptors[fd].flags&1))return invalid(s);
    U32 error=0;
    const auto handle=s.descriptors[fd].handle;
    if(handle!=0xffffffffU) {
        bool shared_standard=false;
        if((fd==1 || fd==2) && s.descriptors.size()>2) {
            // Both original _get_osfhandle calls validate open flags.
            auto get=[&](U32 i){if(!(s.descriptors[i].flags&1)){invalid(s);return 0xffffffffU;}
                return s.descriptors[i].handle;};
            const U32 two=get(2),one=get(1);shared_standard=one==two;
        }
        if(!shared_standard && !svc.close_handle(handle))error=svc.last_error();
    }
    (void)release_os_handle(s,fd,io);s.descriptors[fd].flags=0;
    if(error){map_error(s,error,errors);return -1;}
    return 0;
}
U32 seek_descriptor(IoState& s,U32 fd,I32 offset,U32 origin,DescriptorServices& svc,std::span<const ErrorPair> errors) {
    if(fd>=s.descriptors.size() || !(s.descriptors[fd].flags&1)){invalid(s);return 0xffffffffU;}
    if(s.descriptors[fd].handle==0xffffffffU){s.error=9;return 0xffffffffU;}
    const U32 position=svc.seek_handle(s.descriptors[fd].handle,offset,origin);
    const U32 error=position==0xffffffffU?svc.last_error():0;
    if(error){map_error(s,error,errors);return 0xffffffffU;}
    s.descriptors[fd].flags&=0xfd;return position;
}
btb::original_crt10::Io connect_close(BufferedIoLink& link,IoState& state,IoServices& io,
    DescriptorServices& descriptors,std::span<const ErrorPair> errors) {
    link.set_descriptor_close([&state,&io,&descriptors,errors](U32 fd){
        return close_descriptor(state,fd,io,descriptors,errors);
    });
    return link.callbacks();
}

}
