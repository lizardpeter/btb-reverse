#include "btb/win32_original_bink_exports.hpp"

#if defined(_WIN32)
#include <algorithm>
#include <cctype>
#include <string>
#include <system_error>

namespace btb::full_game {
namespace {

constexpr std::array<OriginalBinkExport,12> kRequiredOriginalExports{{
    {"BinkOpen",8,nullptr},
    {"BinkClose",4,nullptr},
    {"BinkDoFrame",4,nullptr},
    {"BinkCopyToBuffer",28,nullptr},
    {"BinkNextFrame",4,nullptr},
    {"BinkWait",4,nullptr},
    {"BinkGoto",12,nullptr},
    {"BinkPause",8,nullptr},
    {"BinkSetVolume",12,nullptr},
    {"BinkSetSoundSystem",8,nullptr},
    {"BinkOpenDirectSound",4,nullptr},
    {"BinkDDSurfaceType",4,nullptr},
}};

FARPROC lookup_export(
    HMODULE module,
    std::string_view plain,
    unsigned argument_bytes) {

    const std::string name(plain);
    if (auto found=GetProcAddress(module,name.c_str())) {
        return found;
    }
    const auto suffix="@"+std::to_string(argument_bytes);
    const auto with_underscore="_"+name+suffix;
    if (auto found=GetProcAddress(
            module,with_underscore.c_str())) {
        return found;
    }
    const auto without_underscore=name+suffix;
    return GetProcAddress(module,without_underscore.c_str());
}

} // namespace

Win32OriginalBinkExports::~Win32OriginalBinkExports() {
    unload();
}

void Win32OriginalBinkExports::unload() noexcept {
    exports_={};
    if (module_) {
        FreeLibrary(module_);
        module_=nullptr;
    }
}

bool Win32OriginalBinkExports::load_exact_original(
    const std::filesystem::path& dll_path,
    std::string& error) {

    unload();
    if (!dll_path.is_absolute()) {
        error="original binkw32.dll must be loaded by absolute path";
        return false;
    }

    std::wstring filename=dll_path.filename().wstring();
    std::transform(
        filename.begin(),filename.end(),filename.begin(),
        [](wchar_t c) {
            return c>=L'A' && c<=L'Z' ? wchar_t(c+32) : c;
        });
    if (filename!=L"binkw32.dll") {
        error="unexpected Bink library filename; expected original binkw32.dll";
        return false;
    }

    std::error_code ec;
    if (!std::filesystem::is_regular_file(dll_path,ec) || ec) {
        error="the original binkw32.dll file was not supplied";
        return false;
    }

    // Explicit full path avoids searching the process current working
    // directory or silently picking up a different unrelated Bink DLL.
    module_=LoadLibraryExW(
        dll_path.c_str(),nullptr,
        LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR|
        LOAD_LIBRARY_SEARCH_DEFAULT_DIRS);
    if (!module_) {
        const auto win32_code=GetLastError();
        error="LoadLibraryExW failed for the original Bink DLL (Win32 "+
              std::to_string(win32_code)+
              "). This may indicate an x86/x64 architecture mismatch.";
        return false;
    }

    exports_=kRequiredOriginalExports;
    for (auto& symbol : exports_) {
        symbol.proc=lookup_export(
            module_,symbol.name,symbol.stdcall_argument_bytes);
        if (!symbol.proc) {
            error="original Bink DLL is missing imported symbol "+
                  std::string(symbol.name);
            unload();
            return false;
        }
    }
    error.clear();
    return true;
}

FARPROC Win32OriginalBinkExports::find(
    std::string_view symbol) const noexcept {

    if (!module_) {
        return nullptr;
    }
    for (const auto& e : exports_) {
        if (e.name==symbol) {
            return e.proc;
        }
    }
    return nullptr;
}

} // namespace btb::full_game
#endif
