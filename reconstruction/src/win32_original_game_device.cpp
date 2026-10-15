#include "btb/win32_original_game_device.hpp"

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

bool Win32OriginalGameDevice::open_walkthrough_movie(
    const std::filesystem::path& movie,
    const std::filesystem::path& help_wav,
    std::string& error) {
    return bink_.open_walkthrough(movie,help_wav,error);
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

    // Original video decoding happens during observe(), before GameRoot.
    // Only the final present must overlay buffered Bink pixels; global
    // movie intercepts skip ordered draw entirely.
    return draw_.render_ordered(stream,error);
}

bool Win32OriginalGameDevice::present(std::string& error) {
    // Global Bink mode 13/14 intentionally bypasses draw_ordered().
    // Display the last real decoded frame even on those intercepted ticks.
    // For instruction screens this overlays the source-backed UI last.
    if (!bink_.render_walkthrough_overlay(error)) {
        return false;
    }
    return draw_.present(error);
}

} // namespace btb::full_game
#endif
