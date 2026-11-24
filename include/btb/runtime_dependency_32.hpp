#pragma once
// 32 authentic MSVC2003/Win32 runtime procedures from the original game.
// Source evidence: analysis/ghidra_main_exe/functions/{ADDRESS}.c.
// PE SHA256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
// Typed portable C++26 source-level branch models. Host hooks explicitly
// own all original Windows I/O, SEH, allocator and FPU operations.
// The models are not original x86 ABI or byte-for-byte CRT replacements.
#include <algorithm>
#include <array>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <functional>
#include <limits>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace btb::runtime_batch_32 {
using I32=std::int32_t;
using U32=std::uint32_t;
using U16=std::uint16_t;
using U8=std::uint8_t;
using U64=std::uint64_t;
using Callback=std::function<void(I32)>;

// 00430196: install function pointers, check IsProcessorFeaturePresent(0),
// and configure floating point precision, in that exact order.
inline void initialize_crt_runtime(const std::function<void()>& init_pointers,
    const std::function<bool()>& processor_check, bool& flag,
    const std::function<void()>& precision) {
    if(init_pointers)init_pointers();
    flag=processor_check?processor_check():false;
    if(precision)precision();
}

// 00436010 (processor warning sequence); 004310E0 (__amsg_exit);
// 00431105 (fatal Windows ExitProcess). All follow startup latch.
inline void runtime_processor_warning(I32 startup,I32 alternate,
    const Callback& error,const std::function<void()>& hook) {
    if(startup==1||(startup==0&&alternate==1)){
        if(error)error(252);
        if(hook)hook();
        if(error)error(255);
    }
}
inline void runtime_amessage_exit(I32 startup,I32 message,
    const std::function<void()>& prep,
    const Callback& report,const Callback& terminate) {
    if(startup==1&&prep)prep();
    if(report)report(message);
    if(terminate)terminate(255);
}
inline void runtime_fatal_process(I32 startup,I32 message,
    const std::function<void()>& prep,
    const Callback& report,const Callback& exitprocess) {
    if(startup==1&&prep)prep();
    if(report)report(message);
    if(exitprocess)exitprocess(255);
}

// 004331CB: use kernel32 dynamically if exported; fallback to modular
// CPU flag calculation through prior 0043318D source implementation.
inline bool check_processor_feature(
    const std::function<std::optional<bool>()>& kernel32_query,
    const std::function<bool()>& fallback) {
    if(kernel32_query) {
        const auto available=kernel32_query();
        if(available)return *available;
    }
    return fallback?fallback():false;
}

// 00433596: the global CRT flag is set *around* 004333F6 call.
inline void scoped_float_conversion(
    U32& guard,U32 fmt,char* output,std::size_t size,
    const std::function<void(U32,char*,std::size_t)>& format) {
    guard=1;if(format)format(fmt,output,size);guard=0;
}

// 00436EE9: clear 256-byte locale classification table and its +256
// sentinel, and six original multibyte state globals.
struct LocaleState{
    std::array<U8,257> classification{};
    U32 c6b8{},c6cc{},c8e4{},c6c0{},c6c4{},c6c8{};
};
inline void reset_locale_state(LocaleState& state){state=LocaleState{};}

// 00436E6C: locale codepage request -2 OEM,-3 ANSI,-4 stored codepage.
inline I32 choose_codepage(I32 requested,I32 current,U32& use_system_cp,
    const std::function<I32()>& oem,const std::function<I32()>& ansi){
    if(requested==-2){use_system_cp=1;return oem?oem():0;}
    if(requested==-3){use_system_cp=1;return ansi?ansi():0;}
    const bool reuse=requested==-4;
    use_system_cp=reuse?1U:0U;
    return reuse?current:requested;
}

// 004311DC: freebuf if _flag &0x83 AND &0x8, clear 0x0408 flag bits,
// preserve all unrelated native FILE fields.
struct File{
    I32 count{},flags{},file_index{};
    U32 pointer{},base{},tmpname{};
};
inline bool free_crt_buffer(File& f,const std::function<void(U32)>& dealloc){
    if((f.flags&0x83)==0||(f.flags&8)==0)return false;
    if(dealloc)dealloc(f.base);
    f.flags&=~0x0408;
    f.pointer=0;f.base=0;f.count=0;return true;
}

