cmake_minimum_required(VERSION 3.25)
project(btb_reconstruction LANGUAGES CXX)

# CMake does not yet expose cxx_std_26 for every MSVC revision,
# even when the compiler accepts preview C++26 features via /std:c++latest.
# Avoid silently compiling under an older MSVC language mode.
function(btb_require_cpp26 target scope)
    if(MSVC)
        target_compile_options(${target} ${scope} /std:c++latest)
    else()
        target_compile_features(${target} ${scope} cxx_std_26)
    endif()
endfunction()


add_library(btb_dino
    src/dino_level.cpp
    src/dino_runtime.cpp
    src/dino_character_animation.cpp
    src/dino_presentation.cpp
)

target_include_directories(btb_dino PUBLIC include)
btb_require_cpp26(btb_dino PUBLIC)

add_executable(btb_dino_tests
    tests/dino_level_test.cpp
)
target_link_libraries(btb_dino_tests PRIVATE btb_dino)

enable_testing()
add_test(NAME dino_level COMMAND btb_dino_tests)


add_executable(btb_dino_runtime_tests
    tests/dino_runtime_test.cpp
)
target_link_libraries(btb_dino_runtime_tests PRIVATE btb_dino)

add_test(NAME dino_runtime COMMAND btb_dino_runtime_tests)

add_executable(btb_dino_presentation_tests
    tests/dino_presentation_test.cpp
)
target_link_libraries(btb_dino_presentation_tests PRIVATE btb_dino)
add_test(NAME dino_presentation COMMAND btb_dino_presentation_tests)

add_library(btb_fireworks
    src/fireworks_layout.cpp
    src/fireworks_sequence.cpp
    src/fireworks_editor.cpp
)
target_include_directories(btb_fireworks PUBLIC include)
btb_require_cpp26(btb_fireworks PUBLIC)

add_executable(btb_fireworks_layout_tests
    tests/fireworks_layout_test.cpp
)
target_link_libraries(btb_fireworks_layout_tests PRIVATE btb_fireworks)
add_test(NAME fireworks_layout COMMAND btb_fireworks_layout_tests)


add_executable(btb_dino_character_animation_tests
    tests/dino_character_animation_test.cpp
)
target_link_libraries(btb_dino_character_animation_tests PRIVATE btb_dino)

add_test(NAME dino_character_animation COMMAND btb_dino_character_animation_tests)

add_executable(btb_fireworks_sequence_tests
    tests/fireworks_sequence_test.cpp
)
target_link_libraries(btb_fireworks_sequence_tests PRIVATE btb_fireworks)
add_test(NAME fireworks_sequence COMMAND btb_fireworks_sequence_tests)

# Original SHA-256-verified Fireworks grid is now the live Sequence storage:
# test native editor decisions, binary firedata roundtrip and playback events.
add_executable(btb_fireworks_retail_integration_tests
    tests/fireworks_retail_integration_test.cpp
)
target_link_libraries(btb_fireworks_retail_integration_tests PRIVATE btb_fireworks)
add_test(NAME fireworks_retail_integration COMMAND btb_fireworks_retail_integration_tests)

add_executable(btb_fireworks_certificate_tests
    tests/fireworks_certificate_test.cpp
)
target_link_libraries(btb_fireworks_certificate_tests PRIVATE btb_fireworks)
add_test(NAME fireworks_certificate COMMAND btb_fireworks_certificate_tests)

add_executable(btb_fireworks_editor_tests
    tests/fireworks_editor_test.cpp
)
target_link_libraries(btb_fireworks_editor_tests PRIVATE btb_fireworks)
add_test(NAME fireworks_editor COMMAND btb_fireworks_editor_tests)

add_library(btb_golf
    src/golf_data.cpp
    src/golf_runtime.cpp
)
target_include_directories(btb_golf PUBLIC include)
btb_require_cpp26(btb_golf PUBLIC)

add_executable(btb_golf_data_tests
    tests/golf_data_test.cpp
)
target_link_libraries(btb_golf_data_tests PRIVATE btb_golf)
add_test(NAME golf_data COMMAND btb_golf_data_tests)

add_executable(btb_golf_runtime_tests
    tests/golf_runtime_test.cpp
)
target_link_libraries(btb_golf_runtime_tests PRIVATE btb_golf)
add_test(NAME golf_runtime COMMAND btb_golf_runtime_tests)

add_library(btb_maze
    src/maze_data.cpp
    src/maze_runtime.cpp
    src/maze_input.cpp
    src/maze_motion.cpp
    src/maze_player_navigation.cpp
    src/maze_transition.cpp
)
target_include_directories(btb_maze PUBLIC include)
btb_require_cpp26(btb_maze PUBLIC)

