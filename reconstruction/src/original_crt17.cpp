#include "btb/original_crt17.hpp"
#include <limits>
namespace btb::original_crt17 {
void retail_srand(U32& seed,U32 value) noexcept {seed=value;}
U32 retail_rand(U32& seed) noexcept {
 seed=seed*0x343fdU+0x269ec3U;
 return (seed>>16U)&0x7fffU;
}
void delete_proxy(Ptr p,Free deallocate) noexcept {
 if(deallocate)deallocate(p);
}
Ptr operator_new(U32 count,Allocate allocate) noexcept {
 return allocate?allocate(count,1):nullptr;
}
Ptr crt_malloc(std::size_t count,I32 handler,Allocate allocate) noexcept {
 return allocate?allocate(count,handler):nullptr;
}
Ptr nh_malloc(std::size_t size,I32 flag,RawMalloc alloc,NewHandler handler) noexcept{
 if(size>=0xffffffe1ULL || !alloc)return nullptr;
 for(;;){
  if(auto* found=alloc(size))return found;
  if(flag==0||!handler||!handler(size))return nullptr;
 }
}
Ptr fopen_internal(const char* file,const char* mode,U32 flags,
 ThreadState thread,OpenNative native) noexcept{
 Ptr state=thread?thread():nullptr;
 return state&&native?native(file,mode,flags,state):nullptr;
}
Ptr fopen_native(const char* file,const char* mode,
 ThreadState thread,OpenNative native) noexcept {
 return fopen_internal(file,mode,0x40U,thread,native);
}
I32 fscanf_native(Ptr file,const char* format,Ptr args,ScanNative scan) noexcept {
 return scan?scan(file,format,args):-1;
}
void nlg_notify(NlgGlobals& s,U32 target,U32 eax,U32 ebp) noexcept {
 s.target=target;s.eax=eax;s.ebp=ebp;
}
void nlg_from_stack(NlgGlobals& s,U32 target,U32 eax,U32 ebp) noexcept {
 s.target=target;s.eax=eax;s.ebp=ebp;
}
void exception_destruct(Exception& ex,U32 vtable,Free free) noexcept {
 ex.vtable=vtable;
 if(ex.owned!=0 && free)free(ex.message);
}
void type_info_destruct(TypeInfo& ex,U32 vtable,Free free) noexcept {
 ex.vtable=vtable;
 if(ex.decorated_name && free)free(ex.decorated_name);
}
void swap_forward_bytes(std::span<std::byte> lhs,std::span<std::byte> rhs,
 std::size_t count) noexcept {
 if(lhs.data()==rhs.data())return;
 if(count>lhs.size())count=lhs.size();
 if(count>rhs.size())count=rhs.size();
 for(std::size_t i=0;i<count;++i){
  const auto tmp=lhs[i];lhs[i]=rhs[i];rhs[i]=tmp;
 }
}
char* retail_itoa(U32 value,char* out,U32 radix,IntegerFormatter worker) noexcept{
 const bool negative=radix==10 && std::bit_cast<I32>(value)<0;
 if(worker)worker(value,out,radix,negative);
 return out;
}
Ptr deleting_exception(Ptr ex,U32 flag,Destroy destroy,Free free) noexcept{
 if(destroy)destroy(ex);
 if((flag&1U)!=0 && free)free(ex);
 return ex;
}
bool print_dialog_thunk(Ptr configuration,PrintDialog imported) noexcept {
 return imported&&imported(configuration);
}
}
