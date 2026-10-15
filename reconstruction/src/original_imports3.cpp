#include "btb/original_imports3.hpp"
namespace btb::original_imports3 {
I32 create_draw(Services& io,const Guid* adapter,Handle& result,const Guid& iid,Handle outer){return io.direct_draw(adapter,result,iid,outer);}
I32 create_input(Services& io,Handle instance,U32 version,const Guid& iid,Handle& result,Handle outer){return io.direct_input(instance,version,iid,result,outer);}
I32 create_sound(Services& io,const Guid* device,Handle& result,Handle outer){return io.direct_sound(device,result,outer);}
}