add_executable(btb_maze_data_tests
    tests/maze_data_test.cpp
)
target_link_libraries(btb_maze_data_tests PRIVATE btb_maze)
add_test(NAME maze_data COMMAND btb_maze_data_tests)


add_executable(btb_maze_runtime_tests
    tests/maze_runtime_test.cpp
)
target_link_libraries(btb_maze_runtime_tests PRIVATE btb_maze)
add_test(NAME maze_runtime COMMAND btb_maze_runtime_tests)

add_executable(btb_maze_presentation_tests
    tests/maze_presentation_test.cpp
)
target_link_libraries(btb_maze_presentation_tests PRIVATE btb_maze)
add_test(NAME maze_presentation COMMAND btb_maze_presentation_tests)

add_executable(btb_maze_controller_tests
    tests/maze_controller_test.cpp
)
target_link_libraries(btb_maze_controller_tests PRIVATE btb_maze)
add_test(NAME maze_controller COMMAND btb_maze_controller_tests)

add_executable(btb_maze_input_tests
    tests/maze_input_test.cpp
)
target_link_libraries(btb_maze_input_tests PRIVATE btb_maze)
add_test(NAME maze_input COMMAND btb_maze_input_tests)

add_executable(btb_maze_motion_tests
    tests/maze_motion_test.cpp
)
target_link_libraries(btb_maze_motion_tests PRIVATE btb_maze)
add_test(NAME maze_motion COMMAND btb_maze_motion_tests)

add_executable(btb_maze_player_navigation_tests
    tests/maze_player_navigation_test.cpp
)
target_link_libraries(btb_maze_player_navigation_tests PRIVATE btb_maze)
add_test(NAME maze_player_navigation COMMAND btb_maze_player_navigation_tests)

add_executable(btb_maze_transition_tests
    tests/maze_transition_test.cpp
)
target_link_libraries(btb_maze_transition_tests PRIVATE btb_maze)
add_test(NAME maze_transition COMMAND btb_maze_transition_tests)

add_executable(btb_maze_spud_controller_tests
    tests/maze_spud_controller_test.cpp
)
target_include_directories(btb_maze_spud_controller_tests PRIVATE include)
btb_require_cpp26(btb_maze_spud_controller_tests PRIVATE)
add_test(NAME maze_spud_controller COMMAND btb_maze_spud_controller_tests)

add_executable(btb_maze_spud_neighbor_tests
    tests/maze_spud_neighbor_test.cpp
)
target_include_directories(btb_maze_spud_neighbor_tests PRIVATE include)
btb_require_cpp26(btb_maze_spud_neighbor_tests PRIVATE)
add_test(NAME maze_spud_neighbor COMMAND btb_maze_spud_neighbor_tests)

add_executable(btb_maze_spud_carry_tests
    tests/maze_spud_carry_test.cpp
)
target_include_directories(btb_maze_spud_carry_tests PRIVATE include)
btb_require_cpp26(btb_maze_spud_carry_tests PRIVATE)
add_test(NAME maze_spud_carry COMMAND btb_maze_spud_carry_tests)

add_executable(btb_maze_spud_reset_tests
    tests/maze_spud_reset_test.cpp
)
target_include_directories(btb_maze_spud_reset_tests PRIVATE include)
btb_require_cpp26(btb_maze_spud_reset_tests PRIVATE)
add_test(NAME maze_spud_reset COMMAND btb_maze_spud_reset_tests)


add_library(btb_herding
    src/herding_data.cpp
    src/herding_runtime.cpp
)
target_include_directories(btb_herding PUBLIC include)
btb_require_cpp26(btb_herding PUBLIC)

add_executable(btb_herding_data_tests
    tests/herding_data_test.cpp
)
target_link_libraries(btb_herding_data_tests PRIVATE btb_herding)
add_test(NAME herding_data COMMAND btb_herding_data_tests)


add_executable(btb_herding_runtime_tests
    tests/herding_runtime_test.cpp
)
target_link_libraries(btb_herding_runtime_tests PRIVATE btb_herding)
add_test(NAME herding_runtime COMMAND btb_herding_runtime_tests)

add_executable(btb_herding_presentation_tests
    tests/herding_presentation_test.cpp
)
target_link_libraries(btb_herding_presentation_tests PRIVATE btb_herding)
add_test(NAME herding_presentation COMMAND btb_herding_presentation_tests)


add_library(btb_park_designer
    src/park_designer_data.cpp
    src/park_designer_runtime.cpp
)
target_include_directories(btb_park_designer PUBLIC include)
btb_require_cpp26(btb_park_designer PUBLIC)