// 0042FB62: fclose rejects flag0x40, optionally flush, free buffer,
// close OS file, then always zero FILE flags.
inline I32 close_crt_file(File& f,
    const std::function<I32(File&)>& flush,
    const std::function<void(U32)>& freebuffer,
    const std::function<I32(I32)>& closehandle) {
    I32 result=-1;
    if((f.flags&0x40)==0&&(f.flags&0x83)!=0){
        result=flush?flush(f):0;
        free_crt_buffer(f,freebuffer);
        if(closehandle&&closehandle(f.file_index)<0)result=-1;
        else if(f.tmpname!=0){if(freebuffer)freebuffer(f.tmpname);f.tmpname=0;}
    }
    f.flags=0;
    return result;
}

// 00431207 flush_one and 004312A7 flsall preserve legacy return values.
inline I32 flush_one(File* f,
    const std::function<I32()>& flushall,
    const std::function<I32(File&)>& flush,
    const std::function<I32(I32)>& sync_handle) {
    if(!f)return flushall?flushall():0;
    if(flush&&flush(*f)!=0)return -1;
    if((f->flags&0x40)!=0)
        return (sync_handle&&sync_handle(f->file_index)!=0)?-1:0;
    return 0;
}
inline I32 flush_all(std::span<File*> files,I32 mode,
    const std::function<I32(File*)>& flush) {
    I32 good=0,failed=0;
    for(auto* f:files){
        if(!f||(f->flags&0x83)==0)continue;
        if(mode==1){if(!flush||flush(f)!=-1)++good;}
        else if(mode==0&&(f->flags&2)!=0){
            if(flush&&flush(f)==-1)failed=-1;
        }
    }
    return mode==1?good:failed;
}

// 00430FB8: acquire stream flag -> scan -> 00435732 release/reset.
// Original scan itself is the separate 00432971 dependency.
inline I32 formatted_scan_wrapper(File& f,const char* fmt,
    const std::function<I32(File&)>& lock,
    const std::function<I32(File&,const char*)>& scan,
    const std::function<void(I32,File&)>& reset) {
    const auto state=lock?lock(f):0;
    const auto result=scan?scan(f,fmt):-1;
    if(reset)reset(state,f);
    return result;
}

// 00430CB0: native selection-sort passes from last record backward;
// comparison chooses >0 (maximum), then original 00430CFE swaps.
template<class T,class Compare>
inline void selection_sort_original(std::span<T> values,Compare compare){
    for(auto n=values.size();n>1;--n){
        std::size_t candidate=0;
        for(std::size_t j=1;j<n;++j)
            if(compare(values[j],values[candidate])>0)candidate=j;
        std::swap(values[candidate],values[n-1]);
    }
}

// 004392DE: multibyte ignore-case collation: zero count => 0;
// NLS CompareString returning zero => INT_MAX, else return NLS-2.
inline I32 mbsnbicoll(std::size_t max_count,
    const std::function<I32(std::size_t)>& compare) {
    if(max_count==0)return 0;
    const I32 nls=compare?compare(max_count):0;
    return nls==0?std::numeric_limits<I32>::max():nls-2;
}

// 00439ACC: find environment variable by collated prefix and '=' or '\0'.
inline I32 environment_find(std::span<const std::string> env,
    std::string_view name,
    const std::function<I32(std::string_view,std::string_view)>& compare) {
    I32 i=0;
    for(const auto& candidate:env){
        if(candidate.size()>=name.size() &&
           (!compare||compare(name,
              std::string_view(candidate).substr(0,name.size()))==0) &&
           (candidate.size()==name.size()||candidate[name.size()]=='='))
            return i;
        ++i;
    }
    return -i; // original missing key => negative one-past-last
}

// 004343FC: reserve a 0x140-byte heap slab then install original state.
struct HeapGlobals{U32 handle{},root{},current{},control{},step{},count{},next{};};
inline bool initialize_heap_slab(HeapGlobals& s,U32 mode,
    const std::function<U32(std::size_t)>& allocate){
    const U32 slab=allocate?allocate(0x140):0;
    if(!slab)return false;
    s.root=slab;s.current=slab;s.count=0;s.next=0;
    s.control=mode;s.step=0x10;
    return true;
}

