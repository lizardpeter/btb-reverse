#include "vulkan_sprite_batch.hpp"
#include "packed_layer_plan.hpp"
#if defined(_WIN32)
#include <windows.h>
#endif
#include <fstream>
#include <cstring>
#include <algorithm>
namespace btb::enhanced {
namespace {
bool ok(VkResult r,const char* op,std::string& error){
    if(r==VK_SUCCESS)return true;
    error=std::string("Sprite compositor ")+op+" failed: "+std::to_string(int(r));return false;
}
constexpr VkDeviceSize output_bytes=640*480*4;
}
bool VulkanSpriteBatch::buffer(Buffer& b,VkDeviceSize size,VkBufferUsageFlags flags,std::string& error){
    VkBufferCreateInfo info{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
    info.size=size;info.usage=flags;info.sharingMode=VK_SHARING_MODE_EXCLUSIVE;
    if(!ok(vkCreateBuffer(device_,&info,nullptr,&b.buffer),"buffer",error))return false;
    VkMemoryRequirements req{};vkGetBufferMemoryRequirements(device_,b.buffer,&req);
    VkPhysicalDeviceMemoryProperties props{};vkGetPhysicalDeviceMemoryProperties(physical_,&props);
    VkMemoryAllocateInfo alloc{VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
    alloc.allocationSize=req.size;
    bool found=false;
    constexpr auto wanted=VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT|VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    for(uint32_t i=0;i<props.memoryTypeCount;++i)
        if((req.memoryTypeBits&(1u<<i))&&(props.memoryTypes[i].propertyFlags&wanted)==wanted){
            alloc.memoryTypeIndex=i;found=true;break;
        }
    if(!found){error="Sprite compositor needs coherent host-visible storage";return false;}
    return ok(vkAllocateMemory(device_,&alloc,nullptr,&b.memory),"memory",error)&&
        ok(vkBindBufferMemory(device_,b.buffer,b.memory,0),"bind",error)&&
        ok(vkMapMemory(device_,b.memory,0,size,0,&b.mapped),"map",error);
}
void VulkanSpriteBatch::destroy(Buffer& b) noexcept {
    if(b.mapped)vkUnmapMemory(device_,b.memory);
    if(b.buffer)vkDestroyBuffer(device_,b.buffer,nullptr);
    if(b.memory)vkFreeMemory(device_,b.memory,nullptr);
    b={};
}
bool VulkanSpriteBatch::create_pipeline(const std::filesystem::path& path,const char* entry,VkPipeline& pipeline,std::string& error){
    std::ifstream file(path,std::ios::binary|std::ios::ate);
    if(!file){error="Cannot open compiled enhanced compute shader";return false;}
    const std::streamoff size=file.tellg();
    if(size<=0||size>1024*1024||size%4){error="Invalid sprite compute shader size";return false;}
    std::vector<uint32_t> code(static_cast<std::size_t>(size)/4);
    file.seekg(0);
    if(!file.read(reinterpret_cast<char*>(code.data()),static_cast<std::streamsize>(size))||code[0]!=0x07230203){
        error="Invalid sprite compute SPIR-V";return false;
    }
    VkShaderModuleCreateInfo mi{VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO};mi.codeSize=code.size()*4;mi.pCode=code.data();
    VkShaderModule shader{};
    if(!ok(vkCreateShaderModule(device_,&mi,nullptr,&shader),"shader module",error))return false;
    VkComputePipelineCreateInfo ci{VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO};
    ci.layout=pipeline_layout_;ci.stage={VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO};
    ci.stage.stage=VK_SHADER_STAGE_COMPUTE_BIT;ci.stage.module=shader;ci.stage.pName=entry;
    const auto created=vkCreateComputePipelines(device_,VK_NULL_HANDLE,1,&ci,nullptr,&pipeline);
    vkDestroyShaderModule(device_,shader,nullptr);
    if(!ok(created,"compute pipeline",error))return false;
    return true;
}
bool VulkanSpriteBatch::initialize(VkPhysicalDevice physical,VkDevice device,const std::filesystem::path& path,std::string& error){
    physical_=physical;device_=device;
    VkPhysicalDeviceProperties props{};vkGetPhysicalDeviceProperties(physical_,&props);
    if(props.limits.maxStorageBufferRange<batch_word_limit*4||
       props.limits.maxPerStageDescriptorStorageBuffers<2){
        error="GPU storage limits are too small for sprite compositor";return false;
    }
    VkDescriptorSetLayoutBinding bindings[2]{};
    for(uint32_t i=0;i<2;++i){bindings[i].binding=i;bindings[i].descriptorType=VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        bindings[i].descriptorCount=1;bindings[i].stageFlags=VK_SHADER_STAGE_COMPUTE_BIT;}
    VkDescriptorSetLayoutCreateInfo li{VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO};
    li.bindingCount=2;li.pBindings=bindings;
    if(!ok(vkCreateDescriptorSetLayout(device_,&li,nullptr,&layout_),"descriptor layout",error))return false;
    VkPushConstantRange push{VK_SHADER_STAGE_COMPUTE_BIT,0,4};
    VkPipelineLayoutCreateInfo pi{VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO};
    pi.setLayoutCount=1;pi.pSetLayouts=&layout_;pi.pushConstantRangeCount=1;pi.pPushConstantRanges=&push;
    if(!ok(vkCreatePipelineLayout(device_,&pi,nullptr,&pipeline_layout_),"pipeline layout",error))return false;
    if(!create_pipeline(path,"spriteBatch",pipeline_,error)||
       !create_pipeline(path.parent_path()/L"binkOpaque.spv","binkOpaque",video_pipeline_,error))return false;
    if(!buffer(input_,batch_word_limit*4,VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,error)||
       !buffer(output_,output_bytes,VK_BUFFER_USAGE_STORAGE_BUFFER_BIT|VK_BUFFER_USAGE_TRANSFER_SRC_BIT,error))return false;
    VkDescriptorPoolSize ps{VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,2};
    VkDescriptorPoolCreateInfo di{VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO};
    di.maxSets=1;di.poolSizeCount=1;di.pPoolSizes=&ps;
    if(!ok(vkCreateDescriptorPool(device_,&di,nullptr,&pool_),"descriptor pool",error))return false;
    VkDescriptorSetAllocateInfo ai{VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO};
    ai.descriptorPool=pool_;ai.descriptorSetCount=1;ai.pSetLayouts=&layout_;
    if(!ok(vkAllocateDescriptorSets(device_,&ai,&set_),"descriptor set",error))return false;
    VkDescriptorBufferInfo bi[2]={{input_.buffer,0,batch_word_limit*4},{output_.buffer,0,output_bytes}};
    VkWriteDescriptorSet writes[2]{};
    for(uint32_t i=0;i<2;++i){writes[i].sType=VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writes[i].dstSet=set_;writes[i].dstBinding=i;writes[i].descriptorCount=1;
        writes[i].descriptorType=VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;writes[i].pBufferInfo=&bi[i];}
    vkUpdateDescriptorSets(device_,2,writes,0,nullptr);
    error.clear();return true;
}
VulkanSpriteBatch::Bitmap* VulkanSpriteBatch::load(const std::filesystem::path& path){
    if(auto it=cache_.find(path);it!=cache_.end())return &it->second;
    #if defined(_WIN32)
    struct GdiBitmap {HBITMAP h{};~GdiBitmap(){if(h)DeleteObject(h);}};
    GdiBitmap bitmap{static_cast<HBITMAP>(LoadImageW(nullptr,path.c_str(),IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION))};
    BITMAP data{};
    if(!bitmap.h||GetObjectW(bitmap.h,sizeof(data),&data)!=sizeof(data)||
       data.bmWidth<=0||data.bmHeight<=0||data.bmWidth>32768||data.bmHeight>32768)return nullptr;
    const auto count=std::uint64_t(data.bmWidth)*data.bmHeight;
    if(count>batch_word_limit)return nullptr;
    Bitmap pixels{unsigned(data.bmWidth),unsigned(data.bmHeight),{}};
    pixels.pixels.resize(std::size_t(count));
    BITMAPINFO info{};info.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
    info.bmiHeader.biWidth=data.bmWidth;info.bmiHeader.biHeight=-data.bmHeight;
    info.bmiHeader.biPlanes=1;info.bmiHeader.biBitCount=32;info.bmiHeader.biCompression=BI_RGB;
    HDC dc=GetDC(nullptr);if(!dc)return nullptr;
    const int copied=GetDIBits(dc,bitmap.h,0,data.bmHeight,pixels.pixels.data(),&info,DIB_RGB_COLORS);
    ReleaseDC(nullptr,dc);
    if(copied!=data.bmHeight)return nullptr;
    // GDI BI_RGB is BGRX; normalize to the shared RGBA word layout.
    for(auto& c:pixels.pixels)c=0xff000000U|(c&0xff00U)|((c&255U)<<16)|((c>>16)&255U);
    if(cached_words_+count>batch_word_limit*2){cache_.clear();cached_words_=0;}
    cached_words_+=std::size_t(count);
    return &cache_.emplace(path,std::move(pixels)).first->second;
    #else
    // The non-Windows target is a headless compute regression. It uses
    // deterministic raw pixel fixtures, not the Windows BMP loader.
    return nullptr;
    #endif
}
bool VulkanSpriteBatch::prepare(const std::vector<full_game::ResolvedDraw>& draws){
    if(!pipeline_||draws.size()>8192)return false;
    const auto reserved=reserve_atlas_base(draws.size());
    if(!reserved)return false;
    bool reuse=resident_valid_&&resident_base_==reserved&&resident_order_.size()==draws.size();
    for(std::size_t i=0;i<draws.size();++i){
        if(!draws[i].source.found())return false;
        if(reuse&&resident_order_[i]!=draws[i].source.absolute_path)reuse=false;
    }
    jobs_.clear();
    if(!reuse){
        invalidate_resident();resident_.clear();resident_order_.clear();atlas_.clear();
        for(const auto& d:draws){
            const auto& path=d.source.absolute_path;resident_order_.push_back(path);
            if(resident_.contains(path))continue;
            const auto* b=load(path);if(!b)return false;
            if(reserved>batch_word_limit||b->pixels.size()>batch_word_limit-reserved-atlas_.size())return false;
            const auto offset=std::uint32_t(atlas_.size());
            atlas_.insert(atlas_.end(),b->pixels.begin(),b->pixels.end());
            resident_.emplace(path,ResidentBitmap{b->width,b->height,offset});
        }
        if(!upload_resident_atlas(atlas_,reserved))return false;
    }
    for(const auto& d:draws){
        const auto& b=resident_.at(d.source.absolute_path);SpriteJob job{};
        if(!make_sprite_job(job,d.original,b.width,b.height,b.offset))return false;
        if(job.width&&job.height)jobs_.push_back(job);
    }
    return update_resident_jobs(jobs_);
}
bool VulkanSpriteBatch::prepare_layers(const std::vector<full_game::FrameCompositionLayer>& layers){
    if(!pipeline_||layers.size()>8192)return false;
    invalidate_resident();atlas_.clear();jobs_.clear();
    std::unordered_map<std::filesystem::path,ResidentBitmap> offsets;
    for(const auto& layer:layers){
        if(const auto* pixel=std::get_if<full_game::PackedCompositionLayer>(&layer)){
            if(!append_packed_layer(*pixel,atlas_,jobs_))return false;
            continue;
        }
        const auto& draw=std::get<full_game::ResolvedDraw>(layer);
        if(!draw.source.found())return false;
        auto it=offsets.find(draw.source.absolute_path);
        if(it==offsets.end()){
            const auto* b=load(draw.source.absolute_path);
            if(!b||b->pixels.size()>batch_word_limit-atlas_.size())return false;
            const auto offset=std::uint32_t(atlas_.size());
            atlas_.insert(atlas_.end(),b->pixels.begin(),b->pixels.end());
            it=offsets.emplace(draw.source.absolute_path,ResidentBitmap{b->width,b->height,offset}).first;
        }
        SpriteJob job{};const auto& b=it->second;
        if(!make_sprite_job(job,draw.original,b.width,b.height,b.offset))return false;
        if(job.width&&job.height)jobs_.push_back(job);
    }
    return prepare_jobs(jobs_,atlas_);
}
bool VulkanSpriteBatch::upload_resident_atlas(std::span<const std::uint32_t> atlas,std::size_t reserved){
    invalidate_resident();
    if(!input_.mapped||reserved<header_words+tile_words||reserved>batch_word_limit||atlas.size()>batch_word_limit-reserved)return false;
    resident_base_=reserved;resident_size_=atlas.size();
    if(!atlas.empty())std::memcpy(static_cast<std::uint32_t*>(input_.mapped)+resident_base_,atlas.data(),resident_size_*4);
    resident_valid_=true;return true;
}
bool VulkanSpriteBatch::update_resident_jobs(std::span<const SpriteJob> jobs){
    if(!input_.mapped||!resident_valid_||!pack_sprite_metadata(jobs,resident_size_,resident_base_,words_))return false;
    std::memcpy(input_.mapped,words_.data(),words_.size()*4);
    video_pending_=false;return true;
}
bool VulkanSpriteBatch::prepare_jobs(std::span<const SpriteJob> jobs,std::span<const std::uint32_t> atlas){
    invalidate_resident();
    if(!input_.mapped||!pack_sprite_batch(jobs,atlas,words_))return false;
    std::memcpy(input_.mapped,words_.data(),words_.size()*4);
    video_pending_=false;return true;
}
bool VulkanSpriteBatch::prepare_opaque_video(const full_game::OpaqueGlobalVideo& video){
    invalidate_resident();
    if(!video_pipeline_||!input_.mapped||!video.valid())return false;
    auto* words=static_cast<std::uint32_t*>(input_.mapped);
    std::size_t cursor=4;
    for(std::size_t i=0;i<3;++i){
        words[i]=static_cast<std::uint32_t>(cursor);
        std::memcpy(words+cursor,video.planes[i].data(),video.planes[i].size());
        cursor+=video.planes[i].size()/4;
    }
    video_pending_=true;return true;
}
void VulkanSpriteBatch::record(VkCommandBuffer cmd,bool bgra){
    // The host fence ensures the previous output is no longer consumed.
    vkCmdBindPipeline(cmd,VK_PIPELINE_BIND_POINT_COMPUTE,video_pending_?video_pipeline_:pipeline_);
    vkCmdBindDescriptorSets(cmd,VK_PIPELINE_BIND_POINT_COMPUTE,pipeline_layout_,0,1,&set_,0,nullptr);
    const uint32_t value=bgra?1:0;
    vkCmdPushConstants(cmd,pipeline_layout_,VK_SHADER_STAGE_COMPUTE_BIT,0,4,&value);
    vkCmdDispatch(cmd,80,60,1);
    VkBufferMemoryBarrier b{VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER};
    b.srcAccessMask=VK_ACCESS_SHADER_WRITE_BIT;b.dstAccessMask=VK_ACCESS_TRANSFER_READ_BIT|VK_ACCESS_HOST_READ_BIT;
    b.srcQueueFamilyIndex=VK_QUEUE_FAMILY_IGNORED;b.dstQueueFamilyIndex=VK_QUEUE_FAMILY_IGNORED;
    b.buffer=output_.buffer;b.size=VK_WHOLE_SIZE;
    vkCmdPipelineBarrier(cmd,VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,VK_PIPELINE_STAGE_TRANSFER_BIT|VK_PIPELINE_STAGE_HOST_BIT,0,0,nullptr,1,&b,0,nullptr);
}
void VulkanSpriteBatch::shutdown() noexcept {
    if(!device_)return;
    if(video_pipeline_)vkDestroyPipeline(device_,video_pipeline_,nullptr);
    if(pipeline_)vkDestroyPipeline(device_,pipeline_,nullptr);
    if(pipeline_layout_)vkDestroyPipelineLayout(device_,pipeline_layout_,nullptr);
    if(pool_)vkDestroyDescriptorPool(device_,pool_,nullptr);
    if(layout_)vkDestroyDescriptorSetLayout(device_,layout_,nullptr);
    destroy(output_);destroy(input_);device_=VK_NULL_HANDLE;
}
}
