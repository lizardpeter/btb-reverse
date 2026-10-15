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

# Compile native original 0041AEB0 into one reusable library.
add_library(btb_original_activity9_algorithms STATIC
    src/original_activity9.cpp)
target_include_directories(btb_original_activity9_algorithms PUBLIC include)
btb_require_cpp26(btb_original_activity9_algorithms PUBLIC)

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
target_link_libraries(btb_maze PRIVATE btb_original_activity9_algorithms)

add_executable(btb_maze_data_tests
    tests/maze_data_test.cpp
)
target_link_libraries(btb_maze_data_tests PRIVATE btb_maze)
add_test(NAME maze_data COMMAND btb_maze_data_tests)


add_executable(btb_original_live_maze_debounce_tests
    tests/original_live_maze_debounce_test.cpp)
target_link_libraries(btb_original_live_maze_debounce_tests PRIVATE btb_maze)
btb_require_cpp26(btb_original_live_maze_debounce_tests PRIVATE)
add_test(NAME original_live_maze_debounce COMMAND btb_original_live_maze_debounce_tests)

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
    src/profile_name_entry.cpp
    src/original_profile_screen.cpp
)
target_include_directories(btb_player_profiles PUBLIC include)
btb_require_cpp26(btb_player_profiles PUBLIC)

add_executable(btb_player_profiles_tests
    tests/player_profiles_test.cpp
)
target_link_libraries(btb_player_profiles_tests PRIVATE btb_player_profiles)
add_test(NAME player_profiles COMMAND btb_player_profiles_tests)

add_executable(btb_profile_name_entry_tests tests/profile_name_entry_test.cpp)
target_link_libraries(btb_profile_name_entry_tests PRIVATE btb_player_profiles)
add_test(NAME profile_name_entry COMMAND btb_profile_name_entry_tests)


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
    src/full_game_profile_name_entry.cpp
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
# Original Ghidra functions 0040DD50/0040D5B0 and original
# Spud animation/MSVC functions, compiled out of line for direct reuse.
add_library(btb_original_source9_algorithms STATIC
    src/original_source9.cpp)
target_include_directories(btb_original_source9_algorithms PUBLIC include)
btb_require_cpp26(btb_original_source9_algorithms PUBLIC)

add_library(btb_park_designer_scene
    src/park_designer_scene.cpp
)
target_include_directories(btb_park_designer_scene PUBLIC include)
btb_require_cpp26(btb_park_designer_scene PUBLIC)
# Native Park Designer collision checks now execute original 0040DD50
# from a real compiled C++ object, not only an inline source model.
target_link_libraries(btb_park_designer_scene PUBLIC
    btb_original_source9_algorithms)
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
    tests/gameplay_data_batch_20_test.cpp
    src/bobs_band.cpp)
target_include_directories(btb_gameplay_data_batch_20_tests PRIVATE include)
btb_require_cpp26(btb_gameplay_data_batch_20_tests PRIVATE)
target_link_libraries(btb_gameplay_data_batch_20_tests PRIVATE
    btb_fireworks btb_grand_opening btb_front_end_ui_data
    btb_golf btb_spud_skate btb_squirrel btb_maze btb_spud_maze)
add_test(NAME gameplay_data_batch_20 COMMAND btb_gameplay_data_batch_20_tests)


# Original function batch: 10 gameplay/editor and UI/transition functions.
# Retains native COM/audio/movie effects as explicit host operations while
# C++ models execute retail branch and state behavior. The Bob's Band
# composition implementation is directly linked; no fake project stubs.
add_executable(btb_gameplay_control_batch_10_tests
    tests/gameplay_control_batch_10_test.cpp
    src/bobs_band.cpp)
target_include_directories(btb_gameplay_control_batch_10_tests PRIVATE include)
btb_require_cpp26(btb_gameplay_control_batch_10_tests PRIVATE)
add_test(NAME gameplay_control_batch_10 COMMAND btb_gameplay_control_batch_10_tests)


# High-throughput original 32-function MSVC 2003 / Win32 dependency batch.
# Original DOS/PE32 Ghidra machine-code input, typed host-facing C++26
# behaviors and their branch/effect-level unit tests. Native SEH/WinAPI and
# byte-identical retail equivalence remain separate validation work.
add_executable(btb_runtime_dependency_32_tests
    tests/runtime_dependency_32_test.cpp)
target_include_directories(btb_runtime_dependency_32_tests PRIVATE include)
btb_require_cpp26(btb_runtime_dependency_32_tests PRIVATE)
add_test(NAME runtime_dependency_32 COMMAND btb_runtime_dependency_32_tests)


# Game-facing shared UI operations: eight complete Ghidra-source models
# + one explicitly partial SaveAndUnloadPlayerProfiles mapping (not
# promoted to source_written until source stream-mode evidence is resolved).
add_executable(btb_shared_ui_native_9_tests
    tests/shared_ui_native_9_test.cpp)