// 0043439F: create process heap and choose allocation algorithm
// (mode 3 slab, 2 small-block heap, else success without next layer).
inline bool initialize_runtime_heap(HeapGlobals& s,bool isolated,
    const std::function<U32(bool,std::size_t)>& createheap,
    const std::function<I32()>& algorithm,
    const std::function<bool(I32)>& initialize,
    const std::function<void(U32)>& destroy) {
    s.handle=createheap?createheap(isolated,0x1000):0;
    if(!s.handle)return false;
    const I32 mode=algorithm?algorithm():0;
    if(mode==2||mode==3){
        if(!initialize||!initialize(mode)){
            if(destroy)destroy(s.handle);
            return false;
        }
    }
    return true;
}

// 00437371: 4 KiB CRT stream allocation, falls back to inline 2-byte buffer.
struct StreamBuffer {
    U32 ptr{},base{},flags{},capacity{},count{};
    U32 inline_buffer{};
};
inline void allocate_crt_stream_buffer(StreamBuffer& s,
    U32& global_alloc_count,const std::function<U32(std::size_t)>& allocate){
    ++global_alloc_count;
    s.base=allocate?allocate(0x1000):0;
    if(s.base){s.flags|=8;s.capacity=4096;}
    else{s.flags|=4;s.base=s.inline_buffer;s.capacity=2;}
    s.count=0;s.ptr=s.base;
}

// 004351F6: small-block free list index and byte table update.
// Original callback 004350DD(16) occurs after 32 freed full pages.
inline void release_heap_slot(U32& available,U8& slot_byte,
    U8& page_indicator,U32& full_page_count,
    const Callback& reclaim) {
    available+=slot_byte;slot_byte=0;page_indicator=0xf1;
    if(available==0xf0 && ++full_page_count==32) {
        if(reclaim)reclaim(16);
    }
}

// 004358F3: argv tokenizer over quoted or unquoted Win32 string,
// where 00438230 returns a nonzero DBCS lead-class value.
inline std::size_t next_command_token_end(std::string_view arg,
    std::size_t offset,const std::function<bool(U8)>& lead_byte) {
    if(offset>=arg.size())return offset;
    std::size_t pos=offset;
    if(arg[pos]=='"'){
        ++pos;
        while(pos<arg.size()&&arg[pos]!='"'){
            if(lead_byte&&lead_byte(static_cast<U8>(arg[pos])) &&
               pos+1<arg.size())pos+=2;
            else ++pos;
        }
    }else while(pos<arg.size()&&static_cast<U8>(arg[pos])>32)++pos;
    // Quotes are skipped, then ASCII <=32 is consumed.
    if(pos<arg.size()&&arg[pos]=='"')++pos;
    while(pos<arg.size()&&static_cast<U8>(arg[pos])<=32)++pos;
    return pos;
}

// 004325C0: notify 004325F7, call original cleanup, then notify frame.
// HRESULT-like status 0x100 is normalized to 2 in the final notification.
inline void call_setting_frame(I32 status,const Callback& before,
    const std::function<void()>& cleanup,const Callback& after){
    if(before)before(status);
    if(cleanup)cleanup();
    if(after)after(status==0x100?2:status);
}

// 004332B4: format-assignment dispatch: flag0 writes two result words;
// flag!=0 selects 00437A06, otherwise 00437A33.
inline std::array<U32,2> assign_float_result(bool extended,
    const std::function<std::array<U32,2>()>& two_words,
    const std::function<U32()>& one_word){
    if(extended)return two_words?two_words():std::array<U32,2>{};
    return {one_word?one_word():0,0};
}

// 00430F5C: signed radix writer. The original accepts radix up to 36,
// uses 'W'+digit for >=10 => lowercase letters.
inline std::optional<std::string> write_radix(U32 value,U32 radix,bool negative){
    if(radix<2||radix>36)return std::nullopt;
    if(negative)value=0U-value;
    std::string out;
    do{
        const U32 digit=value%radix;value/=radix;
        out.push_back(static_cast<char>(digit<10?'0'+digit:'W'+digit));
    }while(value);
    if(negative)out.push_back('-');
    std::reverse(out.begin(),out.end());
    return out;
}

