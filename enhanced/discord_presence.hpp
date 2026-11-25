#pragma once
#include <atomic>
#include <thread>
namespace btb::enhanced {
// Optional, local-only IPC. Never reads profile names or save files.
class DiscordPresence {
    std::atomic<int> state_{0};
    std::jthread worker_;
public:
    void start();
    void update(int state) noexcept {state_.store(state,std::memory_order_relaxed);}
    void stop() noexcept {if(worker_.joinable()){worker_.request_stop();worker_.join();}}
    ~DiscordPresence(){stop();}
};
}