add_executable(btb_park_designer_data_tests
    tests/park_designer_data_test.cpp
)
target_link_libraries(btb_park_designer_data_tests PRIVATE btb_park_designer)
add_test(NAME park_designer_data COMMAND btb_park_designer_data_tests)


add_executable(btb_park_designer_runtime_tests
    tests/park_designer_runtime_test.cpp
)
target_link_libraries(btb_park_designer_runtime_tests PRIVATE btb_park_designer)
add_test(NAME park_designer_runtime COMMAND btb_park_designer_runtime_tests)


add_library(btb_spud_skate
    src/spud_skate_data.cpp
    src/spud_skate_runtime.cpp
)
target_include_directories(btb_spud_skate PUBLIC include)
btb_require_cpp26(btb_spud_skate PUBLIC)

add_executable(btb_spud_skate_data_tests
    tests/spud_skate_data_test.cpp
)
target_link_libraries(btb_spud_skate_data_tests PRIVATE btb_spud_skate)
add_test(NAME spud_skate_data COMMAND btb_spud_skate_data_tests)

add_executable(btb_spud_skate_runtime_tests
    tests/spud_skate_runtime_test.cpp
)
target_link_libraries(btb_spud_skate_runtime_tests PRIVATE btb_spud_skate)
add_test(NAME spud_skate_runtime COMMAND btb_spud_skate_runtime_tests)

add_executable(btb_spud_skate_presentation_tests
    tests/spud_skate_presentation_test.cpp
)
target_link_libraries(btb_spud_skate_presentation_tests PRIVATE btb_spud_skate)
add_test(NAME spud_skate_presentation COMMAND btb_spud_skate_presentation_tests)


add_library(btb_spud_maze
    src/spud_maze_data.cpp
    src/spud_maze_runtime.cpp
)
target_include_directories(btb_spud_maze PUBLIC include)
btb_require_cpp26(btb_spud_maze PUBLIC)

add_executable(btb_spud_maze_data_tests
    tests/spud_maze_data_test.cpp
)
target_link_libraries(btb_spud_maze_data_tests PRIVATE btb_spud_maze)
add_test(NAME spud_maze_data COMMAND btb_spud_maze_data_tests)


add_executable(btb_spud_maze_runtime_tests
    tests/spud_maze_runtime_test.cpp
)
target_link_libraries(btb_spud_maze_runtime_tests PRIVATE btb_spud_maze)
add_test(NAME spud_maze_runtime COMMAND btb_spud_maze_runtime_tests)


add_library(btb_squirrel
    src/squirrel_data.cpp
    src/squirrel_runtime.cpp
    src/squirrel_presentation.cpp
)
target_include_directories(btb_squirrel PUBLIC include)
btb_require_cpp26(btb_squirrel PUBLIC)

add_executable(btb_squirrel_data_tests
    tests/squirrel_data_test.cpp
)
target_link_libraries(btb_squirrel_data_tests PRIVATE btb_squirrel)
add_test(NAME squirrel_data COMMAND btb_squirrel_data_tests)

add_executable(btb_squirrel_runtime_tests
    tests/squirrel_runtime_test.cpp
)
target_link_libraries(btb_squirrel_runtime_tests PRIVATE btb_squirrel)
add_test(NAME squirrel_runtime COMMAND btb_squirrel_runtime_tests)

add_executable(btb_squirrel_presentation_tests
    tests/squirrel_presentation_test.cpp
)
target_link_libraries(btb_squirrel_presentation_tests PRIVATE btb_squirrel)
add_test(NAME squirrel_presentation COMMAND btb_squirrel_presentation_tests)


add_library(btb_grand_opening
    src/grand_opening_data.cpp
    src/grand_opening_sequence.cpp
    src/grand_opening_runtime.cpp
)
target_include_directories(btb_grand_opening PUBLIC include)
btb_require_cpp26(btb_grand_opening PUBLIC)

add_executable(btb_grand_opening_data_tests
    tests/grand_opening_data_test.cpp
)
target_link_libraries(btb_grand_opening_data_tests PRIVATE btb_grand_opening)
add_test(NAME grand_opening_data COMMAND btb_grand_opening_data_tests)

add_executable(btb_grand_opening_sequence_tests
    tests/grand_opening_sequence_test.cpp
)
target_link_libraries(btb_grand_opening_sequence_tests PRIVATE btb_grand_opening)
add_test(NAME grand_opening_sequence COMMAND btb_grand_opening_sequence_tests)

add_executable(btb_grand_opening_runtime_tests
    tests/grand_opening_runtime_test.cpp
)
target_link_libraries(btb_grand_opening_runtime_tests PRIVATE btb_grand_opening)
add_test(NAME grand_opening_runtime COMMAND btb_grand_opening_runtime_tests)


