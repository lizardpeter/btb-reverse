#pragma once
#if defined(_WIN32)
#include <windows.h>
#include "native_cursor_plan.hpp"
namespace btb::enhanced {
class NativeCursor {
    HCURSOR handle_{};
    unsigned width_{},height_{};
    std::uint64_t revision_{};
    bool attempted_{};
public:
    NativeCursor()=default;
    NativeCursor(const NativeCursor&)=delete;
    NativeCursor& operator=(const NativeCursor&)=delete;
    ~NativeCursor(){reset();}
    bool ensure(const CursorPixels&,unsigned,unsigned,std::uint64_t);
    HCURSOR get()const noexcept{return handle_;}
    void reset()noexcept;
};
}
#endif
