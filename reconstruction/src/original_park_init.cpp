#include "btb/original_park_init.hpp"
#include <algorithm>
#include <array>
#include <cstddef>
#include <stdexcept>
namespace btb::original_park_init {
namespace {
struct Asset {U32 slot,name;bool keyed;};
// 38 direct-source resource registrations transcribed from original
// 0040B7E0. Indexed and stack-assembled path families follow below.
constexpr std::array<Asset,38> kOriginalBitmaps{{
{0x507a00U,0x4425fcU,true},
{0x508ad0U,0x43f978U,true},
{0x509250U,0x440744U,false},
{0x509254U,0x4425dcU,true},
{0x509258U,0x4407c4U,true},
{0x50925cU,0x440844U,true},
{0x509260U,0x4408c4U,true},
{0x509264U,0x440944U,true},
{0x509268U,0x4409c4U,true},
{0x50926cU,0x4409c4U,false},
{0x509270U,0x440a44U,false},
{0x509274U,0x440ac4U,false},
{0x5092b4U,0x4425b0U,true},
{0x5092b0U,0x442580U,true},
{0x4fca90U,0x442560U,true},
{0x4fcaa8U,0x442540U,true},
{0x507b54U,0x44251cU,true},
{0x508c0cU,0x442500U,true},
{0x509278U,0x4424e4U,true},
{0x50927cU,0x4424c8U,true},
{0x509280U,0x4424acU,true},
{0x509284U,0x442490U,true},
{0x509288U,0x442470U,true},
{0x50928cU,0x442454U,true},
{0x509290U,0x442438U,true},
{0x509294U,0x442414U,true},
{0x509298U,0x4423f0U,true},
{0x50929cU,0x4423d0U,true},
{0x5092b8U,0x4423acU,true},
{0x5092bcU,0x44238cU,true},
{0x5092c0U,0x44236cU,true},
{0x5092c4U,0x44234cU,true},
{0x5092c8U,0x442328U,true},
{0x5092ccU,0x442308U,true},
{0x5092d0U,0x4422e4U,true},
{0x5092d4U,0x4422c0U,true},
{0x5092d8U,0x44229cU,true},
{0x5092dcU,0x442278U,true}
}};
constexpr U32 kMagenta=0x00ff00ffU;
void add_surface(State& s,IO& io,U32 slot,U32 source,U32 handle,bool keyed){
 if(handle==0)throw std::runtime_error("retail Park Designer surface missing");
 for(const auto& item:s.surfaces)
  if(item.guest_slot==slot)throw std::logic_error("duplicate original surface slot");
 s.surfaces.push_back({slot,source,handle,keyed});
 if(keyed)io.set_magenta_key(slot,handle,kMagenta);
}
U32 surface(const State& s,U32 slot){
 for(const auto& value:s.surfaces)
  if(value.guest_slot==slot)return value.native_handle;
 throw std::out_of_range("original palette surface dependency unavailable");
}
void init_persistent(State& s){
 for(auto& record:s.save.objects){
  record={};
  record.left=-1;
  record.object_code=-1;
 }
 s.save.trailing_state.fill(0);
 using park_designer::TrailingStateIndex;
 for(auto idx:{TrailingStateIndex::PondSelectedPrimary,
               TrailingStateIndex::PondSpecialRecord,
               TrailingStateIndex::BandstandSelectedRecord,
               TrailingStateIndex::SelectedObjectRecord,
               TrailingStateIndex::PondLinkedRecord0,
               TrailingStateIndex::PondLinkedRecord1,
               TrailingStateIndex::PondLinkedRecord2,
               TrailingStateIndex::PondLinkedRecord3,
               TrailingStateIndex::PondLinkedRecord4}){
  s.save.state(idx)=-1;
 }
 s.save.state(TrailingStateIndex::NextPondPrimaryRecord)=0;
 s.save.state(TrailingStateIndex::NextDecorateRecord)=300;
 s.save.state(TrailingStateIndex::NextBandstandRecord)=200;
 s.save.state(TrailingStateIndex::DecoratePaletteMaxPage)=3;
 s.selected_pond=s.selected_bandstand=s.selected_object=-1;
 s.current_season=0;
 s.game_mode=4;
 s.stage=s.ending_stage=s.completion_phase=0;
 s.scene_music_flag=0;
}
void register_indexed(State& s,IO& io,U32 slot,U32 va,U32 offset,bool keyed=true){
 const U32 handle=io.load_indexed_original_bitmap(slot,va,offset);
 add_surface(s,io,slot,va+offset,handle,keyed);
}
void register_toolbar(State& s,IO& io,U32 slot,I32 group,I32 ordinal){
 const U32 handle=io.load_original_toolbar_bitmap(slot,group,ordinal);
 add_surface(s,io,slot,0U,handle,true);
}
}
void initialize_park_designer(State& s,IO& io){
 // Original resets game stage and installs the native activity unload.
 s=State{};
 init_persistent(s);
 io.register_original_unload();s.register_unload=true;
 for(const auto& a:kOriginalBitmaps){
  const U32 handle=io.load_original_bitmap(a.slot,a.name);
  add_surface(s,io,a.slot,a.name,handle,a.keyed);
 }
 // Source 0040B7E0: five seasonal (summer/winter) pond variants.
 // Each retail path entry occupies 0x80 bytes.
 for(U32 v=0;v<5;++v){
  register_indexed(s,io,0x00507b3cU+4*v,0x00441344U,0x80*v);
  register_indexed(s,io,0x004fca94U+4*v,0x004415c4U,0x80*v);
 }
 // Original arrow bitmap family, nine entries 0x80 apart.
 for(U32 i=0;i<9;++i)
  register_indexed(s,io,0x00507b18U+4*i,0x00440bc4U,0x80*i);
 // The original constructs the following filenames on the stack by
 // concatenating table-defined paths. It is incorrect to fabricate these
 // strings from source-symbol labels.
 for(I32 i=0;i<3;++i)
  register_toolbar(s,io,0x00507a14U+4*static_cast<U32>(i),0,i);
 for(I32 i=0;i<10;++i)
  register_toolbar(s,io,0x00504190U+4*static_cast<U32>(i),1,i);
 for(I32 i=0;i<12;++i)
  register_toolbar(s,io,0x00508ad4U+4*static_cast<U32>(i),2,i);

 // Set up the original 3×4×5 palette cells: DAT_00507FB8 stride 0x30.
 // Source rectangles are provided by the original surface metadata
 // and permutation tables, never by a guessed atlas layout.
 for(I32 mode=0;mode<3;++mode)
  for(I32 category=0;category<4;++category)
   for(I32 variant=0;variant<5;++variant){
    const std::size_t flat=park_designer::bound_polygon_flat_index(
      static_cast<std::size_t>(mode),
      static_cast<std::size_t>(category),
      static_cast<std::size_t>(variant));
    const I32 left=variant*0x22,top=category*0x3d;
    const U32 atlas=mode==0?0x0050926cU:
                    mode==1?0x00509288U:0x00509278U;
    const U32 handle=surface(s,atlas);
    const auto rect=io.original_palette_source_rect(mode,category,variant,handle);
    if(rect[2]<rect[0]||rect[3]<rect[1])
     throw std::runtime_error("invalid original Park Designer palette dimensions");
    s.palette[flat]={mode,category,variant,left,top,left+0x22,top+0x3d,
                     rect[0],rect[1],rect[2],rect[3],handle,-1,-1};
   }
 // 0040B7E0 calls the native data loader only after installing the
 // DirectDraw asset tables and palette source rectangles.
 io.load_original_park_data(s.save,s.bounds);
 s.source_data_loaded=true;
 io.stop_all_managed_sounds();
 s.sound_stopped=true;
 using park_designer::TrailingStateIndex;
 const I32 pond=s.save.state(TrailingStateIndex::NextPondPrimaryRecord);
 const I32 decor=s.save.state(TrailingStateIndex::NextDecorateRecord);
 const I32 band=s.save.state(TrailingStateIndex::NextBandstandRecord);
 // Original DAT_0051C30C starts activity music only if no objects.
 s.scene_music_flag=(pond<=0&&decor<=300&&band<=200)?1:0;
 s.selected_pond=s.save.state(TrailingStateIndex::PondSelectedPrimary);
 s.selected_bandstand=s.save.state(TrailingStateIndex::BandstandSelectedRecord);
 s.selected_object=s.save.state(TrailingStateIndex::SelectedObjectRecord);
 s.current_season=s.save.state(TrailingStateIndex::Season);
 s.initialized=true;
}
} // namespace btb::original_park_init