target_include_directories(btb_shared_ui_native_9_tests PRIVATE include)
btb_require_cpp26(btb_shared_ui_native_9_tests PRIVATE)
add_test(NAME shared_ui_native_9 COMMAND btb_shared_ui_native_9_tests)


# Twelve original function bodies across game activities and their shared
# native platform services. Source-level state/operation validation is NOT
# retail x86 ABI, true DirectDraw/DirectSound/Bink end-to-end equivalence.
add_executable(btb_retail_service_batch_8_tests
    tests/retail_service_batch_8_test.cpp)
target_include_directories(btb_retail_service_batch_8_tests PRIVATE include)
btb_require_cpp26(btb_retail_service_batch_8_tests PRIVATE)
add_test(NAME retail_service_batch_8 COMMAND btb_retail_service_batch_8_tests)

add_executable(btb_native_gameplay_batch_6_tests
    tests/native_gameplay_batch_6_test.cpp
    src/bobs_band.cpp)
target_include_directories(btb_native_gameplay_batch_6_tests PRIVATE include)
btb_require_cpp26(btb_native_gameplay_batch_6_tests PRIVATE)
add_test(NAME native_gameplay_batch_6 COMMAND btb_native_gameplay_batch_6_tests)


# ---------------------------------------------------------------
# UNIFIED RECOMPILATION LINK MILESTONE
# These formerly standalone native game sources are now one library linked
# directly into btb_game. The program exercises the actual GameRoot, four
# activity vtables and the ONE recovered 80-slot native sound manager.
# This does not yet supply the original DirectDraw, DirectSound, Bink runtime,
# remaining activities or verified original executable parity.
# ---------------------------------------------------------------
add_library(btb_recompiled_core STATIC
    src/original_editor3.cpp
    src/original_heap9.cpp
    src/original_game_device2.cpp
    src/original_imports3.cpp
    src/original_display3.cpp
    src/original_cxx12.cpp
    src/original_tail4.cpp
    src/original_closure8.cpp
    src/original_input_crt8.cpp
    src/original_gameplay3.cpp
    src/original_fireworks_start2.cpp
    src/original_maze_init2.cpp
    src/original_profiles_fireworks2.cpp
    src/original_draw4.cpp
    src/original_activity4.cpp
    src/original_unwind13.cpp
    src/original_ftol1.cpp
    src/original_float2.cpp
    src/original_support3.cpp
    src/original_file7.cpp
    src/original_system2.cpp
    src/original_heap4.cpp
    src/original_memory4.cpp
    src/original_environment4.cpp
    src/original_message1.cpp
    src/original_sound2.cpp
    src/original_printer2.cpp
    src/original_wave7.cpp
    src/original_printer7.cpp
    src/original_runtime6.cpp
    src/original_batch20_validation.cpp
    src/original_locale7.cpp
    src/original_locale7_validation.cpp
    src/original_runtime15.cpp
    src/original_runtime15_validation.cpp
    src/original_help2.cpp
    src/full_game_original_help.cpp
    src/original_exception9.cpp
    src/original_startup11.cpp
    src/original_startup11_validation.cpp
    src/original_float6.cpp
    src/original_string8.cpp
    src/original_string8_validation.cpp
    src/original_runtime16.cpp
    src/original_runtime16_validation.cpp
    src/original_crt9.cpp
    src/original_crt9_probe.cpp
    src/original_shared_ui8.cpp
    src/original_shared_ui8_probe.cpp
    src/original_services8.cpp
    src/original_services8_probe.cpp
    src/original_gameplay6.cpp
    src/original_gameplay6_probe.cpp
    src/original_source9_probe.cpp
    src/original_gameplay9.cpp
    src/original_gameplay9_probe.cpp
    src/original_gameplay11.cpp
    src/original_gameplay11_probe.cpp
    src/original_shared11.cpp
    src/original_shared11_probe.cpp
    src/original_lifecycle12.cpp
    src/original_lifecycle12_probe.cpp
    src/original_activity9_probe.cpp
    src/original_system14.cpp
    src/original_system14_probe.cpp
    src/original_media15.cpp
    src/original_media15_probe.cpp
    src/original_crt10.cpp
    src/original_crt10_probe.cpp
    src/original_runtime8.cpp
    src/original_runtime8_probe.cpp
    src/original_active_frame.cpp
    src/original_active_frame_probe.cpp
    src/original_runtime5.cpp
    src/original_runtime5_probe.cpp
    src/original_crt17.cpp
    src/original_crt17_probe.cpp
    src/native_input_pulse.cpp
    src/original_msvc30.cpp
    src/original_msvc30_probe.cpp
    src/original_games_batch_14.cpp
    src/original_crt_batch_32.cpp
    src/original_game_batch_13.cpp
    src/original_leaf_batch_25.cpp
    src/full_game_native_session.cpp
    src/full_game_effect_pipeline.cpp
    src/full_game_resources.cpp
    src/full_game_bobs_band.cpp
    src/full_game_golf.cpp
    src/golf_activity.cpp
    src/full_game_herding.cpp
    src/full_game_herding_event_simulation.cpp
    src/herding_behavior_bridge.cpp
    src/full_game_herding_presentation.cpp
    src/full_game_front_end.cpp
    src/full_game_generic_ui.cpp
    src/full_game_startup_movies.cpp
    src/full_game_walkthrough_catalog.cpp
    src/full_game_ui_bitmaps.cpp
    src/full_game_pregame_resources.cpp
    src/original_asset_resolver.cpp
    src/bobs_band.cpp
    src/bobs_band_activity.cpp
    src/bobs_band_data.cpp
    src/bobs_band_animation.cpp
    src/bobs_band_persistence.cpp
    src/recompiled_core_smoke.cpp
    src/recompiled_gameplay_integration.cpp
)
target_include_directories(btb_recompiled_core PUBLIC include)
btb_require_cpp26(btb_recompiled_core PUBLIC)
target_link_libraries(btb_recompiled_core PUBLIC
    btb_player_profiles
    btb_player_progress
    btb_original_source9_algorithms
    btb_original_activity9_algorithms
    btb_full_game_root_source
    btb_full_game_audio_source
    btb_full_game_fireworks_source
    btb_full_game_park_designer_source
    btb_sound_manager
    btb_dino
    btb_golf
    btb_herding
    btb_maze
    btb_spud_maze
    btb_spud_skate
    btb_squirrel
    btb_fireworks
    btb_grand_opening
    btb_front_end_ui_data
    btb_park_designer
    btb_bitmap_registry
    btb_bink_system
)

