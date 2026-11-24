#include "btb/original_crt9.hpp"
#include <functional>
#include <vector>
#include <string>
// Original PE32/VS2003 process startup, file, heap and string routines,
// compiled out-of-line rather than left as header-only behavior models.
namespace btb::original_crt9 {
// Source-derived runtime_processor_warning.
void runtime_processor_warning(I32 startup,I32 alternate,
    const Callback& error,const std::function<void()>& hook) {
    if(startup==1||(startup==0&&alternate==1)){
        if(error)error(252);
        if(hook)hook();
        if(error)error(255);
    }
}

// Source-derived runtime_amessage_exit.
void runtime_amessage_exit(I32 startup,I32 message,
    const std::function<void()>& prep,
    const Callback& report,const Callback& terminate) {
    if(startup==1&&prep)prep();
    if(report)report(message);
    if(terminate)terminate(255);
}

// Source-derived runtime_fatal_process.
void runtime_fatal_process(I32 startup,I32 message,
    const std::function<void()>& prep,
    const Callback& report,const Callback& exitprocess) {
    if(startup==1&&prep)prep();
    if(report)report(message);
    if(exitprocess)exitprocess(255);
}

// Source-derived check_processor_feature.
bool check_processor_feature(
    const std::function<std::optional<bool>()>& kernel32_query,
    const std::function<bool()>& fallback) {
    if(kernel32_query) {
        const auto available=kernel32_query();
        if(available)return *available;
    }
    return fallback?fallback():false;
}

// Source-derived release_heap_slot.
void release_heap_slot(U32& available,U8& slot_byte,
    U8& page_indicator,U32& full_page_count,
    const Callback& reclaim) {
    available+=slot_byte;slot_byte=0;page_indicator=0xf1;
    if(available==0xf0 && ++full_page_count==32) {
        if(reclaim)reclaim(16);
    }
}

// Source-derived call_setting_frame.
void call_setting_frame(I32 status,const Callback& before,
    const std::function<void()>& cleanup,const Callback& after){
    if(before)before(status);
    if(cleanup)cleanup();
    if(after)after(status==0x100?2:status);
}

// Source-derived assign_float_result.
std::array<U32,2> assign_float_result(bool extended,
    const std::function<std::array<U32,2>()>& two_words,
    const std::function<U32()>& one_word){
    if(extended)return two_words?two_words():std::array<U32,2>{};
    return {one_word?one_word():0,0};
}

// Source-derived duplicate_environment.
std::optional<std::vector<std::string>> duplicate_environment(
    const std::vector<std::string>* env,
    const std::function<std::optional<std::string>(const std::string&)>& copy){
    if(!env)return std::nullopt;
    std::vector<std::string> out;
    for(const auto& src:*env){
        if(copy){
            const auto cloned=copy(src);
            if(!cloned)return std::nullopt;
            out.push_back(*cloned);
        }else out.push_back(src);
    }
    return out;
}

// Source-derived convert_wide_character.
I32 convert_wide_character(char* dest,U16 wchar,
    bool multibyte,I32& err,
    const std::function<std::optional<std::string>(U16)>& converter){
    if(!dest)return 0;
    if(!multibyte&&wchar<256){
        dest[0]=static_cast<char>(wchar);return 1;
    }
    if(multibyte&&converter){
        const auto value=converter(wchar);
        if(value&&!value->empty()&&value->size()<=4){
            std::memcpy(dest,value->data(),value->size());
            return static_cast<I32>(value->size());
        }
    }
    err=42;return -1;
}

}