add_library(btb_player_progress
    src/player_progress.cpp
)
target_include_directories(btb_player_progress PUBLIC include)
btb_require_cpp26(btb_player_progress PUBLIC)

add_executable(btb_player_progress_tests
    tests/player_progress_test.cpp
)
target_link_libraries(btb_player_progress_tests PRIVATE btb_player_progress)
add_test(NAME player_progress COMMAND btb_player_progress_tests)


add_library(btb_player_profiles
    src/player_profiles.cpp
)
target_include_directories(btb_player_profiles PUBLIC include)
btb_require_cpp26(btb_player_profiles PUBLIC)

add_executable(btb_player_profiles_tests
    tests/player_profiles_test.cpp
)
target_link_libraries(btb_player_profiles_tests PRIVATE btb_player_profiles)
add_test(NAME player_profiles COMMAND btb_player_profiles_tests)


add_library(btb_sound_manager
    src/sound_manager.cpp
)
target_include_directories(btb_sound_manager PUBLIC include)
btb_require_cpp26(btb_sound_manager PUBLIC)

add_executable(btb_sound_manager_tests
    tests/sound_manager_test.cpp
)
target_link_libraries(btb_sound_manager_tests PRIVATE btb_sound_manager)
add_test(NAME sound_manager COMMAND btb_sound_manager_tests)


add_library(btb_bitmap_registry
    src/bitmap_registry.cpp
)
target_include_directories(btb_bitmap_registry PUBLIC include)
btb_require_cpp26(btb_bitmap_registry PUBLIC)

add_executable(btb_bitmap_registry_tests
    tests/bitmap_registry_test.cpp
)
target_link_libraries(btb_bitmap_registry_tests PRIVATE btb_bitmap_registry)
add_test(NAME bitmap_registry COMMAND btb_bitmap_registry_tests)


add_executable(btb_display_manager_tests
    tests/display_manager_test.cpp
)
target_include_directories(btb_display_manager_tests PRIVATE include)
btb_require_cpp26(btb_display_manager_tests PRIVATE)
add_test(NAME display_manager COMMAND btb_display_manager_tests)


add_executable(btb_dsutil_tests
    tests/dsutil_test.cpp
)
target_include_directories(btb_dsutil_tests PRIVATE include)
btb_require_cpp26(btb_dsutil_tests PRIVATE)
add_test(NAME dsutil_layout COMMAND btb_dsutil_tests)


add_library(btb_printing_system
    src/printing_system.cpp
)
target_include_directories(btb_printing_system PUBLIC include)
btb_require_cpp26(btb_printing_system PUBLIC)

add_executable(btb_printing_system_tests
    tests/printing_system_test.cpp
)
target_link_libraries(btb_printing_system_tests PRIVATE btb_printing_system)
add_test(NAME printing_system COMMAND btb_printing_system_tests)


add_library(btb_bink_system
    src/bink_system.cpp
)
target_include_directories(btb_bink_system PUBLIC include)
btb_require_cpp26(btb_bink_system PUBLIC)

add_executable(btb_bink_system_tests
    tests/bink_system_test.cpp
)
target_link_libraries(btb_bink_system_tests PRIVATE btb_bink_system)
add_test(NAME bink_system COMMAND btb_bink_system_tests)


add_executable(btb_application_state_tests
    tests/application_state_test.cpp
)
target_include_directories(btb_application_state_tests PRIVATE include)
btb_require_cpp26(btb_application_state_tests PRIVATE)
add_test(NAME application_state COMMAND btb_application_state_tests)


add_executable(btb_application_runtime_tests
    tests/application_runtime_test.cpp
)
target_include_directories(btb_application_runtime_tests PRIVATE include)
btb_require_cpp26(btb_application_runtime_tests PRIVATE)
add_test(NAME application_runtime COMMAND btb_application_runtime_tests)


add_executable(btb_input_runtime_tests
    tests/input_runtime_test.cpp
)
target_include_directories(btb_input_runtime_tests PRIVATE include)
btb_require_cpp26(btb_input_runtime_tests PRIVATE)
add_test(NAME input_runtime COMMAND btb_input_runtime_tests)


add_executable(btb_window_runtime_tests
    tests/window_runtime_test.cpp
)
target_include_directories(btb_window_runtime_tests PRIVATE include)
btb_require_cpp26(btb_window_runtime_tests PRIVATE)
add_test(NAME window_runtime COMMAND btb_window_runtime_tests)


add_executable(btb_game_flow_tests
    tests/game_flow_test.cpp
)
target_include_directories(btb_game_flow_tests PRIVATE include)
btb_require_cpp26(btb_game_flow_tests PRIVATE)
add_test(NAME game_flow COMMAND btb_game_flow_tests)


