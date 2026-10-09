#include "btb/full_game_herding_presentation.hpp"

#include <algorithm>
#include <numeric>
#include <optional>
#include <string>
#include <vector>

namespace btb::full_game {
namespace {

constexpr Rect kWorldClip{
    herding::kWorldViewportDestX,herding::kWorldViewportDestY,
    herding::kWorldViewportDestX+herding::kWorldViewportWidth,
    herding::kWorldViewportDestY+herding::kWorldViewportHeight
};

Rect as_rect(herding::RenderRect source) {
    return {source.left,source.top,source.right,source.bottom};
}

void add_asset(
    std::vector<Draw>& draws,
    herding::HerdingSurface surface,
    int x,int y,
    std::optional<Rect> crop,
    bool in_world) {

    const auto* binding=herding::herding_surface_binding(surface);
    if (!binding) return;
    draws.push_back({
        std::string(binding->filename),x,y,crop,binding->color_keyed,
        in_world ? std::optional<Rect>{kWorldClip} : std::nullopt
    });
}

bool is_animating_animal(herding::EntityType type) {
    return type == herding::EntityType::Sheep ||
           type == herding::EntityType::Rabbit ||
           type == herding::EntityType::Duck;
}

} // namespace

HerdingComposedFrame compose_original_herding_frame(
    HerdingScene& scene) {

    HerdingComposedFrame frame;

    // Retail camera follows Farmer Pickles record 0, not the centroid
    // of the animal herd and not the mouse position.
    const auto hero=std::find_if(
        scene.entities.begin(),scene.entities.end(),
        [](const auto& entity) {
            return entity.entity_type() ==
                   herding::EntityType::FarmerPickles;
        });
    if (hero == scene.entities.end()) {
        frame.missing_farmer_pickles = true;
        return frame;
    }

    frame.camera=herding::herding_camera_plan(hero->x,hero->y);
    add_asset(frame.draws,herding::HerdingSurface::Background,
        frame.camera.background_dest_x,frame.camera.background_dest_y,
        as_rect(frame.camera.background_source),true);

    // Native uses qsort with a non-strict equal-depth comparator. C++
    // std::sort would have undefined ordering requirements with that
    // comparator. Preserve the verified semantics (both gates first,
    // then increasing sprite-bottom Y), retaining input order on ties
    // rather than inventing a zero comparator for retail qsort.
    std::vector<std::size_t> order(scene.entities.size());
    std::iota(order.begin(),order.end(),0);
    std::stable_sort(order.begin(),order.end(),
        [&](std::size_t lhs,std::size_t rhs) {
            const auto& a=scene.entities[lhs];
            const auto& b=scene.entities[rhs];
            const bool ag=herding::is_gate(a.entity_type());
            const bool bg=herding::is_gate(b.entity_type());
            if (ag!=bg) return ag;
            if (ag) return false;
            return herding::entity_depth_y(a) <
                   herding::entity_depth_y(b);
        });

    for (const auto index : order) {
        auto& e=scene.entities[index];
        const auto type=e.entity_type();
        if (type == herding::EntityType::Inactive) continue;

        std::optional<herding::RenderRect> source{};
        herding::Vec2i position{e.x,e.y};
        herding::HerdingSurface surface{};

        if (type == herding::EntityType::FarmerPickles) {
            source=herding::pickles_source_rect(
                e.direction,e.animation_frame);
            surface=herding::HerdingSurface::Pickles;
        } else if (is_animating_animal(type) ||
                   type == herding::EntityType::Scruffty) {
            source=herding::directional_source_rect(
                type,e.direction,e.animation_frame);
            surface=herding::animal_surface(type);
            if (is_animating_animal(type)) {
                const auto next=
                    herding::advance_free_roam_animal_render_animation(
                        type,e.animation_frame,
                        e.animation_frame_countdown,
                        e.movement_speed,e.behavior_state);
                e.animation_frame=next.frame;
                e.animation_frame_countdown=next.countdown;
            }
        } else if (const auto plan=
                       herding::static_entity_render_plan(
                           type,e.animation_frame,position)) {
            position=plan->world_position;
            source=plan->source_rect;
            switch (type) {
            case herding::EntityType::Trailer1:
                surface=herding::HerdingSurface::Trailer1;break;
            case herding::EntityType::Trailer2:
                surface=herding::HerdingSurface::Trailer2;break;
            case herding::EntityType::TravisCab:
                surface=herding::HerdingSurface::TravisCab;break;
            case herding::EntityType::GateLeft:
                surface=herding::HerdingSurface::GateLeft;break;
            case herding::EntityType::GateRight:
                surface=herding::HerdingSurface::GateRight;break;
            default: break;
            }

            if (type==herding::EntityType::TravisCab) {
                const auto next=herding::advance_travis_cab_render_animation(
                    e.animation_frame,e.animation_frame_countdown);
                e.animation_frame=next.frame;
                e.animation_frame_countdown=next.countdown;
            } else if (herding::is_gate(type)) {
                const auto next=herding::advance_gate_render_animation(
                    e.animation_frame,e.animation_frame_countdown);
                e.animation_frame=next.frame;
                e.animation_frame_countdown=next.countdown;
            }
        } else {
            ++frame.rejected_unknown_entities;
            continue;
        }

        if (!source) {
            ++frame.rejected_unknown_entities;
            continue;
        }
        add_asset(frame.draws,surface,
            herding::kWorldViewportDestX+
                position.x-frame.camera.camera_x,
            herding::kWorldViewportDestY+
                position.y-frame.camera.camera_y,
            as_rect(*source),true);
    }

    // The source renderer draws three conditional food-bag overlays AFTER
    // sorting/drawing all entities, before the fixed-screen UI surround.
    constexpr std::array<herding::HerdingSurface,3> bags{{
        herding::HerdingSurface::DuckBag,
        herding::HerdingSurface::RabbitBag,
        herding::HerdingSurface::SheepBag
    }};
    for (std::size_t i=0;i<bags.size();++i) {
        if (!scene.visible_world_bags[i]) continue;
        const auto world=herding::kWorldFoodBagPositions[i];
        add_asset(frame.draws,bags[i],
            herding::kWorldViewportDestX+
                world.x-frame.camera.camera_x,
            herding::kWorldViewportDestY+
                world.y-frame.camera.camera_y,
            std::nullopt,true);
    }

    if (scene.draw_surround) {
        add_asset(frame.draws,herding::HerdingSurface::UiSurround,
            0,0,std::nullopt,false);
    }

    if (scene.selected_food != herding::FoodType::None) {
        add_asset(frame.draws,
            herding::food_toolbar_surface(scene.selected_food),
            herding::kSelectedFoodBagScreenPosition.x,
            herding::kSelectedFoodBagScreenPosition.y,
            std::nullopt,false);
    }

    return frame;
}

} // namespace btb::full_game