# A true executable dependency: not merely separate source-model test bins.
target_link_libraries(btb_game PRIVATE btb_recompiled_core)

add_executable(btb_original_game_batch_13_tests
    tests/original_game_batch_13_test.cpp)
target_link_libraries(btb_original_game_batch_13_tests PRIVATE btb_recompiled_core)
btb_require_cpp26(btb_original_game_batch_13_tests PRIVATE)
add_test(NAME original_game_batch_13 COMMAND btb_original_game_batch_13_tests)

add_executable(btb_original_leaf_batch_25_tests
    tests/original_leaf_batch_25_test.cpp)
target_link_libraries(btb_original_leaf_batch_25_tests PRIVATE btb_recompiled_core)
btb_require_cpp26(btb_original_leaf_batch_25_tests PRIVATE)
add_test(NAME original_leaf_batch_25 COMMAND btb_original_leaf_batch_25_tests)

add_executable(btb_original_games_batch_14_tests
    tests/original_games_batch_14_test.cpp)
target_link_libraries(btb_original_games_batch_14_tests PRIVATE btb_recompiled_core)
btb_require_cpp26(btb_original_games_batch_14_tests PRIVATE)
add_test(NAME original_games_batch_14 COMMAND btb_original_games_batch_14_tests)

add_executable(btb_original_crt_batch_32_tests
    tests/original_crt_batch_32_test.cpp)
target_link_libraries(btb_original_crt_batch_32_tests PRIVATE btb_recompiled_core)
btb_require_cpp26(btb_original_crt_batch_32_tests PRIVATE)
add_test(NAME original_crt_batch_32 COMMAND btb_original_crt_batch_32_tests)

add_executable(btb_original_msvc30_tests
    tests/original_msvc30_test.cpp)
target_link_libraries(btb_original_msvc30_tests PRIVATE btb_recompiled_core)
btb_require_cpp26(btb_original_msvc30_tests PRIVATE)
add_test(NAME original_msvc30 COMMAND btb_original_msvc30_tests)

add_executable(btb_native_input_pulse_tests
    tests/native_input_pulse_test.cpp)
target_link_libraries(btb_native_input_pulse_tests PRIVATE btb_recompiled_core)
btb_require_cpp26(btb_native_input_pulse_tests PRIVATE)
add_test(NAME native_input_pulse COMMAND btb_native_input_pulse_tests)

add_executable(btb_original_crt17_tests
    tests/original_crt17_test.cpp)
target_link_libraries(btb_original_crt17_tests PRIVATE btb_recompiled_core)
btb_require_cpp26(btb_original_crt17_tests PRIVATE)
add_test(NAME original_crt17 COMMAND btb_original_crt17_tests)

add_executable(btb_original_runtime5_tests
    tests/original_runtime5_test.cpp)