add_executable(btb_front_end_ui_tests
    tests/front_end_ui_test.cpp
)
target_include_directories(btb_front_end_ui_tests PRIVATE include)
btb_require_cpp26(btb_front_end_ui_tests PRIVATE)
add_test(NAME front_end_ui COMMAND btb_front_end_ui_tests)


add_library(btb_front_end_ui_data
    src/front_end_ui_data.cpp
)
target_include_directories(btb_front_end_ui_data PUBLIC include)
btb_require_cpp26(btb_front_end_ui_data PUBLIC)

add_executable(btb_front_end_ui_data_tests
    tests/front_end_ui_data_test.cpp
)
target_link_libraries(btb_front_end_ui_data_tests PRIVATE btb_front_end_ui_data)
add_test(NAME front_end_ui_data COMMAND btb_front_end_ui_data_tests)


add_executable(btb_front_end_ui_runtime_tests
    tests/front_end_ui_runtime_test.cpp
)
target_include_directories(btb_front_end_ui_runtime_tests PRIVATE include)
btb_require_cpp26(btb_front_end_ui_runtime_tests PRIVATE)
add_test(NAME front_end_ui_runtime COMMAND btb_front_end_ui_runtime_tests)

# First integrated native executable. The recoveries above remain independent
# testable modules; this host connects retail game-flow dispatch with one
# playable activity. This is NOT yet a full retail WinMain replacement.
add_library(btb_game_session
    src/game_session.cpp
)
target_include_directories(btb_game_session PUBLIC include)
btb_require_cpp26(btb_game_session PUBLIC)
target_link_libraries(btb_game_session PUBLIC btb_dino)

add_executable(btb_game WIN32
    src/game_main.cpp
    src/game_win32.cpp
)
btb_require_cpp26(btb_game PRIVATE)
target_link_libraries(btb_game PRIVATE btb_game_session)

if(WIN32)
    target_compile_definitions(btb_game PRIVATE UNICODE _UNICODE)
    target_link_libraries(btb_game PRIVATE user32 gdi32 msimg32 comdlg32)
else()
    # Linux smoke mode exercises the same game-session and activity libraries.
    add_test(NAME game_executable_smoke COMMAND btb_game --smoke)
endif()

add_executable(btb_game_session_tests
    tests/game_session_test.cpp
)
btb_require_cpp26(btb_game_session_tests PRIVATE)
target_link_libraries(btb_game_session_tests PRIVATE btb_game_session)
add_test(NAME game_session COMMAND btb_game_session_tests)

# Original-executable identity registry. This target does not link Ghidra pseudocode.
add_executable(btb_original_function_registry_tests
    tests/original_function_registry_test.cpp
)
target_include_directories(btb_original_function_registry_tests PRIVATE include)
btb_require_cpp26(btb_original_function_registry_tests PRIVATE)
add_test(NAME original_function_registry COMMAND btb_original_function_registry_tests)

# Native low-level CSound semantics recovered from 00404430/70/B0.
add_library(btb_dsutil_group_runtime src/dsutil_group_runtime.cpp)
target_include_directories(btb_dsutil_group_runtime PUBLIC include)
btb_require_cpp26(btb_dsutil_group_runtime PUBLIC)
add_executable(btb_dsutil_group_runtime_tests tests/dsutil_group_runtime_test.cpp)
target_link_libraries(btb_dsutil_group_runtime_tests PRIVATE btb_dsutil_group_runtime)
add_test(NAME dsutil_group_runtime COMMAND btb_dsutil_group_runtime_tests)

# Compile and test the previously source-only native audio planner.
# This also validates 0x00402C90 StopAllManagedSounds against recovered
# IsSoundIdPlaying / StopSoundSlot semantics.
add_library(btb_full_game_audio_source src/full_game_audio.cpp)
target_include_directories(btb_full_game_audio_source PUBLIC include)
btb_require_cpp26(btb_full_game_audio_source PUBLIC)
target_link_libraries(btb_full_game_audio_source PUBLIC btb_sound_manager)
add_executable(btb_full_game_audio_source_tests tests/full_game_audio_source_test.cpp)
target_link_libraries(btb_full_game_audio_source_tests PRIVATE btb_full_game_audio_source)
add_test(NAME full_game_audio_source COMMAND btb_full_game_audio_source_tests)


