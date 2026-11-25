#include "vulkan_sprite_batch.hpp"
#include "btb/bink32_surface_convert.hpp"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <array>
#include <random>
#if defined(_WIN32)
#include <windows.h>
#endif
namespace {
struct Device {
    VkInstance instance{};VkDevice device{};VkCommandPool pool{};
    ~Device(){if(device){vkDeviceWaitIdle(device);if(pool)vkDestroyCommandPool(device,pool,nullptr);vkDestroyDevice(device,nullptr);}
        if(instance)vkDestroyInstance(instance,nullptr);}
};
int fail(const char* error){std::cerr<<error<<'\n';return 1;}
}
int main(int argc,char** argv){
    using namespace btb::enhanced;
    // The production Win32 renderer requires 1.4. The headless differential
    // requires only Vulkan 1.2 (SPIR-V 1.5); it executes the identical shaders.
    #if defined(_WIN32)
    constexpr auto minimum_api=VK_API_VERSION_1_4;
    #else
    constexpr auto minimum_api=VK_API_VERSION_1_2;
    #endif
    uint32_t version=0;
    if(vkEnumerateInstanceVersion(&version)!=VK_SUCCESS||version<minimum_api){
        std::cout<<"SKIP: required Vulkan loader unavailable\n";return 77;
    }
    Device owner;
    VkApplicationInfo app{VK_STRUCTURE_TYPE_APPLICATION_INFO};app.apiVersion=minimum_api;
    VkInstanceCreateInfo ic{VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO};ic.pApplicationInfo=&app;
    const auto created=vkCreateInstance(&ic,nullptr,&owner.instance);
    if(created==VK_ERROR_INCOMPATIBLE_DRIVER){std::cout<<"SKIP: Vulkan incompatible driver\n";return 77;}
    if(created!=VK_SUCCESS)return fail("create Vulkan instance");
    uint32_t count=0;
    if(vkEnumeratePhysicalDevices(owner.instance,&count,nullptr)!=VK_SUCCESS||!count){std::cout<<"SKIP: no Vulkan physical device\n";return 77;}
    std::vector<VkPhysicalDevice> physicals(count);
    if(vkEnumeratePhysicalDevices(owner.instance,&count,physicals.data())!=VK_SUCCESS)return fail("enumerate GPUs");
    VkPhysicalDevice physical{};uint32_t family=0;
    for(auto p:physicals){
        VkPhysicalDeviceProperties props{};vkGetPhysicalDeviceProperties(p,&props);
        if(props.apiVersion<minimum_api||props.limits.maxStorageBufferRange<batch_word_limit*4)continue;
        uint32_t n=0;vkGetPhysicalDeviceQueueFamilyProperties(p,&n,nullptr);
        std::vector<VkQueueFamilyProperties> queues(n);vkGetPhysicalDeviceQueueFamilyProperties(p,&n,queues.data());
        for(uint32_t i=0;i<n;++i)if(queues[i].queueCount&&(queues[i].queueFlags&VK_QUEUE_COMPUTE_BIT)){
            physical=p;family=i;break;
        }
        if(physical)break;
    }
    if(!physical){std::cout<<"SKIP: no compatible Vulkan compute device\n";return 77;}
    VkPhysicalDeviceProperties chosen{};vkGetPhysicalDeviceProperties(physical,&chosen);
    std::cout<<"Executing compute on "<<chosen.deviceName<<" API "<<VK_VERSION_MAJOR(chosen.apiVersion)<<"."<<VK_VERSION_MINOR(chosen.apiVersion)<<'\n';
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
    std::filesystem::path shader_dir;
    if(argc==2)shader_dir=argv[1];
    else {
        #if defined(_WIN32)
        wchar_t exe[32768]{};
        const auto len=GetModuleFileNameW(nullptr,exe,32768);
        if(!len||len>=32768)return fail("shader location");
        shader_dir=std::filesystem::path(exe).parent_path()/"shaders";
        #else
        return fail("pass the directory containing compiled compute SPIR-V");
        #endif
    }
    VulkanSpriteBatch batch;std::string error;
    if(!batch.initialize(physical,owner.device,shader_dir/"spriteBatch.spv",error))
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
    // Exercise the production BMP-file -> decoded pixels -> GPU atlas path.
    struct TempBmp {
        std::filesystem::path path;
        ~TempBmp(){std::error_code ec;if(!path.empty())std::filesystem::remove(path,ec);}
    } fixture;
    fixture.path=std::filesystem::temp_directory_path()/
        ("btb-gpu-bmp-atlas-"+std::to_string(rng())+".bmp");
    std::vector<std::uint8_t> bmp(70,0);
    const auto put16=[&](std::size_t p,std::uint16_t v){bmp[p]=std::uint8_t(v);bmp[p+1]=std::uint8_t(v>>8);};
    const auto put32=[&](std::size_t p,std::uint32_t v){for(unsigned i=0;i<4;++i)bmp[p+i]=std::uint8_t(v>>(8*i));};
    bmp[0]='B';bmp[1]='M';put32(2,70);put32(10,54);put32(14,40);
    put32(18,2);put32(22,2);put16(26,1);put16(28,24);put32(30,0);put32(34,16);
    // Bottom-up rows: blue + keyed magenta, then red + green.
    const std::array<std::uint8_t,16> bmp_pixels{
        255,0,0, 255,0,255, 0,0,
        0,0,255, 0,255,0, 0,0
    };
    std::copy(bmp_pixels.begin(),bmp_pixels.end(),bmp.begin()+54);
    {
        std::ofstream file(fixture.path,std::ios::binary|std::ios::trunc);
        file.write(reinterpret_cast<const char*>(bmp.data()),static_cast<std::streamsize>(bmp.size()));
        if(!file)return fail("write GPU BMP fixture");
    }
    btb::full_game::ResolvedDraw file_draw{
        {"bmp-fixture",40,50,{},true},
        {btb::full_game::AssetResolutionStatus::FoundInstalled,fixture.path,{}}
    };
    const std::vector<btb::full_game::ResolvedDraw> file_draws{file_draw};
    if(!batch.prepare(file_draws))return fail("decode BMP into GPU atlas");
    if(vkResetCommandBuffer(cmd,0)!=VK_SUCCESS)return fail("BMP reset");
    VkCommandBufferBeginInfo bmp_begin{VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
    if(vkBeginCommandBuffer(cmd,&bmp_begin)!=VK_SUCCESS)return fail("BMP begin");
    batch.record(cmd,false);
    if(vkEndCommandBuffer(cmd)!=VK_SUCCESS)return fail("BMP end");
    VkSubmitInfo bmp_submit{VK_STRUCTURE_TYPE_SUBMIT_INFO};bmp_submit.commandBufferCount=1;bmp_submit.pCommandBuffers=&cmd;
    if(vkQueueSubmit(queue,1,&bmp_submit,VK_NULL_HANDLE)!=VK_SUCCESS||vkQueueWaitIdle(queue)!=VK_SUCCESS)
        return fail("BMP dispatch");
    const auto bmp_output=batch.readback();
    if(bmp_output[50*640+40]!=0xff0000ffU||bmp_output[50*640+41]!=0xff00ff00U||
       bmp_output[51*640+40]!=0xffff0000U||bmp_output[51*640+41]!=0xff000000U)
        return fail("GPU BMP orientation / BGRA conversion / magenta key");
    std::cout<<"PASS: original BMP file decodes into GPU atlas with correct orientation, channel order and color key\n";
    // Exercise the exact resident storage path used in interpolation.
    const auto reserved=reserve_atlas_base(jobs.size());
    if(!batch.upload_resident_atlas(atlas,reserved))return fail("resident upload");
    auto dispatch_resident=[&](const std::vector<SpriteJob>& moving,bool bgra){
        if(!pack_sprite_batch(moving,atlas,packed)||!batch.update_resident_jobs(moving))return false;
        if(vkResetCommandBuffer(cmd,0)!=VK_SUCCESS)return false;
        VkCommandBufferBeginInfo begin{VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
        if(vkBeginCommandBuffer(cmd,&begin)!=VK_SUCCESS)return false;
        batch.record(cmd,bgra);
        if(vkEndCommandBuffer(cmd)!=VK_SUCCESS)return false;
        VkSubmitInfo si{VK_STRUCTURE_TYPE_SUBMIT_INFO};si.commandBufferCount=1;si.pCommandBuffers=&cmd;
        if(vkQueueSubmit(queue,1,&si,VK_NULL_HANDLE)!=VK_SUCCESS||vkQueueWaitIdle(queue)!=VK_SUCCESS)return false;
        const auto pixels=batch.readback();if(pixels.size()!=640*480)return false;
        for(unsigned y=0;y<480;++y)for(unsigned x=0;x<640;++x)
            if(pixels[y*640+x]!=sprite_batch_pixel(packed,x,y,bgra))return false;
        return true;
    };
    if(!dispatch_resident(jobs,false))return fail("resident first frame");
    auto moving=jobs;
    for(auto& j:moving){if(j.dst_x+j.width+3<=640)j.dst_x+=3;if(j.dst_y>=2)j.dst_y-=2;}
    if(!dispatch_resident(moving,true))return fail("resident moving frame without atlas reupload");
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
    if(batch.update_resident_jobs(jobs))return fail("video failed to invalidate resident atlas");
    if(!batch.upload_resident_atlas(atlas,reserved)||!dispatch_resident(jobs,false))return fail("resident reload after video");
    // Actual packed-layer path, including an RGB565 underlay, two
    // overlapping decoded surfaces, and opaque magenta movie pixels.
    auto under=std::make_shared<btb::display::PackedFrameSnapshot>();
    std::vector<uint8_t> raw(640*480*2);
    for(std::size_t i=0;i<raw.size();i+=2){raw[i]=0;raw[i+1]=0xf8;}
    if(!btb::display::capture_packed_frame(*under,raw,640*2,640,480,{2,0xf800,0x7e0,0x1f}))return fail("layer underlay");
    auto overlay=std::make_shared<btb::display::PackedFrameSnapshot>();
    std::vector<uint8_t> rgba(282*179*4);
    for(std::size_t i=0;i<rgba.size();i+=4){rgba[i]=255;rgba[i+1]=0;rgba[i+2]=255;rgba[i+3]=0;}
    if(!btb::display::capture_packed_frame(*overlay,rgba,282*4,282,179,{4,0xff,0xff00,0xff0000}))return fail("layer movie");
    std::vector<btb::full_game::FrameCompositionLayer> layers{btb::full_game::PackedCompositionLayer{under,0,0},btb::full_game::PackedCompositionLayer{overlay,30,40},btb::full_game::PackedCompositionLayer{under,620,470}};
    if(!batch.prepare_layers(layers))return fail("prepare mixed layers");
    if(vkResetCommandBuffer(cmd,0)!=VK_SUCCESS)return fail("layer reset");
    VkCommandBufferBeginInfo layer_begin{VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
    if(vkBeginCommandBuffer(cmd,&layer_begin)!=VK_SUCCESS)return fail("layer begin");
    batch.record(cmd,false);
    if(vkEndCommandBuffer(cmd)!=VK_SUCCESS)return fail("layer end");
    VkSubmitInfo layer_submit{VK_STRUCTURE_TYPE_SUBMIT_INFO};layer_submit.commandBufferCount=1;layer_submit.pCommandBuffers=&cmd;
    if(vkQueueSubmit(queue,1,&layer_submit,VK_NULL_HANDLE)!=VK_SUCCESS||vkQueueWaitIdle(queue)!=VK_SUCCESS)return fail("layer dispatch");
    const auto layer_pixels=batch.readback();
    for(unsigned y=0;y<480;++y)for(unsigned x=0;x<640;++x){
        const uint32_t color=x>=30&&x<312&&y>=40&&y<219?0xffff00ffU:0xff0000ffU;
        if(layer_pixels[y*640+x]!=color)return fail("packed-layer pixel mismatch");
    }
    std::cout<<"PASS: 307200 packed-layer pixels match expected opaque composition\n";
    std::cout<<"PASS: 921600 resident-atlas GPU pixels match source-order oracle\n";
    std::cout<<"PASS: 614400 opaque Bink GPU pixels match native CPU conversion\n";
    std::cout<<"PASS: 614400 GPU-computed pixels match portable painter-order oracle\n";
    return 0;
}
