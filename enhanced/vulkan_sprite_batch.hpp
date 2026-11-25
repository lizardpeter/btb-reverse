#pragma once
#include "sprite_batch_plan.hpp"
#include "btb/full_game_resources.hpp"
#include <vulkan/vulkan.h>
#include <filesystem>
#include <unordered_map>
namespace btb::enhanced {
class VulkanSpriteBatch {
    struct Buffer {VkBuffer buffer{};VkDeviceMemory memory{};void* mapped{};};
    struct Bitmap {unsigned width{},height{};std::vector<std::uint32_t> pixels;};
    VkDevice device_{};VkPhysicalDevice physical_{};
    Buffer input_{},output_{};
    VkDescriptorSetLayout layout_{};VkDescriptorPool pool_{};
    VkDescriptorSet set_{};VkPipelineLayout pipeline_layout_{};VkPipeline pipeline_{};
    std::unordered_map<std::filesystem::path,Bitmap> cache_;
    std::size_t cached_words_{};
    std::vector<std::uint32_t> atlas_,words_;
    std::vector<SpriteJob> jobs_;
    bool buffer(Buffer&,VkDeviceSize,VkBufferUsageFlags,std::string&);
    void destroy(Buffer&) noexcept;
    Bitmap* load(const std::filesystem::path&);
public:
    ~VulkanSpriteBatch(){shutdown();}
    bool initialize(VkPhysicalDevice,VkDevice,const std::filesystem::path&,std::string&);
    bool prepare(const std::vector<full_game::ResolvedDraw>&);
    bool prepare_jobs(std::span<const SpriteJob>,std::span<const std::uint32_t>);
    std::span<const std::uint32_t> readback() const noexcept {
        return output_.mapped?std::span<const std::uint32_t>{static_cast<const std::uint32_t*>(output_.mapped),640*480}:std::span<const std::uint32_t>{};
    }
    // Only called after the previous submission's fence is signaled.
    void record(VkCommandBuffer,bool bgra);
    VkBuffer pixels() const noexcept{return output_.buffer;}
    void shutdown() noexcept;
};
}