# Compile and test the real source-level 68-state, ten-activity GameRoot and
# original five-profile persistence, not a fabricated game coordinator.
# This target is independent of the Windows-only DirectDraw/DirectSound host.
add_library(btb_full_game_root_source
    src/full_game_runtime.cpp
    src/full_game_profiles.cpp
)
target_include_directories(btb_full_game_root_source PUBLIC include)
btb_require_cpp26(btb_full_game_root_source PUBLIC)
target_link_libraries(btb_full_game_root_source PUBLIC
    btb_player_profiles btb_player_progress
)
add_executable(btb_full_game_runtime_source_tests
    tests/full_game_runtime_source_test.cpp
)
target_link_libraries(btb_full_game_runtime_source_tests PRIVATE
    btb_full_game_root_source
)
add_test(NAME full_game_runtime_source COMMAND btb_full_game_runtime_source_tests)
add_executable(btb_full_game_profiles_source_tests
    tests/full_game_profiles_source_test.cpp
)
target_link_libraries(btb_full_game_profiles_source_tests PRIVATE
    btb_full_game_root_source
)
add_test(NAME full_game_profiles_source COMMAND btb_full_game_profiles_source_tests)

# Original retail PE32 leaf routines: directly translated state math and
# low-level API call contracts; not fabricated game implementations.
add_executable(btb_retail_leaf_runtime_tests
    tests/retail_leaf_runtime_test.cpp
)
target_include_directories(btb_retail_leaf_runtime_tests PRIVATE include)
btb_require_cpp26(btb_retail_leaf_runtime_tests PRIVATE)
target_link_libraries(btb_retail_leaf_runtime_tests PRIVATE btb_printing_system)
add_test(NAME retail_leaf_runtime COMMAND btb_retail_leaf_runtime_tests)

# Shared original runtime, graphics data, audio, UI, activity placement,
# and Bink contracts: compile in Linux full-project CI.
add_executable(btb_retail_shared_runtime_tests
    tests/retail_shared_runtime_test.cpp
)
target_include_directories(btb_retail_shared_runtime_tests PRIVATE include)
btb_require_cpp26(btb_retail_shared_runtime_tests PRIVATE)
add_test(NAME retail_shared_runtime COMMAND btb_retail_shared_runtime_tests)

# Real reconstructed fireworks / DirectDraw and float arithmetic helpers.
# Tests verify source semantics; hardware calls are supplied by callbacks.
add_executable(btb_retail_runtime_closure_tests
    tests/retail_runtime_closure_test.cpp
)
target_include_directories(btb_retail_runtime_closure_tests PRIVATE include)
btb_require_cpp26(btb_retail_runtime_closure_tests PRIVATE)
add_test(NAME retail_runtime_closure COMMAND btb_retail_runtime_closure_tests)

# 35-function high-throughput reconstruction: native device lifetimes,
# DirectDraw/DirectSound, WinMM, Bink, search entry points and game audio.
add_executable(btb_retail_system_closure_tests
    tests/retail_system_closure_test.cpp)
target_include_directories(btb_retail_system_closure_tests PRIVATE include)
btb_require_cpp26(btb_retail_system_closure_tests PRIVATE)
add_test(NAME retail_system_closure COMMAND btb_retail_system_closure_tests)

add_executable(btb_retail_media_game_closure_tests
    tests/retail_media_game_closure_test.cpp)
target_include_directories(btb_retail_media_game_closure_tests PRIVATE include)
btb_require_cpp26(btb_retail_media_game_closure_tests PRIVATE)
add_test(NAME retail_media_game_closure COMMAND btb_retail_media_game_closure_tests)

# Ghidra-sourced activity and low-level WinMM/DirectSound/Bink closures.
add_executable(btb_retail_activity_closure_tests
    tests/retail_activity_closure_test.cpp)
target_include_directories(btb_retail_activity_closure_tests PRIVATE include)
btb_require_cpp26(btb_retail_activity_closure_tests PRIVATE)
add_test(NAME retail_activity_closure COMMAND btb_retail_activity_closure_tests)

# Batch: 39 MSVC exception cleanups, 15 game control routines, and
# 17 native CRT/import functions. Independent C++26 behavioral tests.
add_executable(btb_retail_unwind_entrypoints_tests
    tests/retail_unwind_entrypoints_test.cpp)
target_include_directories(btb_retail_unwind_entrypoints_tests PRIVATE include)
btb_require_cpp26(btb_retail_unwind_entrypoints_tests PRIVATE)
add_test(NAME retail_unwind_entrypoints COMMAND btb_retail_unwind_entrypoints_tests)

add_executable(btb_retail_game_batch70_tests
    tests/retail_game_batch70_test.cpp)
target_include_directories(btb_retail_game_batch70_tests PRIVATE include)
btb_require_cpp26(btb_retail_game_batch70_tests PRIVATE)
add_test(NAME retail_game_batch70 COMMAND btb_retail_game_batch70_tests)

add_executable(btb_retail_crt_batch70_tests
    tests/retail_crt_batch70_test.cpp)
