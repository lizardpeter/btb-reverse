#pragma once
#include "btb/original_imports3.hpp"
#if defined(_WIN32)
namespace btb::original_win32 {
class NativeImports3:public original_imports3::Services {
public:
 original_imports3::I32 direct_draw(const original_imports3::Guid*,original_imports3::Handle&,const original_imports3::Guid&,original_imports3::Handle) override;
 original_imports3::I32 direct_input(original_imports3::Handle,original_imports3::U32,const original_imports3::Guid&,original_imports3::Handle&,original_imports3::Handle) override;
 original_imports3::I32 direct_sound(const original_imports3::Guid*,original_imports3::Handle&,original_imports3::Handle) override;
};
}
#endif
