#include "btb/original_crt10.hpp"
#include <array>
namespace btb::original_crt10 {
namespace {
struct Probe{
 I32 frees{},last_freed{},flushes{},closes{},syncs{},locks{},scans{},
     unlocks{},last_unlocked{},destroyed{},small_inits{};
 U32 allocated{},heap_create{},steps{},order[4]{};
 U32* conversion_guard{};
 bool allocate_ok{},slab_ok{true};
 I32 heap_mode{3};
 std::array<std::byte,4096> storage{};
};
Probe& p(void* ctx){return *static_cast<Probe*>(ctx);}
void free_hook(U32 address,void* ctx){
 auto& v=p(ctx);++v.frees;v.last_freed=static_cast<I32>(address);
}
I32 flush_hook(File&,void* ctx){++p(ctx).flushes;return 0;}
I32 close_hook(I32,void* ctx){++p(ctx).closes;return 0;}
I32 sync_hook(I32,void* ctx){++p(ctx).syncs;return 0;}
I32 lock_hook(File&,void* ctx){++p(ctx).locks;return 77;}
I32 scan_hook(File&,const char*,void* ctx){++p(ctx).scans;return 4;}
void unlock_hook(I32 lock,File&,void* ctx){
 ++p(ctx).unlocks;p(ctx).last_unlocked=lock;
}
I32 flush_all_hook(void* ctx){++p(ctx).flushes;return 12;}
void* allocate(std::size_t requested,void* ctx){
 auto& v=p(ctx);++v.allocated;
 return v.allocate_ok&&requested<=v.storage.size()?v.storage.data():nullptr;
}
U32 make_heap(bool isolated,std::size_t size,void* ctx){
 auto& v=p(ctx);++v.heap_create;
 return !isolated&&size==4096?88U:99U;
}
U32 alloc_slab(U32 handle,std::size_t size,void* ctx){
 auto& v=p(ctx);return v.slab_ok&&handle&&size==320?0x2222U:0;
}
I32 heap_mode(void* ctx){return p(ctx).heap_mode;}
bool init_small(void* ctx){++p(ctx).small_inits;return true;}
void destroy(U32,void* ctx){++p(ctx).destroyed;}
void pointers(void* ctx){auto& v=p(ctx);v.order[v.steps++]=1;}
bool cpu(void* ctx){auto& v=p(ctx);v.order[v.steps++]=2;return true;}
void precision(void* ctx){auto& v=p(ctx);v.order[v.steps++]=3;}
void format(U32 value,char* out,std::size_t size,void* ctx){
 auto& v=p(ctx);
 if(v.conversion_guard && *v.conversion_guard==1 && size>0)
    out[0]=static_cast<char>(value);
}
}
bool verify_10_original_crt_io_functions() noexcept {
 Probe p0{};
 Io io{&p0,free_hook,flush_hook,close_hook,sync_hook,lock_hook,
       unlock_hook,scan_hook};
 File original{7,0x8b,3,10,99,0};
 if(!free_file_buffer(original,io)||p0.frees!=1||p0.last_freed!=99||
    original.flags!=0x83||original.pointer!=0||original.base!=0||
    original.count!=0)return false;
 File file{8,0x8b,7,10,55,77};
 if(fclose_original(file,io)!=0||p0.frees!=3||p0.last_freed!=77||
    p0.flushes!=1||p0.closes!=1||file.flags!=0||file.tmpname!=0)
    return false;
 File prohibited{3,0x40,9,0,0,0};
 if(fclose_original(prohibited,io)!=-1||p0.closes!=1||
    prohibited.flags!=0)return false;
 File stdout_file{0,0x42,5,0,0,0};
 if(flush_one(&stdout_file,io,flush_all_hook,&p0)!=0||p0.syncs!=1)
    return false;
 if(flush_one(nullptr,io,flush_all_hook,&p0)!=12)return false;
 File regular{0,0x83,8,0,0,0};
 std::array<File*,3> files{&stdout_file,&regular,nullptr};
 if(flush_all(files,1,io)!=2||flush_all(files,0,io)!=0)return false;
 if(formatted_scan(regular,"%d",io)!=4||p0.locks!=1||p0.scans!=1||
    p0.unlocks!=1||p0.last_unlocked!=77)return false;
 Stream stream{};
 U32 allocation_count=0;
 allocate_file_buffer(stream,allocation_count,allocate,&p0);
 if(allocation_count!=1||stream.capacity!=2||stream.base!=
    stream.inline_buffer.data()||stream.pointer!=stream.base||
    (stream.flags&4)==0)return false;
 p0.allocate_ok=true;
 allocate_file_buffer(stream,allocation_count,allocate,&p0);
 if(allocation_count!=2||stream.capacity!=4096||
    stream.base!=p0.storage.data()||(stream.flags&8)==0)return false;
 Heap heap{};
 if(!initialize_heap(heap,1,make_heap,heap_mode,alloc_slab,
                    init_small,destroy,&p0)||
    heap.heap_handle!=88||heap.root!=0x2222||
    heap.control!=0x3f8||heap.step!=16)return false;
 Heap failure{};p0.slab_ok=false;
 if(initialize_heap(failure,1,make_heap,heap_mode,alloc_slab,
                    init_small,destroy,&p0)||
    p0.destroyed!=1)return false;
 Heap smaller{};p0.heap_mode=2;
 if(!initialize_heap(smaller,1,make_heap,heap_mode,alloc_slab,
                    init_small,destroy,&p0)||p0.small_inits!=1)return false;
 CrtStartup startup{};p0.steps=0;
 initialize_crt(startup,pointers,cpu,precision,&p0);
 if(!startup.cpu_feature||p0.steps!=3||p0.order[0]!=1||
    p0.order[1]!=2||p0.order[2]!=3)return false;
 U32 guard=0;char text[4]{};p0.conversion_guard=&guard;
 float_conversion(guard,'X',text,4,format,&p0);
 return guard==0&&text[0]=='X';
}
}
