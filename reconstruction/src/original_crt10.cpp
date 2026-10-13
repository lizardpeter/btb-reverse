#include "btb/original_crt10.hpp"
namespace btb::original_crt10 {
bool free_file_buffer(File& file,const Io& io){
    if((file.flags&0x83)==0||(file.flags&8)==0)return false;
    if(io.deallocate)io.deallocate(file.base,io.ctx);
    file.flags=(file.flags&~0xffff)|((file.flags&0xffff)&0xfbf7);
    file.pointer=0;file.base=0;file.count=0;
    return true;
}
I32 fclose_original(File& file,const Io& io){
    I32 result=-1;
    if((file.flags&0x40)==0&&(file.flags&0x83)!=0){
        result=io.flush?io.flush(file,io.ctx):0;
        (void)free_file_buffer(file,io);
        if(!io.close_handle||io.close_handle(file.file_index,io.ctx)<0)
            result=-1;
        else if(file.tmpname!=0){
            if(io.deallocate)io.deallocate(file.tmpname,io.ctx);
            file.tmpname=0;
        }
    }
    file.flags=0;
    return result;
}
I32 flush_one(File* file,const Io& io,
              I32(*flush_everything)(void*),void* ctx){
    if(!file)return flush_everything?flush_everything(ctx):0;
    if(io.flush&&io.flush(*file,io.ctx)!=0)return -1;
    if((file->flags&0x40)!=0)
        return io.sync_handle&&io.sync_handle(file->file_index,io.ctx)!=0
            ?-1:0;
    return 0;
}
I32 flush_all(std::span<File*> files,I32 mode,const Io& io){
    I32 good=0,failed=0;
    for(File* file:files){
        if(!file||(file->flags&0x83)==0)continue;
        if(mode==1){
            if(flush_one(file,io,nullptr,nullptr)!=-1)++good;
        } else if(mode==0&&(file->flags&2)!=0){
            if(flush_one(file,io,nullptr,nullptr)==-1)failed=-1;
        }
    }
    return mode==1?good:failed;
}
I32 formatted_scan(File& file,const char* format,const Io& io){
    const I32 lock=io.lock?io.lock(file,io.ctx):0;
    const I32 result=io.scan?io.scan(file,format,io.ctx):-1;
    if(io.unlock)io.unlock(lock,file,io.ctx);
    return result;
}
void allocate_file_buffer(Stream& state,U32& count,Allocate allocate,void* ctx){
    ++count;
    state.base=allocate?static_cast<std::byte*>(allocate(4096,ctx)):nullptr;
    if(state.base){state.flags|=8;state.capacity=4096;}
    else{
        state.flags|=4;
        state.base=state.inline_buffer.data();
        state.capacity=2;
    }
    state.count=0;state.pointer=state.base;
}
bool initialize_heap_slab(Heap& heap,U32 mode,SlabAlloc allocate,void* ctx){
    heap.root=allocate?allocate(heap.heap_handle,320,ctx):0;
    if(!heap.root)return false;
    heap.current=heap.root;
    heap.count=0;heap.next=0;
    heap.control=mode;heap.step=16;
    return true;
}
bool initialize_heap(Heap& heap,I32 requested,HeapCreate create,Mode mode,
                     SlabAlloc slab,InitSmall small,HeapDestroy destroy,
                     void* ctx){
    heap.heap_handle=create?create(requested==0,4096,ctx):0;
    if(!heap.heap_handle)return false;
    heap.algorithm=mode?static_cast<U32>(mode(ctx)):0;
    const bool okay=heap.algorithm==3
        ?initialize_heap_slab(heap,0x3f8,slab,ctx)
        :heap.algorithm==2?(small&&small(ctx)):true;
    if(okay)return true;
    if(destroy)destroy(heap.heap_handle,ctx);
    return false;
}
void initialize_crt(CrtStartup& state,Setup pointers,ProcessorFeature check,
                    Setup precision,void* ctx){
    if(pointers)pointers(ctx);
    state.cpu_feature=check?check(ctx):false;
    if(precision)precision(ctx);
}
void float_conversion(U32& guard,U32 value,char* output,std::size_t size,
                      Format format,void* ctx){
    guard=1;
    if(format)format(value,output,size,ctx);
    guard=0;
}
}