target_include_directories(btb_retail_crt_batch70_tests PRIVATE include)
btb_require_cpp26(btb_retail_crt_batch70_tests PRIVATE)
add_test(NAME retail_crt_batch70 COMMAND btb_retail_crt_batch70_tests)

# Source-evidence architecture registry. This is *metadata* for 200 original
# functions; neither an ABI-faithful compiler output nor a C++ body inventory.
add_executable(btb_original_architecture_map_tests
    tests/original_architecture_map_test.cpp)
target_include_directories(btb_original_architecture_map_tests PRIVATE include)
btb_require_cpp26(btb_original_architecture_map_tests PRIVATE)
add_test(NAME original_architecture_map COMMAND btb_original_architecture_map_tests)

# Entire 615-function Ghidra architecture map, including the final 224 CRT
# functions. Metadata test only; no implied C++ behavioral reconstruction.
add_executable(btb_original_architecture_615_tests
    tests/original_architecture_615_test.cpp)
target_include_directories(btb_original_architecture_615_tests PRIVATE include)
btb_require_cpp26(btb_original_architecture_615_tests PRIVATE)
add_test(NAME original_architecture_615 COMMAND btb_original_architecture_615_tests)


# Fireworks original-file adapter: integrates the 18-cell retail sequence,
# the 33-region editor and binary firedata1..5 save files with GameRoot.
# Bink show and certificate presentation remain explicitly host-owned.
add_library(btb_full_game_fireworks_source
    src/full_game_fireworks.cpp
)
target_include_directories(btb_full_game_fireworks_source PUBLIC include)
btb_require_cpp26(btb_full_game_fireworks_source PUBLIC)
target_link_libraries(btb_full_game_fireworks_source PUBLIC
    btb_fireworks btb_full_game_root_source
)
add_executable(btb_full_game_fireworks_source_tests
    tests/full_game_fireworks_source_test.cpp
)
target_link_libraries(btb_full_game_fireworks_source_tests PRIVATE
    btb_full_game_fireworks_source
)
add_test(NAME full_game_fireworks_source
    COMMAND btb_full_game_fireworks_source_tests)


# Source-backed batch: 9 additional retail PE32 function semantics.
# Park Designer sorting/collision; Spud Maze Bob/Pilchard animation;
# MSVC string/division; little-endian 4/8-byte cursor reads.
add_executable(btb_retail_source_batch_20261011_tests
    tests/retail_source_batch_20261011_test.cpp
)
target_include_directories(btb_retail_source_batch_20261011_tests PRIVATE include)
btb_require_cpp26(btb_retail_source_batch_20261011_tests PRIVATE)
add_test(NAME retail_source_batch_20261011
    COMMAND btb_retail_source_batch_20261011_tests)


# 32 additional source-backed original runtime functions (96-bit arithmetic,
# MSVC CRT, string handling, and native callback wrappers).
add_executable(btb_retail_source_batch_32_tests
    tests/retail_source_batch_32_test.cpp
)
target_include_directories(btb_retail_source_batch_32_tests PRIVATE include)
btb_require_cpp26(btb_retail_source_batch_32_tests PRIVATE)
add_test(NAME retail_source_batch_32
    COMMAND btb_retail_source_batch_32_tests)


# Retail Park Designer scene closure: 8 interconnected original functions.
# Use the exact 400 x 76-byte native save layout and verified polygon parity.
add_library(btb_park_designer_scene
    src/park_designer_scene.cpp
)
target_include_directories(btb_park_designer_scene PUBLIC include)
btb_require_cpp26(btb_park_designer_scene PUBLIC)
add_executable(btb_park_designer_scene_tests
    tests/park_designer_scene_test.cpp
)
target_link_libraries(btb_park_designer_scene_tests PRIVATE
    btb_park_designer_scene
)
add_test(NAME park_designer_scene COMMAND btb_park_designer_scene_tests)


# Original 00410D10 Park Designer outer loop and game dispatcher adapter.
# 0040E440 full editor, DirectDraw bitmap mapping and native music providers
# remain explicit dependencies rather than counterfeit reconstructed hosts.
add_library(btb_park_designer_activity_loop
    src/park_designer_activity_loop.cpp
)
target_include_directories(btb_park_designer_activity_loop PUBLIC include)
btb_require_cpp26(btb_park_designer_activity_loop PUBLIC)
target_link_libraries(btb_park_designer_activity_loop PUBLIC
    btb_park_designer_scene btb_park_designer)

add_executable(btb_park_designer_activity_loop_tests
    tests/park_designer_activity_loop_test.cpp)
target_link_libraries(btb_park_designer_activity_loop_tests PRIVATE
    btb_park_designer_activity_loop)
