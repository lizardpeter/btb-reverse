#include "btb/original_runtime16.hpp"
#include "btb/original_crt_batch_32.hpp"
#include <bit>
#include <cstring>
#include <stdexcept>

namespace btb::original_runtime16 {
namespace {
I32 bad_handle(IoState& s) {s.dos_error=0;s.error=9;return -1;}
constexpr U32 std_handle(U32 descriptor) {return 0xfffffff6U-descriptor;}
}
I32 assign_os_handle(IoState& s,U32 fd,U32 handle,IoServices& io) {
    if(fd>=s.descriptors.size() || s.descriptors[fd].handle!=0xffffffffU)return bad_handle(s);
    if(s.console_application && fd<3)io.set_standard_handle(std_handle(fd),handle);
    s.descriptors[fd].handle=handle;return 0;
}
I32 release_os_handle(IoState& s,U32 fd,IoServices& io) {
    if(fd>=s.descriptors.size() || !(s.descriptors[fd].flags&1) ||
       s.descriptors[fd].handle==0xffffffffU)return bad_handle(s);
    if(s.console_application && fd<3)io.set_standard_handle(std_handle(fd),0);
    s.descriptors[fd].handle=0xffffffffU;return 0;
}
I32 commit_os_handle(IoState& s,U32 fd,IoServices& io) {
    U32 error=s.dos_error;
    if(fd<s.descriptors.size() && (s.descriptors[fd].flags&1)) {
        error=io.flush_file(s.descriptors[fd].handle)?0:io.last_error();
        if(error==0)return 0;
    }
    // Unlike attach/detach, invalid commit preserves the previous DOS error.
    s.dos_error=error;s.error=9;return -1;
}
I32 set_descriptor_mode(IoState& s,U32 fd,I32 mode) {
    if(fd>=s.descriptors.size() || !(s.descriptors[fd].flags&1)) {
        s.error=9;return -1;
    }
    auto& flags=s.descriptors[fd].flags;
    const auto before=flags;
    if(mode==0x8000)flags&=0x7f;
    else if(mode==0x4000)flags|=0x80;
    else {s.error=22;return -1;}
    return before&0x80?0x4000:0x8000;
}
Stream32* acquire_stream(std::span<Stream32*> streams,IoServices& io) {
    for(auto& slot:streams) {
        if(!slot) {
            slot=io.allocate_stream(0x20);
            if(!slot)return nullptr; // do not continue into later slots
        } else if(slot->words[3]&0x83)continue;
        auto& w=slot->words;
        w[4]=0xffffffffU;w[1]=0;w[3]=0;w[2]=0;w[0]=0;w[7]=0;
        // Original words 5 and 6 survive reuse.
        return slot;
    }
    return nullptr;
}
I32 flush_stream(Stream32& stream,IoServices& io) {
    auto& w=stream.words;I32 result=0;
    if((w[3]&3)==2 && (w[3]&0x108)) {
        const U32 pending=w[0]-w[2];
        if(std::bit_cast<I32>(pending)>0) {
            if(io.write_descriptor(w[4],w[2],pending)==pending) {
                if(w[3]&0x80)w[3]&=0xfffffffdU;
            } else {w[3]|=0x20;result=-1;}
        }
    }
    w[1]=0;w[0]=w[2];return result;
}
I32 begin_standard_buffering(Stream32& stream,U32 address,StandardStream kind,
    std::array<U32,2>& buffers,U32& attempts,IoServices& io) {
    auto& w=stream.words;
    if(!io.is_terminal(w[4]) || kind==StandardStream::Other)return 0;
    ++attempts;
    if(w[3]&0x10c)return 0;
    const std::size_t i=kind==StandardStream::Out?0:1;
    if(!buffers[i])buffers[i]=io.allocate_guest_bytes(0x1000);
    if(!buffers[i]) {
        w[2]=address+20;w[0]=address+20;w[6]=2;w[1]=2;
    } else {
        w[6]=0x1000;w[2]=buffers[i];w[0]=buffers[i];w[1]=0x1000;
    }
    // The source writes only the low 16 bits of the flag word.
    w[3]|=0x1102;return 1;
}
U64 unsigned_remainder(U64 numerator,U64 denominator) {
    if(!denominator)throw std::domain_error("original integer divide by zero");
    const U32 lo=static_cast<U32>(numerator),hi=static_cast<U32>(numerator>>32);
    const U32 dlo=static_cast<U32>(denominator),dhi=static_cast<U32>(denominator>>32);
    if(!dhi)return ((static_cast<U64>(hi%dlo)<<32)|lo)%dlo;
    U64 scaled_n=numerator,scaled_d=denominator;
    do{scaled_n>>=1;scaled_d>>=1;}while(scaled_d>>32);
    const U32 quotient=static_cast<U32>(scaled_n/static_cast<U32>(scaled_d));
    const U64 low_product=static_cast<U64>(quotient)*dlo;
    const U32 high_product=quotient*dhi;
    const U32 carry_part=static_cast<U32>(low_product>>32);
    const U32 high_sum=carry_part+high_product;
    U64 product=(static_cast<U64>(high_sum)<<32)|static_cast<U32>(low_product);
    if(high_sum<carry_part || product>numerator)product-=denominator;
    return numerator-product;
}
char* find_character(char* text,int value) noexcept {
    const auto wanted=static_cast<unsigned char>(value);
    for(;;++text) {
        const auto byte=static_cast<unsigned char>(*text);
        if(byte==wanted)return text;
        if(byte==0)return nullptr;
    }
}
const ActionRecord* find_action_record(std::span<const ActionRecord> table,I32 signal) noexcept {
    for(const auto& record:table)if(record.signal==signal)return &record;
    return nullptr;
}
U32 read_linker_version(std::span<const std::byte> module) {
    if(module.size()<2)throw std::out_of_range("short original DOS header");
    if(module[0]!=std::byte{'M'} || module[1]!=std::byte{'Z'})return 0;
    if(module.size()<64)throw std::out_of_range("short original DOS header");
    U32 offset=0;
    for(unsigned i=0;i<4;++i)offset|=std::to_integer<U32>(module[60+i])<<(i*8);
    if(!offset)return 0;
    if(offset>module.size() || module.size()-offset<28)
        throw std::out_of_range("short original PE optional header");
    return std::to_integer<U32>(module[offset+26])|
        (std::to_integer<U32>(module[offset+27])<<8);
}
bool catch_type_matches(const CatchType& thrown,const CatchType& caught,U32 qualifiers) {
    if(!thrown.type)return true;
    if(!thrown.type->decorated_name)throw std::invalid_argument("missing original thrown type name");
    if(!*thrown.type->decorated_name)return true;
    if(!caught.type || !caught.type->decorated_name)
        throw std::invalid_argument("missing original catch type");
    if(thrown.type!=caught.type && std::strcmp(thrown.type->decorated_name,caught.type->decorated_name))return false;
    return (!(caught.properties&2) || (thrown.properties&8)) &&
        (!(qualifiers&1) || (thrown.properties&1)) &&
        (!(qualifiers&2) || (thrown.properties&2));
}
void round_decimal_digits(std::span<char> out,I32 count,std::string_view digits,I32& point) {
    const std::size_t n=count>0?static_cast<std::size_t>(count):0;
    if(out.size()<n+2)throw std::out_of_range("short original decimal buffer");
    out[0]='0';std::size_t source=0;
    for(std::size_t i=1;i<=n;++i) {
        if(source<digits.size() && digits[source]!='\0')out[i]=digits[source++];
        else out[i]='0';
    }
    out[n+1]='\0';
    if(count>=0 && source<digits.size() && digits[source]>'4') {
        std::size_t i=n;
        while(out[i]=='9'){out[i]='0';--i;}
        ++out[i];
    }
    if(out[0]=='1')++point;
    else std::memmove(out.data(),out.data()+1,n+1);
}
btb::original_crt17::Exception& construct_exception(
    btb::original_crt17::Exception& out,const char* message,Allocate allocate) {
    if(!message || !allocate)throw std::invalid_argument("missing original exception allocator/message");
    out.vtable=0x0043b5bc;
    const auto length=std::strlen(message)+1;
    out.message=allocate(length);
    if(out.message)std::memcpy(out.message,message,length);
    out.owned=1;return out;
}
btb::original_crt17::Exception& copy_exception(
    btb::original_crt17::Exception& out,const btb::original_crt17::Exception& source,Allocate allocate) {
    out.vtable=0x0043b5bc;out.owned=source.owned;
    if(!source.owned)out.message=source.message;
    else {
        if(!source.message || !allocate)throw std::invalid_argument("missing original owned exception message");
        const auto length=std::strlen(static_cast<const char*>(source.message))+1;
        out.message=allocate(length);
        if(out.message)std::memcpy(out.message,source.message,length);
    }
    return out;
}
void run_runtime_initializers(Initializer hook,std::span<const Initializer> first,
    std::span<const Initializer> second) {
    if(hook)hook();
    btb::original_crt32::run_initializers(first);
    btb::original_crt32::run_initializers(second);
}
btb::original_crt10::Io BufferedIoLink::callbacks() {
    using File=btb::original_crt10::File;
    btb::original_crt10::Io out{};
    out.ctx=this;
    out.flush=+[](File& file,void* ctx)->I32 {
        auto& link=*static_cast<BufferedIoLink*>(ctx);
        Stream32 stream{{file.pointer,static_cast<U32>(file.count),file.base,
            static_cast<U32>(file.flags),static_cast<U32>(file.file_index),0,0,0}};
        const auto result=flush_stream(stream,link.services_);
        file.pointer=stream.words[0];file.count=std::bit_cast<I32>(stream.words[1]);
        file.flags=std::bit_cast<I32>(stream.words[3]);
        return result;
    };
    out.sync_handle=+[](I32 descriptor,void* ctx)->I32 {
        auto& link=*static_cast<BufferedIoLink*>(ctx);
        return commit_os_handle(link.state_,static_cast<U32>(descriptor),link.services_);
    };
    if(upstream_.deallocate)out.deallocate=+[](U32 address,void* ctx) {
        auto& base=static_cast<BufferedIoLink*>(ctx)->upstream_;base.deallocate(address,base.ctx);
    };
    if(close_)out.close_handle=+[](I32 fd,void* ctx)->I32 {
        return static_cast<BufferedIoLink*>(ctx)->close_(static_cast<U32>(fd));
    };
    else if(upstream_.close_handle)out.close_handle=+[](I32 fd,void* ctx)->I32 {
        auto& base=static_cast<BufferedIoLink*>(ctx)->upstream_;return base.close_handle(fd,base.ctx);
    };
    if(upstream_.lock)out.lock=+[](File& file,void* ctx)->I32 {
        auto& base=static_cast<BufferedIoLink*>(ctx)->upstream_;return base.lock(file,base.ctx);
    };
    if(upstream_.unlock)out.unlock=+[](I32 token,File& file,void* ctx) {
        auto& base=static_cast<BufferedIoLink*>(ctx)->upstream_;base.unlock(token,file,base.ctx);
    };
    if(upstream_.scan)out.scan=+[](File& file,const char* fmt,void* ctx)->I32 {
        auto& base=static_cast<BufferedIoLink*>(ctx)->upstream_;return base.scan(file,fmt,base.ctx);
    };
    return out;
}
} // namespace btb::original_runtime16