target_link_libraries(btb_original_runtime5_tests PRIVATE btb_recompiled_core)
btb_require_cpp26(btb_original_runtime5_tests PRIVATE)
add_test(NAME original_runtime5 COMMAND btb_original_runtime5_tests)

add_executable(btb_original_active_frame_tests
    tests/original_active_frame_test.cpp)
target_link_libraries(btb_original_active_frame_tests PRIVATE btb_recompiled_core)
btb_require_cpp26(btb_original_active_frame_tests PRIVATE)
add_test(NAME original_active_frame COMMAND btb_original_active_frame_tests)

add_executable(btb_original_runtime8_tests
    tests/original_runtime8_test.cpp)
target_link_libraries(btb_original_runtime8_tests PRIVATE btb_recompiled_core)
btb_require_cpp26(btb_original_runtime8_tests PRIVATE)
add_test(NAME original_runtime8 COMMAND btb_original_runtime8_tests)

add_executable(btb_original_crt10_tests
    tests/original_crt10_test.cpp)
target_link_libraries(btb_original_crt10_tests PRIVATE btb_recompiled_core)
btb_require_cpp26(btb_original_crt10_tests PRIVATE)
add_test(NAME original_crt10 COMMAND btb_original_crt10_tests)

# Previously orphaned source-native-session tests are now compiled
# and run on every supported architecture, with checks active in Release.
add_executable(btb_full_game_native_session_source_tests
    tests/full_game_native_session_source_test.cpp)
target_link_libraries(btb_full_game_native_session_source_tests PRIVATE
    btb_recompiled_core)
btb_require_cpp26(btb_full_game_native_session_source_tests PRIVATE)
add_test(NAME full_game_native_session_source
    COMMAND btb_full_game_native_session_source_tests)

add_executable(btb_original_media15_tests
    tests/original_media15_test.cpp)
target_link_libraries(btb_original_media15_tests PRIVATE btb_recompiled_core)
btb_require_cpp26(btb_original_media15_tests PRIVATE)
add_test(NAME original_media15 COMMAND btb_original_media15_tests)

add_executable(btb_original_system14_tests
    tests/original_system14_test.cpp)
target_link_libraries(btb_original_system14_tests PRIVATE btb_recompiled_core)
btb_require_cpp26(btb_original_system14_tests PRIVATE)
add_test(NAME original_system14 COMMAND btb_original_system14_tests)

add_executable(btb_original_activity9_tests
    tests/original_activity9_test.cpp)
target_link_libraries(btb_original_activity9_tests PRIVATE btb_recompiled_core)
btb_require_cpp26(btb_original_activity9_tests PRIVATE)
add_test(NAME original_activity9 COMMAND btb_original_activity9_tests)

add_executable(btb_original_lifecycle12_tests
    tests/original_lifecycle12_test.cpp)
target_link_libraries(btb_original_lifecycle12_tests PRIVATE btb_recompiled_core)
btb_require_cpp26(btb_original_lifecycle12_tests PRIVATE)
add_test(NAME original_lifecycle12 COMMAND btb_original_lifecycle12_tests)

add_executable(btb_original_shared11_tests tests/original_shared11_test.cpp)
target_link_libraries(btb_original_shared11_tests PRIVATE btb_recompiled_core)
btb_require_cpp26(btb_original_shared11_tests PRIVATE)
add_test(NAME original_shared11 COMMAND btb_original_shared11_tests)

add_executable(btb_original_gameplay11_tests
    tests/original_gameplay11_test.cpp)
target_link_libraries(btb_original_gameplay11_tests PRIVATE btb_recompiled_core)
btb_require_cpp26(btb_original_gameplay11_tests PRIVATE)
add_test(NAME original_gameplay11 COMMAND btb_original_gameplay11_tests)

add_executable(btb_original_gameplay9_tests
    tests/original_gameplay9_test.cpp)
target_link_libraries(btb_original_gameplay9_tests PRIVATE btb_recompiled_core)
btb_require_cpp26(btb_original_gameplay9_tests PRIVATE)
add_test(NAME original_gameplay9 COMMAND btb_original_gameplay9_tests)

add_executable(btb_original_source9_tests
    tests/original_source9_test.cpp)
target_link_libraries(btb_original_source9_tests PRIVATE btb_recompiled_core)
btb_require_cpp26(btb_original_source9_tests PRIVATE)
add_test(NAME original_source9 COMMAND btb_original_source9_tests)

add_executable(btb_original_gameplay6_tests
    tests/original_gameplay6_test.cpp)
target_link_libraries(btb_original_gameplay6_tests PRIVATE btb_recompiled_core)
btb_require_cpp26(btb_original_gameplay6_tests PRIVATE)
add_test(NAME original_gameplay6 COMMAND btb_original_gameplay6_tests)

add_executable(btb_original_services8_tests
    tests/original_services8_test.cpp)
