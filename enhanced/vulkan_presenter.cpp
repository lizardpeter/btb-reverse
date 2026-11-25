#include "vulkan_presenter.hpp"
#include "presentation_policy.hpp"
#include "discord_presence.hpp"
#include <vulkan/vulkan.h>
#include <algorithm>
#include <array>
#include <cstring>
#include <vector>
#include <limits>
#include <string>
#include <utility>

namespace btb::enhanced {
namespace {
bool env_flag(const wchar_t* name) noexcept {
    wchar_t v[8]{};return GetEnvironmentVariableW(name,v,8)==1&&v[0]==L'1';
}
bool vk_ok(VkResult result,const char* step,std::string& error){
    if(result==VK_SUCCESS)return true;
    error=std::string(step)+" failed (VkResult "+std::to_string(int(result))+")";return false;
}
class VulkanPresenter final:public full_game::Win32FrameOutput {
    HWND hwnd_{};
    VkInstance instance_{};
    VkSurfaceKHR surface_{};
    VkPhysicalDevice physical_{};
    VkDevice device_{};
    VkQueue queue_{};
    uint32_t family_{};
    VkCommandPool pool_{};
    VkCommandBuffer cmd_{};
    VkFence fence_{};
    VkSemaphore acquired_{};
    VkSwapchainKHR chain_{};
    VkExtent2D extent_{};
    VkFormat format_{VK_FORMAT_UNDEFINED};
    VkPresentModeKHR present_mode_{VK_PRESENT_MODE_FIFO_KHR};
    std::vector<VkImage> images_;
    std::vector<VkSemaphore> ready_;
    VkImage source_{};
    VkDeviceMemory source_memory_{};
    VkBuffer staging_{};
    VkDeviceMemory staging_memory_{};
    void* mapped_{};
    bool source_initialized_{};
    bool dirty_{true},linear_{env_flag(L"BTB_LINEAR_FILTER")};
    bool integer_{!env_flag(L"BTB_FRACTIONAL_SCALE")};
    bool immediate_{env_flag(L"BTB_UNCAPPED_PRESENT")};
    bool borderless_{};
    WINDOWPLACEMENT placement_{sizeof(WINDOWPLACEMENT)};
    LONG_PTR style_{};
    DiscordPresence presence_;
    static constexpr VkDeviceSize kBytes=640*480*4;
    static constexpr VkImageSubresourceRange range_{VK_IMAGE_ASPECT_COLOR_BIT,0,1,0,1};
    static constexpr VkImageSubresourceLayers layers_{VK_IMAGE_ASPECT_COLOR_BIT,0,0,1};

