#include "vulkan_sprite_batch.hpp"
#include "btb/bink32_surface_convert.hpp"
#include <iostream>
#include <array>
#include <random>
#include <windows.h>
namespace {
struct Device {
    VkInstance instance{};VkDevice device{};VkCommandPool pool{};
    ~Device(){if(device){vkDeviceWaitIdle(device);if(pool)vkDestroyCommandPool(device,pool,nullptr);vkDestroyDevice(device,nullptr);}
        if(instance)vkDestroyInstance(instance,nullptr);}
};
int fail(const char* error){std::cerr<<error<<'\n';return 1;}
}
int main(){
    using namespace btb::enhanced;
    uint32_t version=0;
    if(vkEnumerateInstanceVersion(&version)!=VK_SUCCESS||version<VK_API_VERSION_1_4){
        std::cout<<"SKIP: Vulkan 1.4 loader unavailable\n";return 77;
    }
    Device owner;
    VkApplicationInfo app{VK_STRUCTURE_TYPE_APPLICATION_INFO};app.apiVersion=VK_API_VERSION_1_4;
    VkInstanceCreateInfo ic{VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO};ic.pApplicationInfo=&app;
    if(vkCreateInstance(&ic,nullptr,&owner.instance)!=VK_SUCCESS){std::cout<<"SKIP: Vulkan instance unavailable\n";return 77;}
    uint32_t count=0;
    if(vkEnumeratePhysicalDevices(owner.instance,&count,nullptr)!=VK_SUCCESS||!count){std::cout<<"SKIP: no Vulkan physical device\n";return 77;}
    std::vector<VkPhysicalDevice> physicals(count);
    if(vkEnumeratePhysicalDevices(owner.instance,&count,physicals.data())!=VK_SUCCESS)return fail("enumerate GPUs");
    VkPhysicalDevice physical{};uint32_t family=0;
    for(auto p:physicals){
        VkPhysicalDeviceProperties props{};vkGetPhysicalDeviceProperties(p,&props);
        if(props.apiVersion<VK_API_VERSION_1_4||props.limits.maxStorageBufferRange<batch_word_limit*4)continue;
        uint32_t n=0;vkGetPhysicalDeviceQueueFamilyProperties(p,&n,nullptr);
        std::vector<VkQueueFamilyProperties> queues(n);vkGetPhysicalDeviceQueueFamilyProperties(p,&n,queues.data());
        for(uint32_t i=0;i<n;++i)if(queues[i].queueCount&&(queues[i].queueFlags&VK_QUEUE_COMPUTE_BIT)){
            physical=p;family=i;break;
        }
        if(physical)break;
    }
    if(!physical){std::cout<<"SKIP: no Vulkan 1.4 compute device\n";return 77;}
    float priority=1.f;
    VkDeviceQueueCreateInfo qi{VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO};qi.queueFamilyIndex=family;qi.queueCount=1;qi.pQueuePriorities=&priority;
    VkDeviceCreateInfo di{VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO};di.queueCreateInfoCount=1;di.pQueueCreateInfos=&qi;
    if(vkCreateDevice(physical,&di,nullptr,&owner.device)!=VK_SUCCESS)return fail("create device");
    VkQueue queue{};vkGetDeviceQueue(owner.device,family,0,&queue);
    VkCommandPoolCreateInfo pi{VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO};pi.queueFamilyIndex=family;pi.flags=VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    if(vkCreateCommandPool(owner.device,&pi,nullptr,&owner.pool)!=VK_SUCCESS)return fail("create pool");
    VkCommandBufferAllocateInfo ca{VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO};ca.commandPool=owner.pool;ca.level=VK_COMMAND_BUFFER_LEVEL_PRIMARY;ca.commandBufferCount=1;
    VkCommandBuffer cmd{};
    if(vkAllocateCommandBuffers(owner.device,&ca,&cmd)!=VK_SUCCESS)return fail("allocate command");
    wchar_t exe[32768]{};
    const auto len=GetModuleFileNameW(nullptr,exe,32768);
    if(!len||len>=32768)return fail("shader location");
    VulkanSpriteBatch batch;std::string error;
    if(!batch.initialize(physical,owner.device,std::filesystem::path(exe).parent_path()/L"shaders"/L"spriteBatch.spv",error))
        return fail(error.c_str());
    std::mt19937 rng(0x425442);
    std::vector<uint32_t> atlas(128*128);
    for(auto& c:atlas)c=(rng()%5==0)?0xffff00ffU:(0xff000000U|(rng()&0xffffffU));
    std::vector<SpriteJob> jobs;
    for(unsigned i=0;i<192;++i){
        btb::full_game::Draw draw{"fixture",int(rng()%720)-40,int(rng()%560)-40,{},i%3!=0};
        draw.source_rectangle=btb::full_game::Rect{int(i%32),int(i%16),int(i%32)+64,int(i%16)+48};
        if(i%4==0)draw.destination_clip=btb::full_game::Rect{20,20,620,400};
        SpriteJob job{};
        if(!make_sprite_job(job,draw,128,128,0))return fail("make job");
        if(job.width&&job.height)jobs.push_back(job);
    }
    std::vector<uint32_t> packed;
    if(!pack_sprite_batch(jobs,atlas,packed))return fail("pack oracle");
    for(bool bgra:{false,true}){
        if(!batch.prepare_jobs(jobs,atlas))return fail("prepare GPU");
        if(vkResetCommandBuffer(cmd,0)!=VK_SUCCESS)return fail("reset");
        VkCommandBufferBeginInfo begin{VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
        if(vkBeginCommandBuffer(cmd,&begin)!=VK_SUCCESS)return fail("begin");
        batch.record(cmd,bgra);
        if(vkEndCommandBuffer(cmd)!=VK_SUCCESS)return fail("end");
        VkSubmitInfo si{VK_STRUCTURE_TYPE_SUBMIT_INFO};si.commandBufferCount=1;si.pCommandBuffers=&cmd;
        if(vkQueueSubmit(queue,1,&si,VK_NULL_HANDLE)!=VK_SUCCESS||vkQueueWaitIdle(queue)!=VK_SUCCESS)return fail("dispatch");
        const auto pixels=batch.readback();
        if(pixels.size()!=640*480)return fail("readback size");
        for(unsigned y=0;y<480;++y)for(unsigned x=0;x<640;++x){
            const auto expected=sprite_batch_pixel(packed,x,y,bgra);
            if(pixels[y*640+x]!=expected){std::cerr<<"Pixel mismatch at "<<x<<","<<y<<" BGRA="<<bgra<<'\n';return 2;}
        }
    }
    btb::full_game::OpaqueGlobalVideo video;
    btb::bink32_recovered::BinkNativeYuVaFrame frame;
    frame.width=640;frame.height=480;frame.source_has_alpha=false;
    video.planes[0].resize(640*480);video.planes[1].resize(320*240);video.planes[2].resize(320*240);
    for(auto& plane:video.planes)for(auto& c:plane)c=static_cast<uint8_t>(rng());
    for(unsigned i=0;i<3;++i)frame.planes[i]=video.planes[i];
    frame.planes[3].assign(640*480,255);
    std::vector<uint8_t> expected(640*480*4,255);
    for(bool bgra:{false,true}){
        btb::bink32_recovered::BinkPackedSurface cpu{expected,640*4,640,480,4,
            bgra?0xff0000U:0xffU,0xff00U,bgra?0xffU:0xff0000U};
        if(btb::bink32_recovered::blit_bink_yuva_to_surface(frame,cpu)!=btb::bink32_recovered::BinkSurfaceResult::ok)return fail("CPU Bink reference");
        if(!batch.prepare_opaque_video(video))return fail("prepare GPU video");
        if(vkResetCommandBuffer(cmd,0)!=VK_SUCCESS)return fail("video reset");
        VkCommandBufferBeginInfo begin{VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
        if(vkBeginCommandBuffer(cmd,&begin)!=VK_SUCCESS)return fail("video begin");
        batch.record(cmd,bgra);
        if(vkEndCommandBuffer(cmd)!=VK_SUCCESS)return fail("video end");
        VkSubmitInfo si{VK_STRUCTURE_TYPE_SUBMIT_INFO};si.commandBufferCount=1;si.pCommandBuffers=&cmd;
        if(vkQueueSubmit(queue,1,&si,VK_NULL_HANDLE)!=VK_SUCCESS||vkQueueWaitIdle(queue)!=VK_SUCCESS)return fail("video dispatch");
        const auto pixels=batch.readback();
        for(std::size_t i=0;i<pixels.size();++i){
            const auto* p=expected.data()+4*i;
            const uint32_t reference=uint32_t(p[0])|(uint32_t(p[1])<<8)|(uint32_t(p[2])<<16)|(uint32_t(p[3])<<24);
            if(pixels[i]!=reference){std::cerr<<"Bink mismatch at pixel "<<i<<" BGRA="<<bgra<<'\n';return 3;}
        }
    }
    std::cout<<"PASS: 614400 opaque Bink GPU pixels match native CPU conversion\n";
    std::cout<<"PASS: 614400 GPU-computed pixels match portable painter-order oracle\n";
    return 0;
}