// 00433720 strlen, 004337A0 strcmp, 004381B0 strstr.
// Portable bytewise C++ yields same defined result as optimized x86 loops.
inline std::size_t retail_strlen(const char* p) {
    std::size_t n=0;while(p[n])++n;return n;
}
inline I32 retail_strcmp(const char* a,const char* b){
    for(std::size_t i=0;;++i){
        const U8 left=static_cast<U8>(a[i]),right=static_cast<U8>(b[i]);
        if(left!=right)return left<right?-1:1;
        if(left==0)return 0;
    }
}
inline const char* retail_strstr(const char* text,const char* needle) {
    if(needle[0]==0)return text;
    for(std::size_t i=0;text[i];++i){
        std::size_t j=0;
        while(needle[j]&&text[i+j]&&needle[j]==text[i+j])++j;
        if(!needle[j])return text+i;
    }
    return nullptr;
}

// 00439E2B: SBCS strchr or MBCS lead-byte-aware lookahead.
inline const U8* multibyte_strchr(const U8* text,U32 needle,bool mbcs,
    std::span<const U8> lead_flags) {
    const auto target=needle&0xffffU;
    for(std::size_t i=0;;){
        const U8 first=text[i];
        if(first==0)return target==0?text+i:nullptr;
        if(mbcs&&first<lead_flags.size()&&(lead_flags[first]&4)!=0){
            if(text[i+1]==0)return nullptr;
            if(target==((static_cast<U32>(first)<<8)|text[i+1]))return text+i;
            i+=2;
        }else{
            if(target==first)return text+i;
            ++i;
        }
    }
}

// 0043277A: Win32 GetLastError -> errno mapping (retail map length is
// source-owned and injected; preserve fallback error categories).
inline I32 translate_winerror(
    I32 code,std::span<const std::pair<I32,I32>> mappings) {
    for(const auto& [win,err]:mappings)if(win==code)return err;
    if(code>=19&&code<37)return 13;
    if(code>=188&&code<=202)return 8;
    return 22;
}

// 00439B24: duplicate null-terminated CRT environment entries through
// native 00439E9E copier; do not fabricate success when malloc fails.
inline std::optional<std::vector<std::string>> duplicate_environment(
    const std::vector<std::string>* env,
    const std::function<std::optional<std::string>(const std::string&)>& copy){
    if(!env)return std::nullopt;
    std::vector<std::string> out;
    for(const auto& src:*env){
        if(copy){
            const auto cloned=copy(src);
            if(!cloned)return std::nullopt;
            out.push_back(*cloned);
        }else out.push_back(src);
    }
    return out;
}

// 004373DB: WC->MBC; source returns byte count or UINT_MAX and sets
// errno=42 if invalid/unconvertible. CP and WinAPI result come from host.
inline I32 convert_wide_character(char* dest,U16 wchar,
    bool multibyte,I32& err,
    const std::function<std::optional<std::string>(U16)>& converter){
    if(!dest)return 0;
    if(!multibyte&&wchar<256){
        dest[0]=static_cast<char>(wchar);return 1;
    }
    if(multibyte&&converter){
        const auto value=converter(wchar);
        if(value&&!value->empty()&&value->size()<=4){
            std::memcpy(dest,value->data(),value->size());
            return static_cast<I32>(value->size());
        }
    }
    err=42;return -1;
}

// 0043748E original 12-bit x87 control mask remapping, no FPU mutation.
inline U32 translate_fpu_control(U32 flags){
    U32 out=0;
    if(flags&1)out|=0x10;
    if(flags&4)out|=8;
    if(flags&8)out|=4;
    if(flags&16)out|=2;
    if(flags&32)out|=1;
    if(flags&2)out|=0x80000;
    const U32 rounding=flags&0xc00;
    if(rounding==0x400)out|=0x100;
    if(rounding==0x800)out|=0x200;
    if(rounding==0xc00)out|=0x300;
    if((flags&0x300)==0)out|=0x20000;
    else if((flags&0x300)==0x200)out|=0x10000;
    if(flags&0x1000)out|=0x40000;
    return out;
}
} // namespace btb::runtime_batch_32