    bool memory_type(uint32_t allowed,VkMemoryPropertyFlags wanted,uint32_t& index) const {
        VkPhysicalDeviceMemoryProperties p{};vkGetPhysicalDeviceMemoryProperties(physical_,&p);
        for(uint32_t i=0;i<p.memoryTypeCount;++i)
            if((allowed&(1u<<i))&&(p.memoryTypes[i].propertyFlags&wanted)==wanted){index=i;return true;}
        return false;
    }
    bool pick_device(std::string& error){
        uint32_t count=0;
        if(!vk_ok(vkEnumeratePhysicalDevices(instance_,&count,nullptr),"enumerate Vulkan GPUs",error))return false;
        std::vector<VkPhysicalDevice> devices(count);
        if(!vk_ok(vkEnumeratePhysicalDevices(instance_,&count,devices.data()),"enumerate Vulkan GPUs",error))return false;
        int best=-1;
        for(auto p:devices){
            VkPhysicalDeviceProperties props{};vkGetPhysicalDeviceProperties(p,&props);
            if(props.apiVersion<VK_API_VERSION_1_4)continue;
            uint32_t n=0;vkGetPhysicalDeviceQueueFamilyProperties(p,&n,nullptr);
            std::vector<VkQueueFamilyProperties> families(n);vkGetPhysicalDeviceQueueFamilyProperties(p,&n,families.data());
            uint32_t ne=0;if(vkEnumerateDeviceExtensionProperties(p,nullptr,&ne,nullptr)!=VK_SUCCESS)continue;
            std::vector<VkExtensionProperties> extensions(ne);
            if(vkEnumerateDeviceExtensionProperties(p,nullptr,&ne,extensions.data())!=VK_SUCCESS)continue;
            if(std::none_of(extensions.begin(),extensions.end(),[](const auto& x){return std::strcmp(x.extensionName,VK_KHR_SWAPCHAIN_EXTENSION_NAME)==0;}))continue;
            for(uint32_t i=0;i<n;++i){
                VkBool32 present=VK_FALSE;
                if(vkGetPhysicalDeviceSurfaceSupportKHR(p,i,surface_,&present)!=VK_SUCCESS||
                   !present||!(families[i].queueFlags&VK_QUEUE_GRAPHICS_BIT))continue;
                const int score=props.deviceType==VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU?3:
                    props.deviceType==VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU?2:1;
                if(score>best){physical_=p;family_=i;best=score;}
            }
        }
        if(!physical_){error="No Vulkan 1.4 graphics/present device with VK_KHR_swapchain";return false;}
        return true;
    }
    void drop_chain() noexcept {
        if(!device_)return;
        for(auto s:ready_)if(s)vkDestroySemaphore(device_,s,nullptr);
        ready_.clear();images_.clear();
        if(source_)vkDestroyImage(device_,source_,nullptr);
        if(source_memory_)vkFreeMemory(device_,source_memory_,nullptr);
        if(chain_)vkDestroySwapchainKHR(device_,chain_,nullptr);
        source_=VK_NULL_HANDLE;source_memory_=VK_NULL_HANDLE;chain_=VK_NULL_HANDLE;
        source_initialized_=false;extent_={};
    }
    bool create_source(std::string& error){
        VkImageCreateInfo im{VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
        im.imageType=VK_IMAGE_TYPE_2D;im.format=format_;im.extent={640,480,1};
        im.mipLevels=1;im.arrayLayers=1;im.samples=VK_SAMPLE_COUNT_1_BIT;
        im.tiling=VK_IMAGE_TILING_OPTIMAL;im.usage=VK_IMAGE_USAGE_TRANSFER_DST_BIT|VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
        im.sharingMode=VK_SHARING_MODE_EXCLUSIVE;im.initialLayout=VK_IMAGE_LAYOUT_UNDEFINED;
        if(!vk_ok(vkCreateImage(device_,&im,nullptr,&source_),"create game image",error))return false;
        VkMemoryRequirements req{};vkGetImageMemoryRequirements(device_,source_,&req);
        VkMemoryAllocateInfo alloc{VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};alloc.allocationSize=req.size;
        if(!memory_type(req.memoryTypeBits,VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,alloc.memoryTypeIndex)){
            error="No device-local memory for game image";return false;
        }
        return vk_ok(vkAllocateMemory(device_,&alloc,nullptr,&source_memory_),"allocate game image",error)&&
               vk_ok(vkBindImageMemory(device_,source_,source_memory_,0),"bind game image",error);
    }
    bool rebuild(std::string& error){
        RECT rect{};GetClientRect(hwnd_,&rect);
        if(rect.right<=rect.left||rect.bottom<=rect.top)return true;
        if(!vk_ok(vkDeviceWaitIdle(device_),"idle before swapchain recreation",error))return false;
        drop_chain();
        VkSurfaceCapabilitiesKHR caps{};
        if(!vk_ok(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_,surface_,&caps),"surface capabilities",error))return false;
        if(!(caps.supportedUsageFlags&VK_IMAGE_USAGE_TRANSFER_DST_BIT)){
            error="Surface does not support Vulkan transfer presentation";return false;
        }
        uint32_t count=0;
        if(!vk_ok(vkGetPhysicalDeviceSurfaceFormatsKHR(physical_,surface_,&count,nullptr),"surface format count",error))return false;
        std::vector<VkSurfaceFormatKHR> formats(count);
        if(!vk_ok(vkGetPhysicalDeviceSurfaceFormatsKHR(physical_,surface_,&count,formats.data()),"surface formats",error))return false;
        VkSurfaceFormatKHR chosen{};bool found=false;
        for(auto preferred:{VK_FORMAT_B8G8R8A8_UNORM,VK_FORMAT_R8G8B8A8_UNORM,VK_FORMAT_B8G8R8A8_SRGB,VK_FORMAT_R8G8B8A8_SRGB}){
            for(auto candidate:formats){
                if(candidate.format!=preferred&&candidate.format!=VK_FORMAT_UNDEFINED)continue;
                VkFormatProperties props{};vkGetPhysicalDeviceFormatProperties(physical_,preferred,&props);
                const auto need=VK_FORMAT_FEATURE_BLIT_SRC_BIT|VK_FORMAT_FEATURE_BLIT_DST_BIT|
                    (linear_?VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT:0);
                if((props.optimalTilingFeatures&need)!=need)continue;
                chosen={preferred,candidate.colorSpace};found=true;break;
            }
            if(found)break;
        }
        if(!found){error="No compatible 32-bit Vulkan surface format for scaled blitting";return false;}
        format_=chosen.format;
        extent_=caps.currentExtent;
        if(extent_.width==UINT32_MAX){
            extent_.width=std::clamp(uint32_t(rect.right-rect.left),caps.minImageExtent.width,caps.maxImageExtent.width);
            extent_.height=std::clamp(uint32_t(rect.bottom-rect.top),caps.minImageExtent.height,caps.maxImageExtent.height);
        }
        if(!extent_.width||!extent_.height){dirty_=true;return true;}
        count=0;
        if(!vk_ok(vkGetPhysicalDeviceSurfacePresentModesKHR(physical_,surface_,&count,nullptr),"present mode count",error))return false;
        std::vector<VkPresentModeKHR> modes(count);
        if(!vk_ok(vkGetPhysicalDeviceSurfacePresentModesKHR(physical_,surface_,&count,modes.data()),"present modes",error))return false;
        present_mode_=VK_PRESENT_MODE_FIFO_KHR;
        if(immediate_){
            for(auto m:{VK_PRESENT_MODE_IMMEDIATE_KHR,VK_PRESENT_MODE_MAILBOX_KHR})
                if(std::find(modes.begin(),modes.end(),m)!=modes.end()){present_mode_=m;break;}
        }
        VkSwapchainCreateInfoKHR sc{VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR};
        sc.surface=surface_;sc.minImageCount=caps.minImageCount+1;
        if(caps.maxImageCount)sc.minImageCount=std::min(sc.minImageCount,caps.maxImageCount);
        sc.imageFormat=format_;sc.imageColorSpace=chosen.colorSpace;sc.imageExtent=extent_;
        sc.imageArrayLayers=1;sc.imageUsage=VK_IMAGE_USAGE_TRANSFER_DST_BIT;sc.imageSharingMode=VK_SHARING_MODE_EXCLUSIVE;
        sc.preTransform=caps.currentTransform;sc.presentMode=present_mode_;sc.clipped=VK_TRUE;
        for(auto alpha:{VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
                         VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR})
            if(caps.supportedCompositeAlpha&alpha){sc.compositeAlpha=alpha;break;}
        if(!vk_ok(vkCreateSwapchainKHR(device_,&sc,nullptr,&chain_),"create Vulkan swapchain",error))return false;
        count=0;if(!vk_ok(vkGetSwapchainImagesKHR(device_,chain_,&count,nullptr),"swapchain image count",error))return false;
        images_.resize(count);if(!vk_ok(vkGetSwapchainImagesKHR(device_,chain_,&count,images_.data()),"swapchain images",error))return false;
        ready_.resize(count,VK_NULL_HANDLE);
        VkSemaphoreCreateInfo semaphore{VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};
        for(auto& s:ready_)if(!vk_ok(vkCreateSemaphore(device_,&semaphore,nullptr,&s),"image present semaphore",error))return false;
        if(!create_source(error))return false;
        dirty_=false;return true;
    }
    void barrier(VkImage image,VkImageLayout old,VkImageLayout next,
        VkAccessFlags src,VkAccessFlags dst,VkPipelineStageFlags from,VkPipelineStageFlags to){
        VkImageMemoryBarrier b{VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER};
        b.srcAccessMask=src;b.dstAccessMask=dst;b.oldLayout=old;b.newLayout=next;
        b.srcQueueFamilyIndex=VK_QUEUE_FAMILY_IGNORED;b.dstQueueFamilyIndex=VK_QUEUE_FAMILY_IGNORED;
        b.image=image;b.subresourceRange=range_;
        vkCmdPipelineBarrier(cmd_,from,to,0,0,nullptr,0,nullptr,1,&b);
    }
public:
    ~VulkanPresenter() override{shutdown();}
    bool initialize(HWND hwnd,std::string& error) override {
        hwnd_=hwnd;
        uint32_t loader=VK_API_VERSION_1_0;
        auto enumerate=reinterpret_cast<PFN_vkEnumerateInstanceVersion>(vkGetInstanceProcAddr(nullptr,"vkEnumerateInstanceVersion"));
        if(!enumerate||enumerate(&loader)!=VK_SUCCESS||loader<VK_API_VERSION_1_4){
            error="Vulkan 1.4 loader unavailable; install a current GPU driver";return false;
        }
        VkApplicationInfo app{VK_STRUCTURE_TYPE_APPLICATION_INFO};
        app.pApplicationName="Bob Builds a Park Enhanced";app.apiVersion=VK_API_VERSION_1_4;
        const char* extensions[]={VK_KHR_SURFACE_EXTENSION_NAME,VK_KHR_WIN32_SURFACE_EXTENSION_NAME};
        VkInstanceCreateInfo ic{VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO};
        ic.pApplicationInfo=&app;ic.enabledExtensionCount=2;ic.ppEnabledExtensionNames=extensions;
        if(!vk_ok(vkCreateInstance(&ic,nullptr,&instance_),"create Vulkan 1.4 instance",error))return false;
        VkWin32SurfaceCreateInfoKHR si{VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR};
        si.hinstance=GetModuleHandleW(nullptr);si.hwnd=hwnd_;
        if(!vk_ok(vkCreateWin32SurfaceKHR(instance_,&si,nullptr,&surface_),"create Win32 Vulkan surface",error)||!pick_device(error))return false;
        float priority=1.f;
        VkDeviceQueueCreateInfo qi{VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO};
        qi.queueFamilyIndex=family_;qi.queueCount=1;qi.pQueuePriorities=&priority;
        const char* swap=VK_KHR_SWAPCHAIN_EXTENSION_NAME;
        VkDeviceCreateInfo di{VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO};di.queueCreateInfoCount=1;di.pQueueCreateInfos=&qi;
        di.enabledExtensionCount=1;di.ppEnabledExtensionNames=&swap;
        if(!vk_ok(vkCreateDevice(physical_,&di,nullptr,&device_),"create Vulkan device",error))return false;
        vkGetDeviceQueue(device_,family_,0,&queue_);
        VkCommandPoolCreateInfo pi{VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO};
        pi.flags=VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;pi.queueFamilyIndex=family_;
        if(!vk_ok(vkCreateCommandPool(device_,&pi,nullptr,&pool_),"command pool",error))return false;
        VkCommandBufferAllocateInfo ca{VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO};
        ca.commandPool=pool_;ca.level=VK_COMMAND_BUFFER_LEVEL_PRIMARY;ca.commandBufferCount=1;
        if(!vk_ok(vkAllocateCommandBuffers(device_,&ca,&cmd_),"command buffer",error))return false;
        VkFenceCreateInfo fi{VK_STRUCTURE_TYPE_FENCE_CREATE_INFO};fi.flags=VK_FENCE_CREATE_SIGNALED_BIT;
        VkSemaphoreCreateInfo se{VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};
        if(!vk_ok(vkCreateFence(device_,&fi,nullptr,&fence_),"frame fence",error)||
           !vk_ok(vkCreateSemaphore(device_,&se,nullptr,&acquired_),"image acquire semaphore",error))return false;
        VkBufferCreateInfo bi{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
        bi.size=kBytes;bi.usage=VK_BUFFER_USAGE_TRANSFER_SRC_BIT;bi.sharingMode=VK_SHARING_MODE_EXCLUSIVE;
        if(!vk_ok(vkCreateBuffer(device_,&bi,nullptr,&staging_),"upload staging buffer",error))return false;
        VkMemoryRequirements req{};vkGetBufferMemoryRequirements(device_,staging_,&req);
        VkMemoryAllocateInfo ma{VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};ma.allocationSize=req.size;
        if(!memory_type(req.memoryTypeBits,VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT|VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,ma.memoryTypeIndex)){
            error="No coherent host-visible Vulkan upload memory";return false;
        }
        if(!vk_ok(vkAllocateMemory(device_,&ma,nullptr,&staging_memory_),"allocate upload memory",error)||
           !vk_ok(vkBindBufferMemory(device_,staging_,staging_memory_,0),"bind upload memory",error)||
           !vk_ok(vkMapMemory(device_,staging_memory_,0,kBytes,0,&mapped_),"map upload memory",error))return false;
        presence_.start();
        error.clear();return true;
    }
    bool present(const display::PackedFrameSnapshot& frame,std::string& error) override {
        if(!frame.valid()||frame.width!=640||frame.height!=480){error="Invalid composed game frame";return false;}
        RECT rect{};GetClientRect(hwnd_,&rect);
        if(IsIconic(hwnd_)||rect.right<=rect.left||rect.bottom<=rect.top){error.clear();return true;}
        if(dirty_||extent_.width!=uint32_t(rect.right-rect.left)||extent_.height!=uint32_t(rect.bottom-rect.top))
            if(!rebuild(error))return false;
        if(!chain_){error.clear();return true;}
        if(!vk_ok(vkWaitForFences(device_,1,&fence_,VK_TRUE,UINT64_MAX),"wait upload fence",error))return false;
        uint32_t index=0;
        auto result=vkAcquireNextImageKHR(device_,chain_,0,acquired_,VK_NULL_HANDLE,&index);
        if(result==VK_ERROR_OUT_OF_DATE_KHR){dirty_=true;error.clear();return true;}
        if(result==VK_NOT_READY||result==VK_TIMEOUT){error.clear();return true;}
        if(result==VK_SUBOPTIMAL_KHR)dirty_=true;
        else if(!vk_ok(result,"acquire swapchain image",error))return false;
        const bool bgra=format_==VK_FORMAT_B8G8R8A8_UNORM||format_==VK_FORMAT_B8G8R8A8_SRGB;
        const display::PackedRgbFormat packed{4,bgra?0x00ff0000U:0x000000ffU,0x0000ff00U,bgra?0x000000ffU:0x00ff0000U};
        if(!display::restore_packed_frame(frame,{static_cast<uint8_t*>(mapped_),size_t(kBytes)},640*4,packed)){
            error="Cannot convert composed RGB frame to Vulkan upload format";return false;
        }
        // Alpha is opaque, irrespective of unused high bits in DD7 XRGB.
        auto* bytes=static_cast<uint8_t*>(mapped_);
        for(size_t i=3;i<size_t(kBytes);i+=4)bytes[i]=255;
        if(!vk_ok(vkResetCommandBuffer(cmd_,0),"reset command buffer",error))return false;
        VkCommandBufferBeginInfo cb{VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};cb.flags=VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        if(!vk_ok(vkBeginCommandBuffer(cmd_,&cb),"begin Vulkan frame",error))return false;
        barrier(source_,source_initialized_?VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,source_initialized_?VK_ACCESS_TRANSFER_READ_BIT:0,
            VK_ACCESS_TRANSFER_WRITE_BIT,source_initialized_?VK_PIPELINE_STAGE_TRANSFER_BIT:VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,VK_PIPELINE_STAGE_TRANSFER_BIT);
        VkBufferImageCopy copy{};copy.imageSubresource=layers_;copy.imageExtent={640,480,1};
        vkCmdCopyBufferToImage(cmd_,staging_,source_,VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,1,&copy);
        barrier(source_,VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
            VK_ACCESS_TRANSFER_WRITE_BIT,VK_ACCESS_TRANSFER_READ_BIT,VK_PIPELINE_STAGE_TRANSFER_BIT,VK_PIPELINE_STAGE_TRANSFER_BIT);
        auto dest=images_[index];
        barrier(dest,VK_IMAGE_LAYOUT_UNDEFINED,VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,0,VK_ACCESS_TRANSFER_WRITE_BIT,
            VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,VK_PIPELINE_STAGE_TRANSFER_BIT);
        VkClearColorValue black{};black.float32[3]=1.f;
        vkCmdClearColorImage(cmd_,dest,VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,&black,1,&range_);
        // Order the clear and blit: both write the same destination image.
        barrier(dest,VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            VK_ACCESS_TRANSFER_WRITE_BIT,VK_ACCESS_TRANSFER_WRITE_BIT,VK_PIPELINE_STAGE_TRANSFER_BIT,VK_PIPELINE_STAGE_TRANSFER_BIT);
        const auto v=viewport(int(extent_.width),int(extent_.height),integer_);
        VkImageBlit blit{};blit.srcSubresource=layers_;blit.dstSubresource=layers_;
        blit.srcOffsets[1]={640,480,1};blit.dstOffsets[0]={v.x,v.y,0};blit.dstOffsets[1]={v.x+v.width,v.y+v.height,1};
        vkCmdBlitImage(cmd_,source_,VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,dest,VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            1,&blit,linear_?VK_FILTER_LINEAR:VK_FILTER_NEAREST);
        barrier(dest,VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,VK_ACCESS_TRANSFER_WRITE_BIT,0,
            VK_PIPELINE_STAGE_TRANSFER_BIT,VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT);
        if(!vk_ok(vkEndCommandBuffer(cmd_),"end Vulkan frame",error)||
           !vk_ok(vkResetFences(device_,1,&fence_),"reset frame fence",error))return false;
        const VkPipelineStageFlags wait=VK_PIPELINE_STAGE_TRANSFER_BIT;
        VkSubmitInfo submit{VK_STRUCTURE_TYPE_SUBMIT_INFO};
        submit.waitSemaphoreCount=1;submit.pWaitSemaphores=&acquired_;submit.pWaitDstStageMask=&wait;
        submit.commandBufferCount=1;submit.pCommandBuffers=&cmd_;submit.signalSemaphoreCount=1;submit.pSignalSemaphores=&ready_[index];
        if(!vk_ok(vkQueueSubmit(queue_,1,&submit,fence_),"submit Vulkan frame",error))return false;
        source_initialized_=true;
        VkPresentInfoKHR p{VK_STRUCTURE_TYPE_PRESENT_INFO_KHR};
        p.waitSemaphoreCount=1;p.pWaitSemaphores=&ready_[index];p.swapchainCount=1;p.pSwapchains=&chain_;p.pImageIndices=&index;
        result=vkQueuePresentKHR(queue_,&p);
        if(result==VK_ERROR_OUT_OF_DATE_KHR||result==VK_SUBOPTIMAL_KHR)dirty_=true;
        else if(!vk_ok(result,"present Vulkan frame",error))return false;
        error.clear();return true;
    }
    void state_changed(int state) noexcept override {presence_.update(state);}
    void map_pointer(int& x,int& y,int w,int h) const noexcept override {enhanced::map_pointer(x,y,viewport(w,h,integer_));}
    void toggle_borderless() noexcept override {
        if(!borderless_){
            MONITORINFO monitor{sizeof(MONITORINFO)};
            if(!GetWindowPlacement(hwnd_,&placement_)||!GetMonitorInfoW(MonitorFromWindow(hwnd_,MONITOR_DEFAULTTONEAREST),&monitor))return;
            style_=GetWindowLongPtrW(hwnd_,GWL_STYLE);
            SetWindowLongPtrW(hwnd_,GWL_STYLE,style_&~WS_OVERLAPPEDWINDOW);
            SetWindowPos(hwnd_,HWND_TOP,monitor.rcMonitor.left,monitor.rcMonitor.top,
                monitor.rcMonitor.right-monitor.rcMonitor.left,monitor.rcMonitor.bottom-monitor.rcMonitor.top,SWP_FRAMECHANGED|SWP_NOOWNERZORDER);
        }else{
            SetWindowLongPtrW(hwnd_,GWL_STYLE,style_);SetWindowPlacement(hwnd_,&placement_);
            SetWindowPos(hwnd_,nullptr,0,0,0,0,SWP_FRAMECHANGED|SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER|SWP_NOOWNERZORDER);
        }
        borderless_=!borderless_;dirty_=true;
    }
    void shutdown() noexcept override {
        presence_.stop();
        if(device_){
            vkDeviceWaitIdle(device_);
            drop_chain();
            if(mapped_)vkUnmapMemory(device_,staging_memory_);
            if(staging_)vkDestroyBuffer(device_,staging_,nullptr);
            if(staging_memory_)vkFreeMemory(device_,staging_memory_,nullptr);
            if(acquired_)vkDestroySemaphore(device_,acquired_,nullptr);
            if(fence_)vkDestroyFence(device_,fence_,nullptr);
            if(pool_)vkDestroyCommandPool(device_,pool_,nullptr);
            vkDestroyDevice(device_,nullptr);
        }
        if(surface_&&instance_)vkDestroySurfaceKHR(instance_,surface_,nullptr);
        if(instance_)vkDestroyInstance(instance_,nullptr);
        device_=VK_NULL_HANDLE;surface_=VK_NULL_HANDLE;instance_=VK_NULL_HANDLE;mapped_=nullptr;
    }
};
}
std::unique_ptr<full_game::Win32FrameOutput> make_vulkan_presenter(){return std::make_unique<VulkanPresenter>();}
}