add_test(NAME park_designer_activity_loop
    COMMAND btb_park_designer_activity_loop_tests)

add_library(btb_full_game_park_designer_source
    src/full_game_park_designer.cpp)
target_include_directories(btb_full_game_park_designer_source PUBLIC include)
btb_require_cpp26(btb_full_game_park_designer_source PUBLIC)
target_link_libraries(btb_full_game_park_designer_source PUBLIC
    btb_full_game_root_source btb_retail_game_lifecycle_15 btb_park_designer_activity_loop)
add_executable(btb_full_game_park_designer_source_tests
    tests/full_game_park_designer_source_test.cpp)
target_link_libraries(btb_full_game_park_designer_source_tests PRIVATE
    btb_full_game_park_designer_source)
add_test(NAME full_game_park_designer_source
    COMMAND btb_full_game_park_designer_source_tests)


# Verified toolbar / View mode slices of central editor 0x0040E440.
# PARTIAL ONLY: does not promote the 10,433-byte whole function to completed.
add_executable(btb_park_designer_editor_control_slice_tests
    tests/park_designer_editor_control_slice_test.cpp)
target_include_directories(btb_park_designer_editor_control_slice_tests PRIVATE include)
btb_require_cpp26(btb_park_designer_editor_control_slice_tests PRIVATE)
add_test(NAME park_designer_editor_control_slice
    COMMAND btb_park_designer_editor_control_slice_tests)


# Throughput batch: 30 authentic MSVC 2003 CRT/function-pointer wrappers,
# stream/character operations, status-table and exception source semantics.
# Source-level models only; native x86 ABI/OS callbacks remain host-owned.
add_executable(btb_retail_crt_dependency_30_tests
    tests/retail_crt_dependency_30_test.cpp)
target_include_directories(btb_retail_crt_dependency_30_tests PRIVATE include)
btb_require_cpp26(btb_retail_crt_dependency_30_tests PRIVATE)
add_test(NAME retail_crt_dependency_30 COMMAND btb_retail_crt_dependency_30_tests)


# Retail 15-function native game lifecycle batch:
# 9 original resource ownership functions; 3 Park Designer save/load/geometry;
# 2 Fireworks editor input entrypoints; 1 Spud Skate state dispatcher.
add_library(btb_retail_game_lifecycle_15
    src/retail_game_lifecycle_15.cpp)
target_include_directories(btb_retail_game_lifecycle_15 PUBLIC include)
btb_require_cpp26(btb_retail_game_lifecycle_15 PUBLIC)
target_link_libraries(btb_retail_game_lifecycle_15 PUBLIC
    btb_park_designer)
add_executable(btb_retail_game_lifecycle_15_tests
    tests/retail_game_lifecycle_15_test.cpp)
target_link_libraries(btb_retail_game_lifecycle_15_tests PRIVATE
    btb_retail_game_lifecycle_15)
add_test(NAME retail_game_lifecycle_15
    COMMAND btb_retail_game_lifecycle_15_tests)


# Batch source-model reconstruction of 0041D4D0/00423F40 activity dispatch,
# 004211A0 Spud Maze directional input and 00423470 collision/drop. Shared
# Maze input, motion, transition, shortest-path and Spud Maze path models are
# explicitly linked and exercised in the combined gameplay regression.
add_executable(btb_maze_spud_frame_batch_tests
    tests/maze_spud_frame_batch_test.cpp)
target_include_directories(btb_maze_spud_frame_batch_tests PRIVATE include)
btb_require_cpp26(btb_maze_spud_frame_batch_tests PRIVATE)
target_link_libraries(btb_maze_spud_frame_batch_tests PRIVATE
    btb_maze btb_spud_maze)
add_test(NAME maze_spud_frame_batch COMMAND btb_maze_spud_frame_batch_tests)


# 20 original-game-function throughput batch. Eleven newly recovered native
# gameplay/data procedures + nine earlier, explicitly validated parsers.
# The original PE32 resource/DirectDraw/Bink/DirectSound effects remain host
# callbacks; passing this test does not imply retail binary/ABI parity.
add_executable(btb_gameplay_data_batch_20_tests
    tests/gameplay_data_batch_20_test.cpp)
target_include_directories(btb_gameplay_data_batch_20_tests PRIVATE include)
btb_require_cpp26(btb_gameplay_data_batch_20_tests PRIVATE)
target_link_libraries(btb_gameplay_data_batch_20_tests PRIVATE
    btb_fireworks btb_grand_opening btb_front_end_ui_data
    btb_golf btb_spud_skate btb_squirrel btb_maze btb_spud_maze)
add_test(NAME gameplay_data_batch_20 COMMAND btb_gameplay_data_batch_20_tests)