target_link_libraries(btb_original_services8_tests PRIVATE btb_recompiled_core)
btb_require_cpp26(btb_original_services8_tests PRIVATE)
add_test(NAME original_services8 COMMAND btb_original_services8_tests)

add_executable(btb_original_shared_ui8_tests
    tests/original_shared_ui8_test.cpp)
target_link_libraries(btb_original_shared_ui8_tests PRIVATE btb_recompiled_core)
btb_require_cpp26(btb_original_shared_ui8_tests PRIVATE)
add_test(NAME original_shared_ui8 COMMAND btb_original_shared_ui8_tests)

add_executable(btb_original_crt9_tests
    tests/original_crt9_test.cpp)
target_link_libraries(btb_original_crt9_tests PRIVATE btb_recompiled_core)
btb_require_cpp26(btb_original_crt9_tests PRIVATE)
add_test(NAME original_crt9 COMMAND btb_original_crt9_tests)

add_executable(btb_recompiled_core_link_tests
    tests/recompiled_core_link_test.cpp)
target_link_libraries(btb_recompiled_core_link_tests PRIVATE btb_recompiled_core)
add_test(NAME recompiled_core_link COMMAND btb_recompiled_core_link_tests)

# This source regression was previously checked in but never registered.
add_executable(btb_bobs_band_source_tests tests/bobs_band_source_test.cpp)
target_link_libraries(btb_bobs_band_source_tests PRIVATE btb_recompiled_core)
add_test(NAME bobs_band_source COMMAND btb_bobs_band_source_tests)

add_executable(btb_original_profile_screen_tests tests/original_profile_screen_test.cpp)
target_link_libraries(btb_original_profile_screen_tests PRIVATE btb_recompiled_core)
add_test(NAME original_profile_screen COMMAND btb_original_profile_screen_tests)

add_executable(btb_original_runtime16_tests tests/original_runtime16_test.cpp)
target_link_libraries(btb_original_runtime16_tests PRIVATE btb_recompiled_core)
add_test(NAME original_runtime16 COMMAND btb_original_runtime16_tests)

add_executable(btb_original_string8_tests tests/original_string8_test.cpp)
target_link_libraries(btb_original_string8_tests PRIVATE btb_recompiled_core)
add_test(NAME original_string8 COMMAND btb_original_string8_tests)

add_executable(btb_retail_pure_runtime_vectors_tests tests/retail_pure_runtime_vectors_test.cpp)
target_link_libraries(btb_retail_pure_runtime_vectors_tests PRIVATE btb_recompiled_core)
add_test(NAME retail_pure_runtime_vectors COMMAND btb_retail_pure_runtime_vectors_tests
    "${CMAKE_CURRENT_SOURCE_DIR}/../analysis/reconstruction/retail-pure-runtime-vectors.txt")

foreach(test_target IN ITEMS btb_profile_name_entry_tests
        btb_full_game_profiles_source_tests btb_bobs_band_source_tests
        btb_original_profile_screen_tests btb_original_runtime16_tests btb_original_string8_tests)
    # These regressions use assert, including state-changing expressions.
    # Keep them active in the Windows Release CI configurations as well.
    if(MSVC)
        target_compile_options(${test_target} PRIVATE /UNDEBUG)
    else()
        target_compile_options(${test_target} PRIVATE -UNDEBUG)
    endif()
endforeach()

# Test actual linked game executable on every supported platform,
# including original x86 Windows (not only a library-linked test program).
add_test(NAME recompiled_game_executable_link
    COMMAND btb_game --recompiled-smoke)

# Compiles previously orphaned original native Windows implementations as part
# of the REAL Windows game executable, not just behavior-plan tests.
if(WIN32)
    add_library(btb_original_win32_platform STATIC
        src/win32_original_imports3.cpp
        src/win32_original_directdraw.cpp
        src/original_recompiled_bitmaps.cpp
        src/win32_recompiled_graphics.cpp
        src/win32_native_bitmap_registry.cpp
        src/win32_original_directsound.cpp
        src/win32_original_bink_exports.cpp
        src/win32_original_game_device.cpp
        src/win32_original_platform_link.cpp
        src/win32_recompiled_host.cpp
        src/win32_original_display_lifecycle.cpp
        src/win32_original_main_window_proc.cpp
    )
    target_include_directories(btb_original_win32_platform PUBLIC include)
    btb_require_cpp26(btb_original_win32_platform PUBLIC)
    target_link_libraries(btb_original_win32_platform PUBLIC
        btb_recompiled_core ddraw dxguid dsound winmm ole32 gdi32 user32)
    target_link_libraries(btb_game PRIVATE btb_original_win32_platform)

    target_link_libraries(btb_game PRIVATE shell32)

    add_executable(btb_original_win32_platform_link_tests
        tests/original_win32_platform_link_test.cpp)
    target_link_libraries(btb_original_win32_platform_link_tests
        PRIVATE btb_original_win32_platform)
    btb_require_cpp26(btb_original_win32_platform_link_tests PRIVATE)
    add_test(NAME original_win32_platform_link
        COMMAND btb_original_win32_platform_link_tests)
