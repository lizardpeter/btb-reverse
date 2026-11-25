#pragma once
#if defined(_WIN32)
#include <windows.h>
#include <filesystem>
namespace btb::enhanced {
int run_enhanced(HINSTANCE,const std::filesystem::path&,int,bool,int);
}
#endif
