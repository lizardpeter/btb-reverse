#include "btb/original_message1.hpp"
namespace btb::original_message1 {
I32 show_message(State& state,const char* text,const char* title,U32 flags,Services& io){
 if(!state.message){const auto module=io.load_library("user32.dll");if(!module)return 0;state.message=io.get_proc(module,"MessageBoxA");if(!state.message)return 0;state.active=io.get_proc(module,"GetActiveWindow");state.popup=io.get_proc(module,"GetLastActivePopup");}
 Handle owner=0;if(state.active){owner=io.active_window(state.active);if(owner && state.popup)owner=io.last_popup(state.popup,owner);}
 return io.message_box(state.message,owner,text,title,flags);
}
}
