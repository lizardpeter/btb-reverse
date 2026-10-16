#include "btb/full_game_dino.hpp"

#include <array>
#include <cstdint>
#include <exception>
#include <fstream>
#include <limits>
#include <utility>

namespace btb::full_game {
namespace {

// Names and nesting verified against the nine original retail Dinosaur
// folders: Data/SubGameDino/Dino/{Raptor,Tric,Trex}/{Easy,Medium,Hard}.
constexpr std::array<const char*,3> kSpecies{{"Raptor","Tric","Trex"}};
constexpr std::array<const char*,3> kDifficulties{{"Easy","Medium","Hard"}};

std::uint32_t le32(const std::array<unsigned char,26>& bytes,
                   std::size_t offset) noexcept {
    return static_cast<std::uint32_t>(bytes[offset])
        | (static_cast<std::uint32_t>(bytes[offset+1]) << 8U)
        | (static_cast<std::uint32_t>(bytes[offset+2]) << 16U)
        | (static_cast<std::uint32_t>(bytes[offset+3]) << 24U);
}

// Decode real source BMP dimensions, not made-up 44x44 fallback hitboxes.
// The retail DirectDraw loader itself consumes the complete BMP later.
bool bmp_dimensions(const std::filesystem::path& path,
                    int& width,int& height) {
    std::ifstream file(path,std::ios::binary);
    std::array<unsigned char,26> header{};
    if(!file.read(reinterpret_cast<char*>(header.data()),
                  static_cast<std::streamsize>(header.size()))
       || header[0]!='B' || header[1]!='M'
       || le32(header,14)<40) return false;
    const auto w=static_cast<std::int32_t>(le32(header,18));
    const auto h=static_cast<std::int32_t>(le32(header,22));
    if(w<=0||w>8192||h==0||h==std::numeric_limits<std::int32_t>::min()
       ||h>8192||h< -8192)return false;
    width=w;height=h<0?-h:h;
    return true;
}

Draw present_dino_command(const dino::DrawCommand& cmd,
                          const std::string& backdrop,
                          const std::vector<std::string>& pieces) {
    Draw out{};
    out.x=cmd.x;out.y=cmd.y;out.color_keyed=cmd.color_keyed;
    if(cmd.source_rect) {
        const auto& r=*cmd.source_rect;
        out.source_rectangle=Rect{r.left,r.top,r.right,r.bottom};
    }
    switch(cmd.layer) {
    case dino::DrawLayer::Background:
        out.source_asset=backdrop;break;
    case dino::DrawLayer::Bob:
        out.source_asset="Data/SubGameDino/Bob.bmp";break;
    case dino::DrawLayer::Ellis:
        out.source_asset="Data/SubGameDino/ellis.bmp";break;
    case dino::DrawLayer::PlacedPiece:
    case dino::DrawLayer::LoosePiece:
    case dino::DrawLayer::SpecialPiece:
        if(cmd.piece_index && *cmd.piece_index<pieces.size())
            out.source_asset=pieces[*cmd.piece_index];
        break;
    }
    return out;
}

} // namespace

void DinoDriver::configure_menu_state(const RetailMenuState& menu) noexcept {
    if(menu.subgame_selection_origin==game_flow::State::DinoChooserUpdate &&
       menu.selected_subgame>=0 && menu.selected_subgame<3) {
        species_=static_cast<dino::Species>(menu.selected_subgame);
    }
    if((menu.variant_selection_origin==game_flow::State::DinoPregameUpdate ||
        menu.variant_selection_origin==game_flow::State::PlayAgainDifficultyUpdate)
       && menu.source_variant>=0 && menu.source_variant<3) {
        difficulty_=static_cast<dino::Difficulty>(menu.source_variant);
    }
}

bool DinoDriver::initialize(int player_index,std::string& error) {
    if(runtime_) {
        error="Dinosaur is already initialized; unload before replay";
        return false;
    }
    if(player_index<0 ||
       player_index>=static_cast<int>(progress::kPlayerCount)) {
        error="Dinosaur player index outside original five profiles";
        return false;
    }
    const auto species=static_cast<std::size_t>(species_);
    const auto difficulty=static_cast<std::size_t>(difficulty_);
    if(species>=kSpecies.size() || difficulty>=kDifficulties.size()) {
        error="Dinosaur menu selected invalid species/difficulty";
        return false;
    }
    const auto folder=std::string("Data/SubGameDino/Dino/")+
        kSpecies[species]+"/"+kDifficulties[difficulty];
    const auto original_level=assets_.resolve(folder+"/dino.txt");
    if(!original_level.found()) {
        error="cannot resolve original Dinosaur level: "+
            folder+"/dino.txt ("+original_level.error+")";
        return false;
    }
    try {
        std::ifstream source(original_level.absolute_path);
        if(!source) {
            error="cannot open installed original Dinosaur dino.txt";
            return false;
        }
        const auto level=dino::parse_level(source);
        if(level.piece_count()==0 || level.piece_count()>64 ||
           !level.special_render_anchor()) {
            error="original Dinosaur level lacks valid pieces/anchor";
            return false;
        }
        auto candidate=std::make_unique<dino::Runtime>(level);
        std::vector<std::string> art;
        art.reserve(level.piece_count());
        for(std::size_t i=0;i<level.piece_count();++i) {
            const auto original_name=folder+"/Piece"+
                std::to_string(candidate->pieces()[i].piece_id)+".bmp";
            const auto bmp=assets_.resolve(original_name);
            int width=0,height=0;
            if(!bmp.found() ||
               !bmp_dimensions(bmp.absolute_path,width,height)) {
                error="missing or invalid original Dinosaur BMP: "+original_name;
                return false;
            }
            candidate->set_piece_dimensions(i,width,height);
            art.push_back(original_name);
        }
        const auto background=folder+"/background.bmp";
        for(const auto& original_name:{
                background,
                std::string("Data/SubGameDino/Bob.bmp"),
                std::string("Data/SubGameDino/ellis.bmp")}) {
            const auto image=assets_.resolve(original_name);
            if(!image.found()) {
                error="missing authentic Dinosaur artwork: "+original_name;
                return false;
            }
        }
        runtime_=std::move(candidate);
        level_folder_=folder;
        background_=background;
        pieces_=std::move(art);
        special_anchor_=*level.special_render_anchor();
        animations_={};
        print_state_={};
        pickup_offset_={};
        player_index_=player_index;
        startup_pending_=true;
        completion_started_=false;
        error.clear();
        return true;
    } catch(const std::exception& exception) {
        error=exception.what();
        return false;
    }
}

ActivityFrameOutput DinoDriver::advance(const ActivityFrameInput& input) {
    ActivityFrameOutput out;
    if(!runtime_) {
        out.fatal_error="Dinosaur run state entered before retail initializer";
        return out;
    }
    const dino::Vec2i cursor{input.pointer_x,input.pointer_y};

    if(startup_pending_) {
        const auto intro=dino::startup_presentation(species_);
        out.audio.push_back({AudioOperation::ManagedSoundId,{},
                             intro.intro_sound_id,
                             intro.intro_sound_priority,
                             intro.intro_arbitration_class,
                             intro.mark_intro_slot_input_interruptible});
        startup_pending_=false;
    }

    if(runtime_->mode()==dino::InteractionMode::FinalizeAcceptedDrop)
        (void)runtime_->finalize_accepted_drop();

    if(!runtime_->complete()) {
        if(runtime_->mode()==dino::InteractionMode::Idle &&
           input.click_pulse) {
            const auto chosen=runtime_->begin_drag(cursor);
            if(chosen) {
                const auto& piece=runtime_->pieces()[*chosen];
                pickup_offset_={piece.current.x-cursor.x,
                                piece.current.y-cursor.y};
                animations_.queue_pickup(input.random_value&1,
                                         (input.random_value>>1)&1);
            }
        } else if(runtime_->mode()==dino::InteractionMode::Carrying &&
                  input.click_pulse) {
            const auto index=dino::level_index(species_,difficulty_);
            const auto dropped=runtime_->try_drop(
                cursor,dino::Runtime::snap_tolerance(index));
            const auto roll=static_cast<int>(
                static_cast<std::uint32_t>(input.random_value)%6U);
            if(dropped==dino::DropResult::Accepted) {
                out.audio.push_back({AudioOperation::ManagedSoundId,{},
                    dino::correct_drop_sound_id(roll),50,1});
                (void)animations_.queue_correct_drop(roll);
            } else if(dropped==dino::DropResult::Rejected) {
                out.audio.push_back({AudioOperation::ManagedSoundId,{},
                    dino::wrong_drop_sound_id(roll),50,1});
                (void)animations_.queue_wrong_drop(roll,
                    (input.random_value>>1)&1);
            }
        }
    }

    const auto composition=dino::compose_dino_frame(
        *runtime_,animations_,species_,special_anchor_,
        dino::kInitialSpecialRenderOffsetIndex);
    for(const auto& command:composition.commands) {
        auto draw=present_dino_command(command,background_,pieces_);
        if(draw.source_asset.empty()) {
            out.fatal_error="Dinosaur original draw has no registered bitmap";
            return out;
        }
        out.draws.push_back(std::move(draw));
    }
    if(const auto carried=runtime_->mode()==dino::InteractionMode::Carrying
           ? runtime_->selected_piece() : std::nullopt) {
        // The retail cursor surface owns the carried piece until the NEXT
        // click. Releasing the first click does not attempt placement.
        // Draw the same
        // real BMP last at the original pickup offset until placement.
        const auto& piece=runtime_->pieces()[*carried];
        out.draws.push_back({
            pieces_[*carried],
            cursor.x+pickup_offset_.x,
            cursor.y+pickup_offset_.y,
            Rect{0,0,piece.source_rect.right,piece.source_rect.bottom},
            true
        });
    }

    const auto gate=dino::completion_gate(
        runtime_->complete(),input.managed_sound_playing,
        completion_started_?1:0);
    if(gate.begin_completion) {
        const auto finished=dino::begin_completion(species_,0);
        out.audio.push_back({AudioOperation::StopManagedSounds});
        out.audio.push_back({AudioOperation::ManagedSoundId,{},
            finished.completion_sound_id,
            finished.completion_sound_priority,
            finished.completion_arbitration_class});
        out.progress_writes.push_back({finished.progress_slot,1});
        out.shared_completion_code=1;
        completion_started_=true;
    }
    if(completion_started_) {
        out.draws.push_back({
            std::string(dino::completion_artwork_filename(species_)),
            0,0,std::nullopt,false
        });
        const auto print=dino::update_print_button(print_state_,{
            input.pointer_x,input.pointer_y,input.click_pulse,false
        });
        if(print.hover_sound_id) {
            out.audio.push_back({AudioOperation::ManagedSoundId,{},
                *print.hover_sound_id,print.hover_sound_priority,
                print.hover_arbitration_class});
        }
        if(print.visual!=dino::PrintVisual::None) {
            out.draws.push_back({
                print.visual==dino::PrintVisual::Pressed
                    ? "Data/SubGameFirework/certprintdep.bmp"
                    : "Data/SubGameFirework/certprint.bmp",
                print.overlay_x,print.overlay_y,std::nullopt,true
            });
        }
        if(print.invoke_shared_print) {
            // Printing is a separate shared Win32 source provider. Do not
            // silently claim the certificate was printed without it.
            out.fatal_error="original Dinosaur certificate print provider "
                            "is not yet connected";
        }
    }
    return out;
}

bool DinoDriver::unload(std::string& error) {
    runtime_.reset();
    pieces_.clear();
    level_folder_.clear();
    background_.clear();
    player_index_=-1;
    startup_pending_=false;
    completion_started_=false;
    animations_={};
    print_state_={};
    error.clear();
    return true;
}

} // namespace btb::full_game
