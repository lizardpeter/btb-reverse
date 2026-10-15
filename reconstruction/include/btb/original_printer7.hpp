#pragma once
#include "btb/printing_system.hpp"
#include "btb/original_exception9.hpp"
#include <span>
#include <stdexcept>
namespace btb::original_printer7 {
using U32=std::uint32_t;using I32=std::int32_t;using Handle=std::uintptr_t;
using Rect=display::RetailRect32;
struct Bitmap {I32 width{},height{};std::uint16_t bits{};const std::byte* pixels{};};
struct DibSection {Bitmap bitmap{};std::array<std::byte,40> header{};};
struct Printer {
    std::byte* dib{};std::byte* bits{};U32 allocation_size{};
    std::array<char,260> document{};Rect target{};I32 scale_mode{3};
};
class Error final:public std::runtime_error {
public:
    explicit Error(const char*);Error(const Error&);Error& operator=(const Error&)=delete;~Error() noexcept override;
    const original_exception9::Object& retail_object() const noexcept{return object_;}
private:original_string8::NativeHeap heap_;original_exception9::Object object_{};
};
class Services {
public:
    virtual ~Services()=default;
    virtual I32 get_bitmap(Handle,Bitmap&)=0;
    virtual I32 get_section(Handle,DibSection&)=0;
    virtual std::byte* allocate(U32)=0;
    virtual void release(std::byte*) noexcept=0;
    virtual Handle screen_dc()=0;
    virtual void release_screen_dc(Handle)=0;
    virtual Handle select_palette(Handle,Handle,bool)=0;
    virtual void realize_palette(Handle)=0;
    virtual I32 get_dibits(Handle,Handle,U32,std::byte*,std::byte*)=0;
    virtual Handle compatible_dc()=0;
    virtual Handle select_bitmap(Handle,Handle)=0;
    virtual void get_colors(Handle,U32,std::byte*)=0;
    virtual void delete_dc(Handle)=0;
    virtual I32 raster_caps(Handle)=0;
    virtual void query_escape(Handle,I32 command)=0;
    virtual void query_version()=0;
    virtual void stretch_mode(Handle,I32)=0;
    virtual I32 stretch_dib(Handle,const Rect&,I32,I32,const std::byte*,const std::byte*)=0;
    // Virtual slot 0x24 is still a separately unreviewed original procedure.
    virtual void compute_target(Printer&,Handle)=0;
    virtual I32 start_doc(Handle,const char*)=0;
    virtual I32 start_page(Handle)=0;
    virtual void end_page(Handle)=0;
    virtual void end_doc(Handle)=0;
};
// 00404E30 / 00404F90 / 00405140 / 00405C40 / 00405EE0 / 004057E0
void set_bitmap(Printer&,Handle,Handle palette,Services&);
void render_to_dc(Printer&,Handle,Services&);
[[noreturn]] void rethrow_print_error(Handle,Services&); // 00405309
void print_document(Printer&,Handle,Services&);
void import_ddb(Printer&,Handle,Handle palette,Services&);
void import_section(Printer&,Handle,Services&);
void stretch_dib(Printer&,Handle,Services&);
bool verify_7_original_printer_functions();
}
