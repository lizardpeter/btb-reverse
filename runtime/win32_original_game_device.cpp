#include "btb/win32_original_game_device.hpp"
#include "btb/win32_retail_print_provider.hpp"

#if defined(_WIN32)
namespace btb::full_game {

bool Win32OriginalGameDevice::observe(
    PhysicalFrameObservation& out,std::string& error) {

    if (!audio_.observe(
            out.managed,out.any_managed_voice_playing,error)) {
        return false;
    }
    // The real Bink provider services decode/timing before the restored
    // original 68-state game dispatch reads movie-completion status.
    return bink_.service(out.global_movie_finished,error);
}

bool Win32OriginalGameDevice::open_global_movie(
    const std::filesystem::path& movie,std::string& error) {
    return bink_.open_global(movie,error);
}

bool Win32OriginalGameDevice::close_global_movie(std::string& error) {
    return bink_.close_global(error);
}

bool Win32OriginalGameDevice::open_walkthrough_movie(
    const std::filesystem::path& movie,
    std::string& error) {
    return bink_.open_walkthrough(movie,error);
}

bool Win32OriginalGameDevice::close_walkthrough_movie(
    std::string& error) {
    return bink_.close_walkthrough(error);
}

bool Win32OriginalGameDevice::apply_sound(
    const ResolvedSoundEffect& effect,std::string& error) {
    return audio_.apply(effect,error);
}

bool Win32OriginalGameDevice::draw_ordered(
    const std::vector<ResolvedDraw>& stream,std::string& error) {

    std::vector<ResolvedDraw> before,after;
    before.reserve(stream.size());
    for(const auto& draw:stream)
        (draw.original.after_activity_movie?after:before).push_back(draw);
    if (!draw_.render_ordered(before,error)) {
        return false;
    }
    // Original Spud Skate's selected 600x380 decoded Bink stream is
    // composited at (20,20) over the source BMP backdrop. This never
    // replaces/forges activity frames or skips the actual movie decoder.
    if(activity_movie_overlay_ && !activity_movie_overlay_(error))
        return false;
    if(!after.empty()&&!draw_.render_additional(after,error))return false;
    // Source-backed looping walkthrough Bink overlays belong above the
    // original instructional background and button surfaces.
    return bink_.render_walkthrough_overlay(error);
}

bool Win32OriginalGameDevice::capture_replay_underlay(std::string& error) {
    return draw_.capture_replay_underlay(error);
}

bool Win32OriginalGameDevice::draw_replay_ordered(
    const std::vector<ResolvedDraw>& draws,std::string& error) {
    // No active minigame movie or walkthrough may paint over this UI.
    return draw_.render_replay_ordered(draws,error);
}

bool Win32OriginalGameDevice::print_current_frame(std::string& error) {
    return Win32RetailPrintProvider::print_original_backbuffer(
        draw_.bink_render_target(),draw_.window_handle(),error);
}

bool Win32OriginalGameDevice::present(std::string& error) {
    return draw_.present(error);
}

} // namespace btb::full_game
#endif