endif()

# Fail configuration if any production .cpp is silently left out of the
# build, as happened previously to the Golf and Herding source files.
if(WIN32)
    target_sources(btb_recompiled_core PRIVATE src/win32_original_locale7.cpp)
endif()

file(GLOB BTB_PRODUCTION_CPP CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
get_property(BTB_BUILD_TARGETS DIRECTORY PROPERTY BUILDSYSTEM_TARGETS)
set(BTB_ATTACHED_CPP)
foreach(btb_target IN LISTS BTB_BUILD_TARGETS)
    get_target_property(btb_sources "${btb_target}" SOURCES)
    if(NOT btb_sources)
        continue()
    endif()
    foreach(btb_source IN LISTS btb_sources)
        # Generator expressions denote usage requirements, not .cpp paths.
        if(btb_source MATCHES "[$]<")
            continue()
        endif()
        get_filename_component(btb_absolute "${btb_source}" ABSOLUTE
            BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
        if(btb_absolute MATCHES "/src/[^/]+[.]cpp$")
            list(APPEND BTB_ATTACHED_CPP "${btb_absolute}")
        endif()
    endforeach()
endforeach()
list(REMOVE_DUPLICATES BTB_ATTACHED_CPP)
foreach(btb_source IN LISTS BTB_PRODUCTION_CPP)
    if(NOT WIN32 AND (btb_source MATCHES "/win32_" OR
                      btb_source MATCHES "/original_recompiled_bitmaps[.]cpp$"))
        continue()
    endif()
    if(NOT btb_source IN_LIST BTB_ATTACHED_CPP)
        message(FATAL_ERROR
            "Uncompiled original game source: ${btb_source}. "
            "Add it to a CMake target so the compiler can find errors.")
    endif()
endforeach()

add_executable(btb_original_float6_tests tests/original_float6_test.cpp)
target_link_libraries(btb_original_float6_tests PRIVATE btb_recompiled_core)
add_test(NAME original_float6 COMMAND btb_original_float6_tests ${CMAKE_CURRENT_SOURCE_DIR}/../analysis/reconstruction/retail-float-vectors.txt)

add_executable(btb_original_exception9_tests tests/original_exception9_test.cpp)
target_link_libraries(btb_original_exception9_tests PRIVATE btb_recompiled_core)
add_test(NAME original_exception9 COMMAND btb_original_exception9_tests)
if(MSVC)
    target_compile_options(btb_original_exception9_tests PRIVATE /UNDEBUG)
else()
    target_compile_options(btb_original_exception9_tests PRIVATE -UNDEBUG)
endif()
add_executable(btb_original_startup11_tests tests/original_startup11_test.cpp)
target_link_libraries(btb_original_startup11_tests PRIVATE btb_recompiled_core)
add_test(NAME original_startup11 COMMAND btb_original_startup11_tests ${CMAKE_CURRENT_SOURCE_DIR}/../analysis/reconstruction/retail-startup-vectors.txt)

add_executable(btb_original_help2_tests tests/original_help2_test.cpp)
target_link_libraries(btb_original_help2_tests PRIVATE btb_recompiled_core)
add_test(NAME original_help2 COMMAND btb_original_help2_tests ${CMAKE_CURRENT_SOURCE_DIR}/../analysis/reconstruction/retail-help-vectors.txt)
if(MSVC)
    target_compile_options(btb_original_help2_tests PRIVATE /UNDEBUG)
else()
    target_compile_options(btb_original_help2_tests PRIVATE -UNDEBUG)
endif()

add_executable(btb_original_editor3_tests tests/original_editor3_test.cpp)
target_link_libraries(btb_original_editor3_tests PRIVATE btb_recompiled_core)
add_test(NAME original_editor3 COMMAND btb_original_editor3_tests ${CMAKE_CURRENT_SOURCE_DIR}/../analysis/reconstruction/retail-editor-vectors.txt)

add_executable(btb_original_runtime15_tests tests/original_runtime15_test.cpp)
target_link_libraries(btb_original_runtime15_tests PRIVATE btb_recompiled_core)
add_test(NAME original_runtime15 COMMAND btb_original_runtime15_tests ${CMAKE_CURRENT_SOURCE_DIR}/../analysis/reconstruction/retail-runtime15-vectors.txt)
add_executable(btb_original_locale7_tests tests/original_locale7_test.cpp)
target_link_libraries(btb_original_locale7_tests PRIVATE btb_recompiled_core)
add_test(NAME original_locale7 COMMAND btb_original_locale7_tests)
foreach(target btb_original_runtime15_tests btb_original_locale7_tests)
    if(MSVC)
        target_compile_options(${target} PRIVATE /UNDEBUG)
    else()
        target_compile_options(${target} PRIVATE -UNDEBUG)
    endif()
endforeach()

foreach(group wave7 printer7 runtime6)
    add_executable(btb_original_${group}_tests tests/original_${group}_test.cpp)
    target_link_libraries(btb_original_${group}_tests PRIVATE btb_recompiled_core)
    add_test(NAME original_${group} COMMAND btb_original_${group}_tests ${CMAKE_CURRENT_SOURCE_DIR}/../analysis/reconstruction/retail-batch20-vectors.txt)
    if(MSVC)
        target_compile_options(btb_original_${group}_tests PRIVATE /UNDEBUG)
    else()
        target_compile_options(btb_original_${group}_tests PRIVATE -UNDEBUG)
    endif()
endforeach()

add_executable(btb_original_batch13_tests tests/original_batch13_test.cpp)
target_link_libraries(btb_original_batch13_tests PRIVATE btb_recompiled_core)
add_test(NAME original_batch13 COMMAND btb_original_batch13_tests ${CMAKE_CURRENT_SOURCE_DIR}/../analysis/reconstruction/retail-batch13-vectors.txt)
if(MSVC)
 target_compile_options(btb_original_batch13_tests PRIVATE /UNDEBUG)
else()
 target_compile_options(btb_original_batch13_tests PRIVATE -UNDEBUG)
endif()

add_executable(btb_original_io13_tests tests/original_io13_test.cpp)
target_link_libraries(btb_original_io13_tests PRIVATE btb_recompiled_core)
add_test(NAME original_io13 COMMAND btb_original_io13_tests ${CMAKE_CURRENT_SOURCE_DIR}/../analysis/reconstruction/retail-io13-vectors.txt)
if(MSVC)
 target_compile_options(btb_original_io13_tests PRIVATE /UNDEBUG)
else()
 target_compile_options(btb_original_io13_tests PRIVATE -UNDEBUG)
endif()

add_executable(btb_original_heap14_tests tests/original_heap14_test.cpp)
target_link_libraries(btb_original_heap14_tests PRIVATE btb_recompiled_core)
add_test(NAME original_heap14 COMMAND btb_original_heap14_tests ${CMAKE_CURRENT_SOURCE_DIR}/../analysis/reconstruction/retail-heap14-vectors.txt)
if(MSVC)
 target_compile_options(btb_original_heap14_tests PRIVATE /UNDEBUG)
else()
 target_compile_options(btb_original_heap14_tests PRIVATE -UNDEBUG)
endif()

add_executable(btb_original_unwind15_tests tests/original_unwind15_test.cpp)
target_link_libraries(btb_original_unwind15_tests PRIVATE btb_recompiled_core)
add_test(NAME original_unwind15 COMMAND btb_original_unwind15_tests ${CMAKE_CURRENT_SOURCE_DIR}/../analysis/reconstruction/retail-unwind15-vectors.txt)
if(MSVC)
 target_compile_options(btb_original_unwind15_tests PRIVATE /UNDEBUG)
else()
 target_compile_options(btb_original_unwind15_tests PRIVATE -UNDEBUG)
endif()

add_executable(btb_original_cxx12_tests tests/original_cxx12_test.cpp)
target_link_libraries(btb_original_cxx12_tests PRIVATE btb_recompiled_core)
add_test(NAME original_cxx12 COMMAND btb_original_cxx12_tests ${CMAKE_CURRENT_SOURCE_DIR}/../analysis/reconstruction/retail-cxx12-vectors.txt)
if(MSVC)
 target_compile_options(btb_original_cxx12_tests PRIVATE /UNDEBUG)
else()
 target_compile_options(btb_original_cxx12_tests PRIVATE -UNDEBUG)
endif()

add_executable(btb_original_device8_tests tests/original_device8_test.cpp)
target_link_libraries(btb_original_device8_tests PRIVATE btb_recompiled_core)
add_test(NAME original_device8 COMMAND btb_original_device8_tests ${CMAKE_CURRENT_SOURCE_DIR}/../analysis/reconstruction/retail-device8-vectors.txt)
if(MSVC)
 target_compile_options(btb_original_device8_tests PRIVATE /UNDEBUG)
else()
 target_compile_options(btb_original_device8_tests PRIVATE -UNDEBUG)
endif()

# Four original CRT/time/stack-unwind source-level boundary functions.
add_executable(btb_original_tail4_tests tests/original_tail4_test.cpp)
target_link_libraries(btb_original_tail4_tests PRIVATE btb_recompiled_core)
add_test(NAME original_tail4 COMMAND btb_original_tail4_tests)
if(MSVC)
 target_compile_options(btb_original_tail4_tests PRIVATE /UNDEBUG)
else()
 target_compile_options(btb_original_tail4_tests PRIVATE -UNDEBUG)
endif()

# Eight large original-address activity/UI/resource/print source reconstructions.
add_executable(btb_original_closure8_tests tests/original_closure8_test.cpp)
target_link_libraries(btb_original_closure8_tests PRIVATE btb_recompiled_core)
add_test(NAME original_closure8 COMMAND btb_original_closure8_tests)
if(MSVC)
 target_compile_options(btb_original_closure8_tests PRIVATE /UNDEBUG)
else()
 target_compile_options(btb_original_closure8_tests PRIVATE -UNDEBUG)
endif()

# Eight additional original-address DirectInput and MSVC runtime source bodies.
add_executable(btb_original_input_crt8_tests tests/original_input_crt8_test.cpp)
target_link_libraries(btb_original_input_crt8_tests PRIVATE btb_recompiled_core)
add_test(NAME original_input_crt8 COMMAND btb_original_input_crt8_tests)
if(MSVC)
 target_compile_options(btb_original_input_crt8_tests PRIVATE /UNDEBUG)
else()
 target_compile_options(btb_original_input_crt8_tests PRIVATE -UNDEBUG)
endif()

# Three interconnected source-level retail gameplay state machines.
add_executable(btb_original_gameplay3_tests tests/original_gameplay3_test.cpp)
target_link_libraries(btb_original_gameplay3_tests PRIVATE btb_recompiled_core)
add_test(NAME original_gameplay3 COMMAND btb_original_gameplay3_tests)
if(MSVC)
 target_compile_options(btb_original_gameplay3_tests PRIVATE /UNDEBUG)
else()
 target_compile_options(btb_original_gameplay3_tests PRIVATE -UNDEBUG)
endif()

# Original Fireworks movie bank and editor setup entry points.
add_executable(btb_original_fireworks_start2_tests tests/original_fireworks_start2_test.cpp)
target_link_libraries(btb_original_fireworks_start2_tests PRIVATE btb_recompiled_core)
add_test(NAME original_fireworks_start2 COMMAND btb_original_fireworks_start2_tests)
if(MSVC)
 target_compile_options(btb_original_fireworks_start2_tests PRIVATE /UNDEBUG)
else()
 target_compile_options(btb_original_fireworks_start2_tests PRIVATE -UNDEBUG)
endif()

# Two original Maze initialization bodies; source/asset bindings tested on
# Linux C++26 and native Windows x86/x64.
add_executable(btb_original_maze_init2_tests tests/original_maze_init2_test.cpp)
target_link_libraries(btb_original_maze_init2_tests PRIVATE btb_recompiled_core)
add_test(NAME original_maze_init2 COMMAND btb_original_maze_init2_tests)
if(MSVC)
 target_compile_options(btb_original_maze_init2_tests PRIVATE /UNDEBUG)
else()
 target_compile_options(btb_original_maze_init2_tests PRIVATE -UNDEBUG)
endif()

# Original profile initialization / Fireworks editor drawing recovery.
add_executable(btb_original_profiles_fireworks2_tests tests/original_profiles_fireworks2_test.cpp)
target_link_libraries(btb_original_profiles_fireworks2_tests PRIVATE btb_recompiled_core)
add_test(NAME original_profiles_fireworks2 COMMAND btb_original_profiles_fireworks2_tests)
if(MSVC)
 target_compile_options(btb_original_profiles_fireworks2_tests PRIVATE /UNDEBUG)
else()
 target_compile_options(btb_original_profiles_fireworks2_tests PRIVATE -UNDEBUG)
endif()

# Four original drawing functions with explicit native video/physics boundary.
add_executable(btb_original_draw4_tests tests/original_draw4_test.cpp)
target_link_libraries(btb_original_draw4_tests PRIVATE btb_recompiled_core)
add_test(NAME original_draw4 COMMAND btb_original_draw4_tests)
if(MSVC)
 target_compile_options(btb_original_draw4_tests PRIVATE /UNDEBUG)
else()
 target_compile_options(btb_original_draw4_tests PRIVATE -UNDEBUG)
endif()

# Four source-level original game activity coordination routines.
add_executable(btb_original_activity4_tests tests/original_activity4_test.cpp)
target_link_libraries(btb_original_activity4_tests PRIVATE btb_recompiled_core)
add_test(NAME original_activity4 COMMAND btb_original_activity4_tests)
if(MSVC)
  target_compile_options(btb_original_activity4_tests PRIVATE /UNDEBUG)
else()
  target_compile_options(btb_original_activity4_tests PRIVATE -UNDEBUG)
endif()
