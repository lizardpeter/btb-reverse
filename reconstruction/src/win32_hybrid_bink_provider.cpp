#include "btb/win32_hybrid_bink_provider.hpp"
#if defined(_WIN32)
#include <string>

namespace btb::full_game {
void Win32HybridBinkProvider::cancel_global_movie() noexcept {
    native_.reset_global();
    retail_.cancel_global_movie();
    global_=Backend::none;
}
bool Win32HybridBinkProvider::open_global(
    const std::filesystem::path& movie,std::string& error) {
    // The old movie's handles are released BEFORE choosing the backend
    // for the next global cinematic. Never leave a retail decoder alive
    // behind a native movie that has taken over the game backbuffer.
    cancel_global_movie();
    std::string native_error;
    if(native_enabled_ && native_.open_global(movie,native_error)) {
        global_=Backend::native;
        error.clear();
        return true;
    }
    std::string retail_error;
    if(retail_.open_global(movie,retail_error)) {
        global_=Backend::retail;
        error.clear();
        return true;
    }
    error="native Bink: "+native_error+"; original Bink: "+retail_error;
    return false;
}

bool Win32HybridBinkProvider::service(
    bool& global_finished,std::string& error) {
    global_finished=false;
    bool ok=true;
    switch(global_) {
    case Backend::native:
        ok=native_.service(global_finished,error);
        break;
    case Backend::retail:
        ok=retail_.service(global_finished,error);
        break;
    case Backend::none:
        error.clear();
        return true;
    }
    if(ok&&global_finished) {
        global_=Backend::none;
        // Native and retail providers release their own completed movie.
    }
    return ok;
}

bool Win32HybridBinkProvider::open_walkthrough(
    const std::filesystem::path& movie,
    const std::filesystem::path& help_wav,std::string& error) {
    std::string close_error;
    if(!native_.close_walkthrough(close_error)||
       !retail_.close_walkthrough(close_error)) {
        error=close_error;
        return false;
    }
    walkthrough_=Backend::none;
    std::string native_error;
    if(native_enabled_ && native_.open_walkthrough(movie,help_wav,native_error)) {
        walkthrough_=Backend::native;
        error.clear();
        return true;
    }
    std::string retail_error;
    if(retail_.open_walkthrough(movie,help_wav,retail_error)) {
        walkthrough_=Backend::retail;
        error.clear();
        return true;
    }
    error="native Bink walkthrough: "+native_error+
          "; original Bink walkthrough: "+retail_error;
    return false;
}

bool Win32HybridBinkProvider::close_walkthrough(
    std::string& error) {
    walkthrough_=Backend::none;
    std::string native_error,retail_error;
    const bool n=native_.close_walkthrough(native_error);
    const bool r=retail_.close_walkthrough(retail_error);
    if(!n||!r) {
        error="failed closing Bink walkthrough: "+
              native_error+"; "+retail_error;
        return false;
    }
    error.clear();
    return true;
}

bool Win32HybridBinkProvider::render_walkthrough_overlay(
    std::string& error) {
    switch(walkthrough_) {
    case Backend::native:
        return native_.render_walkthrough_overlay(error);
    case Backend::retail:
        return retail_.render_walkthrough_overlay(error);
    case Backend::none:
        error.clear();
        return true;
    }
    error="invalid Bink walkthrough backend";
    return false;
}
}
#endif
