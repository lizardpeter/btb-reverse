get_filename_component(BTB_ROOT "${CMAKE_CURRENT_LIST_DIR}/.." ABSOLUTE)

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

# Portable native Bink1 codec and test suite, kept isolated from current
# production Windows Bink game-host playback to avoid regressions.
add_library(btb_bink32_recovered STATIC
    ${BTB_ROOT}/src/bink32_recovered.cpp
    ${BTB_ROOT}/src/bink32_container.cpp
    ${BTB_ROOT}/src/bink32_playback.cpp
    ${BTB_ROOT}/src/bink32_packet_decode.cpp
    ${BTB_ROOT}/src/bink32_fixed_huffman.cpp
    ${BTB_ROOT}/src/bink32_dynamic_huffman.cpp
    ${BTB_ROOT}/src/bink32_entropy_commands.cpp
    ${BTB_ROOT}/src/bink32_signed_coefficients.cpp
    ${BTB_ROOT}/src/bink32_pixel_reconstruction.cpp
    ${BTB_ROOT}/src/bink32_inverse_transform.cpp
    ${BTB_ROOT}/src/bink32_full_idct.cpp
    ${BTB_ROOT}/src/bink32_original_quant.cpp
    ${BTB_ROOT}/src/bink32_ac_nonzero.cpp
    ${BTB_ROOT}/src/bink32_ac_group4.cpp
    ${BTB_ROOT}/src/bink32_ac_command_pass.cpp
    ${BTB_ROOT}/src/bink32_ac_unit_pass.cpp
    ${BTB_ROOT}/src/bink32_ac_outer_levels.cpp
    ${BTB_ROOT}/src/bink32_ac_block_pixels.cpp
    ${BTB_ROOT}/src/bink32_dc_stream.cpp
    ${BTB_ROOT}/src/bink32_block_modes.cpp
    ${BTB_ROOT}/src/bink32_video_planes.cpp
    ${BTB_ROOT}/src/bink32_simple_picture.cpp
    ${BTB_ROOT}/src/bink32_yuva_frame.cpp
    ${BTB_ROOT}/src/bink32_native_video_stream.cpp
    ${BTB_ROOT}/src/bink32_surface_convert.cpp
    ${BTB_ROOT}/src/bink32_native_movie_player.cpp
    ${BTB_ROOT}/src/bink32_audio_packets.cpp
    ${BTB_ROOT}/src/bink32_audio_spectrum.cpp
    ${BTB_ROOT}/src/bink32_audio_overlap.cpp
    ${BTB_ROOT}/src/bink32_audio_inverse.cpp
    ${BTB_ROOT}/src/bink32_audio_stream.cpp
    ${BTB_ROOT}/src/bink32_audio_pcm.cpp
    ${BTB_ROOT}/src/bink32_audio_movie.cpp
    ${BTB_ROOT}/src/bink32_residue.cpp
    ${BTB_ROOT}/src/bink32_dct_coefficients.cpp
    ${BTB_ROOT}/src/bink32_dct_pixels.cpp
)
target_include_directories(btb_bink32_recovered PUBLIC ${BTB_ROOT}/include)
btb_require_cpp26(btb_bink32_recovered PUBLIC)




add_library(btb_dino
    ${BTB_ROOT}/src/dino_level.cpp
    ${BTB_ROOT}/src/dino_runtime.cpp
    ${BTB_ROOT}/src/dino_character_animation.cpp
    ${BTB_ROOT}/src/dino_presentation.cpp
)

target_include_directories(btb_dino PUBLIC ${BTB_ROOT}/include)
btb_require_cpp26(btb_dino PUBLIC)

add_executable(btb_dino_tests
    ${BTB_ROOT}/tests/dino_level_test.cpp
)
target_link_libraries(btb_dino_tests PRIVATE btb_dino)

enable_testing()
add_test(NAME dino_level COMMAND btb_dino_tests)


add_executable(btb_dino_runtime_tests
    ${BTB_ROOT}/tests/dino_runtime_test.cpp
)
target_link_libraries(btb_dino_runtime_tests PRIVATE btb_dino)

add_test(NAME dino_runtime COMMAND btb_dino_runtime_tests)

add_executable(btb_dino_presentation_tests
    ${BTB_ROOT}/tests/dino_presentation_test.cpp
)
target_link_libraries(btb_dino_presentation_tests PRIVATE btb_dino)
add_test(NAME dino_presentation COMMAND btb_dino_presentation_tests)

add_library(btb_fireworks
    ${BTB_ROOT}/src/fireworks_layout.cpp
    ${BTB_ROOT}/src/fireworks_sequence.cpp
    ${BTB_ROOT}/src/fireworks_editor.cpp
)
target_include_directories(btb_fireworks PUBLIC ${BTB_ROOT}/include)
btb_require_cpp26(btb_fireworks PUBLIC)

add_executable(btb_fireworks_layout_tests
    ${BTB_ROOT}/tests/fireworks_layout_test.cpp
)
target_link_libraries(btb_fireworks_layout_tests PRIVATE btb_fireworks)
add_test(NAME fireworks_layout COMMAND btb_fireworks_layout_tests)


add_executable(btb_dino_character_animation_tests
    ${BTB_ROOT}/tests/dino_character_animation_test.cpp
)
target_link_libraries(btb_dino_character_animation_tests PRIVATE btb_dino)

add_test(NAME dino_character_animation COMMAND btb_dino_character_animation_tests)

add_executable(btb_fireworks_sequence_tests
    ${BTB_ROOT}/tests/fireworks_sequence_test.cpp
)
target_link_libraries(btb_fireworks_sequence_tests PRIVATE btb_fireworks)
add_test(NAME fireworks_sequence COMMAND btb_fireworks_sequence_tests)

# Original SHA-256-verified Fireworks grid is now the live Sequence storage:
# test native editor decisions, binary firedata roundtrip and playback events.
add_executable(btb_fireworks_retail_integration_tests
    ${BTB_ROOT}/tests/fireworks_retail_integration_test.cpp
)
target_link_libraries(btb_fireworks_retail_integration_tests PRIVATE btb_fireworks)
add_test(NAME fireworks_retail_integration COMMAND btb_fireworks_retail_integration_tests)

add_executable(btb_fireworks_certificate_tests
    ${BTB_ROOT}/tests/fireworks_certificate_test.cpp
)
target_link_libraries(btb_fireworks_certificate_tests PRIVATE btb_fireworks)
btb_require_cpp26(btb_fireworks_certificate_tests PRIVATE)
if(MSVC)
    target_compile_options(btb_fireworks_certificate_tests PRIVATE /UNDEBUG)
else()
    target_compile_options(btb_fireworks_certificate_tests PRIVATE -UNDEBUG)
endif()
add_test(NAME fireworks_certificate COMMAND btb_fireworks_certificate_tests)

add_executable(btb_fireworks_editor_tests
    ${BTB_ROOT}/tests/fireworks_editor_test.cpp
)
target_link_libraries(btb_fireworks_editor_tests PRIVATE btb_fireworks)
add_test(NAME fireworks_editor COMMAND btb_fireworks_editor_tests)

add_library(btb_golf
    ${BTB_ROOT}/src/golf_data.cpp
    ${BTB_ROOT}/src/golf_runtime.cpp
)
target_include_directories(btb_golf PUBLIC ${BTB_ROOT}/include)
btb_require_cpp26(btb_golf PUBLIC)

add_executable(btb_golf_data_tests
    ${BTB_ROOT}/tests/golf_data_test.cpp
)
target_link_libraries(btb_golf_data_tests PRIVATE btb_golf)
add_test(NAME golf_data COMMAND btb_golf_data_tests)

add_executable(btb_golf_runtime_tests
    ${BTB_ROOT}/tests/golf_runtime_test.cpp
)
target_link_libraries(btb_golf_runtime_tests PRIVATE btb_golf)
add_test(NAME golf_runtime COMMAND btb_golf_runtime_tests)

# Compile native original 0041AEB0 into one reusable library.
add_library(btb_original_activity9_algorithms STATIC
    ${BTB_ROOT}/src/original_activity9.cpp)
target_include_directories(btb_original_activity9_algorithms PUBLIC ${BTB_ROOT}/include)
btb_require_cpp26(btb_original_activity9_algorithms PUBLIC)

add_library(btb_maze
    ${BTB_ROOT}/src/maze_data.cpp
    ${BTB_ROOT}/src/maze_runtime.cpp
    ${BTB_ROOT}/src/maze_input.cpp
    ${BTB_ROOT}/src/maze_motion.cpp
    ${BTB_ROOT}/src/maze_player_navigation.cpp
    ${BTB_ROOT}/src/maze_transition.cpp
)
target_include_directories(btb_maze PUBLIC ${BTB_ROOT}/include)
btb_require_cpp26(btb_maze PUBLIC)
target_link_libraries(btb_maze PRIVATE btb_original_activity9_algorithms)

add_executable(btb_maze_data_tests
    ${BTB_ROOT}/tests/maze_data_test.cpp
)
target_link_libraries(btb_maze_data_tests PRIVATE btb_maze)
add_test(NAME maze_data COMMAND btb_maze_data_tests)


add_executable(btb_original_live_maze_debounce_tests
    ${BTB_ROOT}/tests/original_live_maze_debounce_test.cpp)
target_link_libraries(btb_original_live_maze_debounce_tests PRIVATE btb_maze)
btb_require_cpp26(btb_original_live_maze_debounce_tests PRIVATE)
add_test(NAME original_live_maze_debounce COMMAND btb_original_live_maze_debounce_tests)

add_executable(btb_maze_runtime_tests
    ${BTB_ROOT}/tests/maze_runtime_test.cpp
)
target_link_libraries(btb_maze_runtime_tests PRIVATE btb_maze)
add_test(NAME maze_runtime COMMAND btb_maze_runtime_tests)

add_executable(btb_maze_presentation_tests
    ${BTB_ROOT}/tests/maze_presentation_test.cpp
)
target_link_libraries(btb_maze_presentation_tests PRIVATE btb_maze)
add_test(NAME maze_presentation COMMAND btb_maze_presentation_tests)

add_executable(btb_maze_controller_tests
    ${BTB_ROOT}/tests/maze_controller_test.cpp
)
target_link_libraries(btb_maze_controller_tests PRIVATE btb_maze)
add_test(NAME maze_controller COMMAND btb_maze_controller_tests)

add_executable(btb_maze_input_tests
    ${BTB_ROOT}/tests/maze_input_test.cpp
)
target_link_libraries(btb_maze_input_tests PRIVATE btb_maze)
add_test(NAME maze_input COMMAND btb_maze_input_tests)

add_executable(btb_maze_motion_tests
    ${BTB_ROOT}/tests/maze_motion_test.cpp
)
target_link_libraries(btb_maze_motion_tests PRIVATE btb_maze)
add_test(NAME maze_motion COMMAND btb_maze_motion_tests)

add_executable(btb_maze_player_navigation_tests
    ${BTB_ROOT}/tests/maze_player_navigation_test.cpp
)
target_link_libraries(btb_maze_player_navigation_tests PRIVATE btb_maze)
add_test(NAME maze_player_navigation COMMAND btb_maze_player_navigation_tests)

add_executable(btb_maze_transition_tests
    ${BTB_ROOT}/tests/maze_transition_test.cpp
)
target_link_libraries(btb_maze_transition_tests PRIVATE btb_maze)
add_test(NAME maze_transition COMMAND btb_maze_transition_tests)

add_executable(btb_maze_spud_controller_tests
    ${BTB_ROOT}/tests/maze_spud_controller_test.cpp
)
target_include_directories(btb_maze_spud_controller_tests PRIVATE ${BTB_ROOT}/include)
btb_require_cpp26(btb_maze_spud_controller_tests PRIVATE)
add_test(NAME maze_spud_controller COMMAND btb_maze_spud_controller_tests)

add_executable(btb_maze_spud_neighbor_tests
    ${BTB_ROOT}/tests/maze_spud_neighbor_test.cpp
)
target_include_directories(btb_maze_spud_neighbor_tests PRIVATE ${BTB_ROOT}/include)
btb_require_cpp26(btb_maze_spud_neighbor_tests PRIVATE)
add_test(NAME maze_spud_neighbor COMMAND btb_maze_spud_neighbor_tests)

add_executable(btb_maze_spud_carry_tests
    ${BTB_ROOT}/tests/maze_spud_carry_test.cpp
)
target_include_directories(btb_maze_spud_carry_tests PRIVATE ${BTB_ROOT}/include)
btb_require_cpp26(btb_maze_spud_carry_tests PRIVATE)
add_test(NAME maze_spud_carry COMMAND btb_maze_spud_carry_tests)

add_executable(btb_maze_spud_reset_tests
    ${BTB_ROOT}/tests/maze_spud_reset_test.cpp
)
target_include_directories(btb_maze_spud_reset_tests PRIVATE ${BTB_ROOT}/include)
btb_require_cpp26(btb_maze_spud_reset_tests PRIVATE)
add_test(NAME maze_spud_reset COMMAND btb_maze_spud_reset_tests)


add_library(btb_herding
    ${BTB_ROOT}/src/herding_data.cpp
    ${BTB_ROOT}/src/herding_runtime.cpp
)
target_include_directories(btb_herding PUBLIC ${BTB_ROOT}/include)
btb_require_cpp26(btb_herding PUBLIC)

add_executable(btb_herding_data_tests
    ${BTB_ROOT}/tests/herding_data_test.cpp
)
target_link_libraries(btb_herding_data_tests PRIVATE btb_herding)
add_test(NAME herding_data COMMAND btb_herding_data_tests)


add_executable(btb_herding_runtime_tests
    ${BTB_ROOT}/tests/herding_runtime_test.cpp
)
target_link_libraries(btb_herding_runtime_tests PRIVATE btb_herding)
add_test(NAME herding_runtime COMMAND btb_herding_runtime_tests)

add_executable(btb_herding_presentation_tests
    ${BTB_ROOT}/tests/herding_presentation_test.cpp
)
target_link_libraries(btb_herding_presentation_tests PRIVATE btb_herding)
add_test(NAME herding_presentation COMMAND btb_herding_presentation_tests)


# Compile and exercise all isolated Pets Corner source kernels, which
# previously existed on disk without being registered in CTest.
set(BTB_HERDING_PURE_TARGETS "")
foreach(test_name IN ITEMS
    herding_navigation_boundary_source
    herding_retail_rect_contact_source
    herding_source_animal_chatter_source
    herding_source_pair_separation_source
    herding_source_pickles_controls_source
    herding_source_follower_approach_source
    herding_source_heading_source
    herding_source_home_arrival_source
    herding_source_partial_dispatcher_source
    herding_source_roam_braking_source
    herding_source_roaming_loop_source
    herding_source_roaming_recovery_source
    herding_source_scruffty_patrol_source
    herding_source_steering_source
    herding_source_temporary_target_source
    herding_source_tracked_targets_source
    herding_source_wrong_home_source
)
    set(target_name btb_${test_name}_tests)
    add_executable(${target_name} ${BTB_ROOT}/tests/${test_name}_test.cpp)
    target_link_libraries(${target_name} PRIVATE btb_herding)
    btb_require_cpp26(${target_name} PRIVATE)
    if(MSVC)
        target_compile_options(${target_name} PRIVATE /UNDEBUG)
    else()
        target_compile_options(${target_name} PRIVATE -UNDEBUG)
    endif()
    add_test(NAME ${test_name} COMMAND ${target_name})
    list(APPEND BTB_HERDING_PURE_TARGETS ${target_name})
endforeach()
add_custom_target(btb_herding_pure_source_regressions DEPENDS ${BTB_HERDING_PURE_TARGETS})

add_library(btb_park_designer
    ${BTB_ROOT}/src/park_designer_data.cpp
    ${BTB_ROOT}/src/park_designer_runtime.cpp
)
target_include_directories(btb_park_designer PUBLIC ${BTB_ROOT}/include)
btb_require_cpp26(btb_park_designer PUBLIC)

add_executable(btb_park_designer_data_tests
    ${BTB_ROOT}/tests/park_designer_data_test.cpp
)
target_link_libraries(btb_park_designer_data_tests PRIVATE btb_park_designer)
add_test(NAME park_designer_data COMMAND btb_park_designer_data_tests)


add_executable(btb_park_designer_runtime_tests
    ${BTB_ROOT}/tests/park_designer_runtime_test.cpp
)
target_link_libraries(btb_park_designer_runtime_tests PRIVATE btb_park_designer)
add_test(NAME park_designer_runtime COMMAND btb_park_designer_runtime_tests)


add_library(btb_spud_skate
    ${BTB_ROOT}/src/spud_skate_data.cpp
    ${BTB_ROOT}/src/spud_skate_runtime.cpp
)
target_include_directories(btb_spud_skate PUBLIC ${BTB_ROOT}/include)
btb_require_cpp26(btb_spud_skate PUBLIC)

add_executable(btb_spud_skate_data_tests
    ${BTB_ROOT}/tests/spud_skate_data_test.cpp
)
target_link_libraries(btb_spud_skate_data_tests PRIVATE btb_spud_skate)
add_test(NAME spud_skate_data COMMAND btb_spud_skate_data_tests)

add_executable(btb_spud_skate_runtime_tests
    ${BTB_ROOT}/tests/spud_skate_runtime_test.cpp
)
target_link_libraries(btb_spud_skate_runtime_tests PRIVATE btb_spud_skate)
add_test(NAME spud_skate_runtime COMMAND btb_spud_skate_runtime_tests)

add_executable(btb_spud_skate_presentation_tests
    ${BTB_ROOT}/tests/spud_skate_presentation_test.cpp
)
target_link_libraries(btb_spud_skate_presentation_tests PRIVATE btb_spud_skate)
add_test(NAME spud_skate_presentation COMMAND btb_spud_skate_presentation_tests)


add_library(btb_spud_maze
    ${BTB_ROOT}/src/spud_maze_data.cpp
    ${BTB_ROOT}/src/spud_maze_runtime.cpp
)
target_include_directories(btb_spud_maze PUBLIC ${BTB_ROOT}/include)
btb_require_cpp26(btb_spud_maze PUBLIC)

add_executable(btb_spud_maze_data_tests
    ${BTB_ROOT}/tests/spud_maze_data_test.cpp
)
target_link_libraries(btb_spud_maze_data_tests PRIVATE btb_spud_maze)
add_test(NAME spud_maze_data COMMAND btb_spud_maze_data_tests)


add_executable(btb_spud_maze_runtime_tests
    ${BTB_ROOT}/tests/spud_maze_runtime_test.cpp
)
target_link_libraries(btb_spud_maze_runtime_tests PRIVATE btb_spud_maze)
add_test(NAME spud_maze_runtime COMMAND btb_spud_maze_runtime_tests)


add_library(btb_squirrel
    ${BTB_ROOT}/src/squirrel_data.cpp
    ${BTB_ROOT}/src/squirrel_runtime.cpp
    ${BTB_ROOT}/src/squirrel_presentation.cpp
)
target_include_directories(btb_squirrel PUBLIC ${BTB_ROOT}/include)
btb_require_cpp26(btb_squirrel PUBLIC)

add_executable(btb_squirrel_data_tests
    ${BTB_ROOT}/tests/squirrel_data_test.cpp
)
target_link_libraries(btb_squirrel_data_tests PRIVATE btb_squirrel)
add_test(NAME squirrel_data COMMAND btb_squirrel_data_tests)

add_executable(btb_squirrel_runtime_tests
    ${BTB_ROOT}/tests/squirrel_runtime_test.cpp
)
target_link_libraries(btb_squirrel_runtime_tests PRIVATE btb_squirrel)
add_test(NAME squirrel_runtime COMMAND btb_squirrel_runtime_tests)
# Runtime checks enumerate retail branch tables and must not be stripped in Release.
if(MSVC)
    target_compile_options(btb_squirrel_runtime_tests PRIVATE /UNDEBUG)
else()
    target_compile_options(btb_squirrel_runtime_tests PRIVATE -UNDEBUG)
endif()

add_executable(btb_squirrel_presentation_tests
    ${BTB_ROOT}/tests/squirrel_presentation_test.cpp
)
target_link_libraries(btb_squirrel_presentation_tests PRIVATE btb_squirrel)
add_test(NAME squirrel_presentation COMMAND btb_squirrel_presentation_tests)
if(MSVC)
    target_compile_options(btb_squirrel_presentation_tests PRIVATE /UNDEBUG)
else()
    target_compile_options(btb_squirrel_presentation_tests PRIVATE -UNDEBUG)
endif()


add_library(btb_grand_opening
    ${BTB_ROOT}/src/grand_opening_data.cpp
    ${BTB_ROOT}/src/grand_opening_sequence.cpp
    ${BTB_ROOT}/src/grand_opening_runtime.cpp
)
target_include_directories(btb_grand_opening PUBLIC ${BTB_ROOT}/include)
btb_require_cpp26(btb_grand_opening PUBLIC)

add_executable(btb_grand_opening_data_tests
    ${BTB_ROOT}/tests/grand_opening_data_test.cpp
)
target_link_libraries(btb_grand_opening_data_tests PRIVATE btb_grand_opening)
add_test(NAME grand_opening_data COMMAND btb_grand_opening_data_tests)

add_executable(btb_grand_opening_sequence_tests
    ${BTB_ROOT}/tests/grand_opening_sequence_test.cpp
)
target_link_libraries(btb_grand_opening_sequence_tests PRIVATE btb_grand_opening)
add_test(NAME grand_opening_sequence COMMAND btb_grand_opening_sequence_tests)

add_executable(btb_grand_opening_runtime_tests
    ${BTB_ROOT}/tests/grand_opening_runtime_test.cpp
)
target_link_libraries(btb_grand_opening_runtime_tests PRIVATE btb_grand_opening)
add_test(NAME grand_opening_runtime COMMAND btb_grand_opening_runtime_tests)


add_library(btb_player_progress
    ${BTB_ROOT}/src/player_progress.cpp
)
target_include_directories(btb_player_progress PUBLIC ${BTB_ROOT}/include)
btb_require_cpp26(btb_player_progress PUBLIC)

add_executable(btb_player_progress_tests
    ${BTB_ROOT}/tests/player_progress_test.cpp
)
target_link_libraries(btb_player_progress_tests PRIVATE btb_player_progress)
add_test(NAME player_progress COMMAND btb_player_progress_tests)


add_library(btb_player_profiles
    ${BTB_ROOT}/src/player_profiles.cpp
    ${BTB_ROOT}/src/profile_name_entry.cpp
    ${BTB_ROOT}/src/original_profile_screen.cpp
)
target_include_directories(btb_player_profiles PUBLIC ${BTB_ROOT}/include)
btb_require_cpp26(btb_player_profiles PUBLIC)

add_executable(btb_player_profiles_tests
    ${BTB_ROOT}/tests/player_profiles_test.cpp
)
target_link_libraries(btb_player_profiles_tests PRIVATE btb_player_profiles)
add_test(NAME player_profiles COMMAND btb_player_profiles_tests)

add_executable(btb_profile_name_entry_tests ${BTB_ROOT}/tests/profile_name_entry_test.cpp)
target_link_libraries(btb_profile_name_entry_tests PRIVATE btb_player_profiles)
add_test(NAME profile_name_entry COMMAND btb_profile_name_entry_tests)


add_library(btb_sound_manager
    ${BTB_ROOT}/src/sound_manager.cpp
)
target_include_directories(btb_sound_manager PUBLIC ${BTB_ROOT}/include)
btb_require_cpp26(btb_sound_manager PUBLIC)

add_executable(btb_sound_manager_tests
    ${BTB_ROOT}/tests/sound_manager_test.cpp
)
target_link_libraries(btb_sound_manager_tests PRIVATE btb_sound_manager)
add_test(NAME sound_manager COMMAND btb_sound_manager_tests)


add_library(btb_bitmap_registry
    ${BTB_ROOT}/src/bitmap_registry.cpp
)
target_include_directories(btb_bitmap_registry PUBLIC ${BTB_ROOT}/include)
btb_require_cpp26(btb_bitmap_registry PUBLIC)

add_executable(btb_bitmap_registry_tests
    ${BTB_ROOT}/tests/bitmap_registry_test.cpp
)
target_link_libraries(btb_bitmap_registry_tests PRIVATE btb_bitmap_registry)
add_test(NAME bitmap_registry COMMAND btb_bitmap_registry_tests)


add_executable(btb_display_manager_tests
    ${BTB_ROOT}/tests/display_manager_test.cpp
)
target_include_directories(btb_display_manager_tests PRIVATE ${BTB_ROOT}/include)
btb_require_cpp26(btb_display_manager_tests PRIVATE)
add_test(NAME display_manager COMMAND btb_display_manager_tests)


add_executable(btb_dsutil_tests
    ${BTB_ROOT}/tests/dsutil_test.cpp
)
target_include_directories(btb_dsutil_tests PRIVATE ${BTB_ROOT}/include)
btb_require_cpp26(btb_dsutil_tests PRIVATE)
add_test(NAME dsutil_layout COMMAND btb_dsutil_tests)


add_library(btb_printing_system
    ${BTB_ROOT}/src/printing_system.cpp
)
target_include_directories(btb_printing_system PUBLIC ${BTB_ROOT}/include)
btb_require_cpp26(btb_printing_system PUBLIC)

add_executable(btb_printing_system_tests
    ${BTB_ROOT}/tests/printing_system_test.cpp
)
target_link_libraries(btb_printing_system_tests PRIVATE btb_printing_system)
add_test(NAME printing_system COMMAND btb_printing_system_tests)


add_library(btb_bink_system
    ${BTB_ROOT}/src/bink_system.cpp
)
target_include_directories(btb_bink_system PUBLIC ${BTB_ROOT}/include)
btb_require_cpp26(btb_bink_system PUBLIC)

add_executable(btb_bink_system_tests
    ${BTB_ROOT}/tests/bink_system_test.cpp
)
target_link_libraries(btb_bink_system_tests PRIVATE btb_bink_system)
add_test(NAME bink_system COMMAND btb_bink_system_tests)


add_executable(btb_application_state_tests
    ${BTB_ROOT}/tests/application_state_test.cpp
)
target_include_directories(btb_application_state_tests PRIVATE ${BTB_ROOT}/include)
btb_require_cpp26(btb_application_state_tests PRIVATE)
add_test(NAME application_state COMMAND btb_application_state_tests)


add_executable(btb_application_runtime_tests
    ${BTB_ROOT}/tests/application_runtime_test.cpp
)
target_include_directories(btb_application_runtime_tests PRIVATE ${BTB_ROOT}/include)
btb_require_cpp26(btb_application_runtime_tests PRIVATE)
add_test(NAME application_runtime COMMAND btb_application_runtime_tests)


add_executable(btb_input_runtime_tests
    ${BTB_ROOT}/tests/input_runtime_test.cpp
)
target_include_directories(btb_input_runtime_tests PRIVATE ${BTB_ROOT}/include)
btb_require_cpp26(btb_input_runtime_tests PRIVATE)
add_test(NAME input_runtime COMMAND btb_input_runtime_tests)


add_executable(btb_window_runtime_tests
    ${BTB_ROOT}/tests/window_runtime_test.cpp
)
target_include_directories(btb_window_runtime_tests PRIVATE ${BTB_ROOT}/include)
btb_require_cpp26(btb_window_runtime_tests PRIVATE)
add_test(NAME window_runtime COMMAND btb_window_runtime_tests)


add_executable(btb_game_flow_tests
    ${BTB_ROOT}/tests/game_flow_test.cpp
)
target_include_directories(btb_game_flow_tests PRIVATE ${BTB_ROOT}/include)
btb_require_cpp26(btb_game_flow_tests PRIVATE)
add_test(NAME game_flow COMMAND btb_game_flow_tests)


add_executable(btb_front_end_ui_tests
    ${BTB_ROOT}/tests/front_end_ui_test.cpp
)
target_include_directories(btb_front_end_ui_tests PRIVATE ${BTB_ROOT}/include)
btb_require_cpp26(btb_front_end_ui_tests PRIVATE)
add_test(NAME front_end_ui COMMAND btb_front_end_ui_tests)


add_library(btb_front_end_ui_data
    ${BTB_ROOT}/src/front_end_ui_data.cpp
)
target_include_directories(btb_front_end_ui_data PUBLIC ${BTB_ROOT}/include)
btb_require_cpp26(btb_front_end_ui_data PUBLIC)

add_executable(btb_front_end_ui_data_tests
    ${BTB_ROOT}/tests/front_end_ui_data_test.cpp
)
target_link_libraries(btb_front_end_ui_data_tests PRIVATE btb_front_end_ui_data)
add_test(NAME front_end_ui_data COMMAND btb_front_end_ui_data_tests)


add_executable(btb_front_end_ui_runtime_tests
    ${BTB_ROOT}/tests/front_end_ui_runtime_test.cpp
)
target_include_directories(btb_front_end_ui_runtime_tests PRIVATE ${BTB_ROOT}/include)
btb_require_cpp26(btb_front_end_ui_runtime_tests PRIVATE)
add_test(NAME front_end_ui_runtime COMMAND btb_front_end_ui_runtime_tests)

# First integrated native executable. The recoveries above remain independent
# testable modules; this host connects retail game-flow dispatch with one
# playable activity. This is NOT yet a full retail WinMain replacement.
add_library(btb_game_session
    ${BTB_ROOT}/src/game_session.cpp
)
target_include_directories(btb_game_session PUBLIC ${BTB_ROOT}/include)
btb_require_cpp26(btb_game_session PUBLIC)
target_link_libraries(btb_game_session PUBLIC btb_dino)

add_executable(btb_game WIN32
    ${BTB_ROOT}/runtime/game_main.cpp
    ${BTB_ROOT}/runtime/game_win32.cpp
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
    ${BTB_ROOT}/tests/game_session_test.cpp
)
btb_require_cpp26(btb_game_session_tests PRIVATE)
target_link_libraries(btb_game_session_tests PRIVATE btb_game_session)
add_test(NAME game_session COMMAND btb_game_session_tests)

# Original-executable identity registry. This target does not link Ghidra pseudocode.
add_executable(btb_original_function_registry_tests
    ${BTB_ROOT}/tests/original_function_registry_test.cpp
)
target_include_directories(btb_original_function_registry_tests PRIVATE ${BTB_ROOT}/include)
btb_require_cpp26(btb_original_function_registry_tests PRIVATE)
add_test(NAME original_function_registry COMMAND btb_original_function_registry_tests)

# Native low-level CSound semantics recovered from 00404430/70/B0.
add_library(btb_dsutil_group_runtime ${BTB_ROOT}/src/dsutil_group_runtime.cpp)
target_include_directories(btb_dsutil_group_runtime PUBLIC ${BTB_ROOT}/include)
btb_require_cpp26(btb_dsutil_group_runtime PUBLIC)
add_executable(btb_dsutil_group_runtime_tests ${BTB_ROOT}/tests/dsutil_group_runtime_test.cpp)
target_link_libraries(btb_dsutil_group_runtime_tests PRIVATE btb_dsutil_group_runtime)
add_test(NAME dsutil_group_runtime COMMAND btb_dsutil_group_runtime_tests)

# Compile and test the previously source-only native audio planner.
# This also validates 0x00402C90 StopAllManagedSounds against recovered
# IsSoundIdPlaying / StopSoundSlot semantics.
add_library(btb_full_game_audio_source ${BTB_ROOT}/src/full_game_audio.cpp)
target_include_directories(btb_full_game_audio_source PUBLIC ${BTB_ROOT}/include)
btb_require_cpp26(btb_full_game_audio_source PUBLIC)
target_link_libraries(btb_full_game_audio_source PUBLIC btb_sound_manager)
add_executable(btb_full_game_audio_source_tests ${BTB_ROOT}/tests/full_game_audio_source_test.cpp)
target_link_libraries(btb_full_game_audio_source_tests PRIVATE btb_full_game_audio_source)
add_test(NAME full_game_audio_source COMMAND btb_full_game_audio_source_tests)


# Compile and test the real source-level 68-state, ten-activity GameRoot and
# original five-profile persistence, not a fabricated game coordinator.
# This target is independent of the Windows-only DirectDraw/DirectSound host.
add_library(btb_full_game_root_source
    ${BTB_ROOT}/src/full_game_runtime.cpp
    ${BTB_ROOT}/src/full_game_profiles.cpp
    ${BTB_ROOT}/src/full_game_profile_name_entry.cpp
    ${BTB_ROOT}/src/original_help2.cpp
    ${BTB_ROOT}/src/full_game_original_help.cpp
    ${BTB_ROOT}/src/original_asset_resolver.cpp
)
target_include_directories(btb_full_game_root_source PUBLIC ${BTB_ROOT}/include)
btb_require_cpp26(btb_full_game_root_source PUBLIC)
target_link_libraries(btb_full_game_root_source PUBLIC
    btb_player_profiles btb_player_progress
)
add_executable(btb_full_game_runtime_source_tests
    ${BTB_ROOT}/tests/full_game_runtime_source_test.cpp
)
target_link_libraries(btb_full_game_runtime_source_tests PRIVATE
    btb_full_game_root_source
)
add_test(NAME full_game_runtime_source COMMAND btb_full_game_runtime_source_tests)
add_executable(btb_full_game_profiles_source_tests
    ${BTB_ROOT}/tests/full_game_profiles_source_test.cpp
)
target_link_libraries(btb_full_game_profiles_source_tests PRIVATE
    btb_full_game_root_source
)
add_test(NAME full_game_profiles_source COMMAND btb_full_game_profiles_source_tests)

# Original retail PE32 leaf routines: directly translated state math and
# low-level API call contracts; not fabricated game implementations.
add_executable(btb_retail_leaf_runtime_tests
    ${BTB_ROOT}/tests/retail_leaf_runtime_test.cpp
)
target_include_directories(btb_retail_leaf_runtime_tests PRIVATE ${BTB_ROOT}/include)
btb_require_cpp26(btb_retail_leaf_runtime_tests PRIVATE)
target_link_libraries(btb_retail_leaf_runtime_tests PRIVATE btb_printing_system)
add_test(NAME retail_leaf_runtime COMMAND btb_retail_leaf_runtime_tests)

# Shared original runtime, graphics data, audio, UI, activity placement,
# and Bink contracts: compile in Linux full-project CI.
add_executable(btb_retail_shared_runtime_tests
    ${BTB_ROOT}/tests/retail_shared_runtime_test.cpp
)
target_include_directories(btb_retail_shared_runtime_tests PRIVATE ${BTB_ROOT}/include)
btb_require_cpp26(btb_retail_shared_runtime_tests PRIVATE)
add_test(NAME retail_shared_runtime COMMAND btb_retail_shared_runtime_tests)

# Real reconstructed fireworks / DirectDraw and float arithmetic helpers.
# Tests verify source semantics; hardware calls are supplied by callbacks.
add_executable(btb_retail_runtime_closure_tests
    ${BTB_ROOT}/tests/retail_runtime_closure_test.cpp
)
target_include_directories(btb_retail_runtime_closure_tests PRIVATE ${BTB_ROOT}/include)
btb_require_cpp26(btb_retail_runtime_closure_tests PRIVATE)
add_test(NAME retail_runtime_closure COMMAND btb_retail_runtime_closure_tests)

# 35-function high-throughput reconstruction: native device lifetimes,
# DirectDraw/DirectSound, WinMM, Bink, search entry points and game audio.
add_executable(btb_retail_system_closure_tests
    ${BTB_ROOT}/tests/retail_system_closure_test.cpp)
target_include_directories(btb_retail_system_closure_tests PRIVATE ${BTB_ROOT}/include)
btb_require_cpp26(btb_retail_system_closure_tests PRIVATE)
add_test(NAME retail_system_closure COMMAND btb_retail_system_closure_tests)

add_executable(btb_retail_media_game_closure_tests
    ${BTB_ROOT}/tests/retail_media_game_closure_test.cpp)
target_include_directories(btb_retail_media_game_closure_tests PRIVATE ${BTB_ROOT}/include)
btb_require_cpp26(btb_retail_media_game_closure_tests PRIVATE)
add_test(NAME retail_media_game_closure COMMAND btb_retail_media_game_closure_tests)

# Ghidra-sourced activity and low-level WinMM/DirectSound/Bink closures.
add_executable(btb_retail_activity_closure_tests
    ${BTB_ROOT}/tests/retail_activity_closure_test.cpp)
target_include_directories(btb_retail_activity_closure_tests PRIVATE ${BTB_ROOT}/include)
btb_require_cpp26(btb_retail_activity_closure_tests PRIVATE)
add_test(NAME retail_activity_closure COMMAND btb_retail_activity_closure_tests)

# Batch: 39 MSVC exception cleanups, 15 game control routines, and
# 17 native CRT/import functions. Independent C++26 behavioral tests.
add_executable(btb_retail_unwind_entrypoints_tests
    ${BTB_ROOT}/tests/retail_unwind_entrypoints_test.cpp)
target_include_directories(btb_retail_unwind_entrypoints_tests PRIVATE ${BTB_ROOT}/include)
btb_require_cpp26(btb_retail_unwind_entrypoints_tests PRIVATE)
add_test(NAME retail_unwind_entrypoints COMMAND btb_retail_unwind_entrypoints_tests)

add_executable(btb_retail_game_batch70_tests
    ${BTB_ROOT}/tests/retail_game_batch70_test.cpp)
target_include_directories(btb_retail_game_batch70_tests PRIVATE ${BTB_ROOT}/include)
btb_require_cpp26(btb_retail_game_batch70_tests PRIVATE)
add_test(NAME retail_game_batch70 COMMAND btb_retail_game_batch70_tests)

add_executable(btb_retail_crt_batch70_tests
    ${BTB_ROOT}/tests/retail_crt_batch70_test.cpp)
target_include_directories(btb_retail_crt_batch70_tests PRIVATE ${BTB_ROOT}/include)
btb_require_cpp26(btb_retail_crt_batch70_tests PRIVATE)
add_test(NAME retail_crt_batch70 COMMAND btb_retail_crt_batch70_tests)

# Source-evidence architecture registry. This is *metadata* for 200 original
# functions; neither an ABI-faithful compiler output nor a C++ body inventory.
add_executable(btb_original_architecture_map_tests
    ${BTB_ROOT}/tests/original_architecture_map_test.cpp)
target_include_directories(btb_original_architecture_map_tests PRIVATE ${BTB_ROOT}/include)
btb_require_cpp26(btb_original_architecture_map_tests PRIVATE)
add_test(NAME original_architecture_map COMMAND btb_original_architecture_map_tests)

# Entire 615-function Ghidra architecture map, including the final 224 CRT
# functions. Metadata test only; no implied C++ behavioral reconstruction.
add_executable(btb_original_architecture_615_tests
    ${BTB_ROOT}/tests/original_architecture_615_test.cpp)
target_include_directories(btb_original_architecture_615_tests PRIVATE ${BTB_ROOT}/include)
btb_require_cpp26(btb_original_architecture_615_tests PRIVATE)
add_test(NAME original_architecture_615 COMMAND btb_original_architecture_615_tests)


# Fireworks original-file adapter: integrates the 18-cell retail sequence,
# the 33-region editor and binary firedata1..5 save files with GameRoot.
# Bink show and certificate presentation remain explicitly host-owned.
add_library(btb_full_game_fireworks_source
    ${BTB_ROOT}/src/full_game_fireworks.cpp
)
target_include_directories(btb_full_game_fireworks_source PUBLIC ${BTB_ROOT}/include)
btb_require_cpp26(btb_full_game_fireworks_source PUBLIC)
target_link_libraries(btb_full_game_fireworks_source PUBLIC
    btb_fireworks btb_full_game_root_source
)
add_executable(btb_full_game_fireworks_source_tests
    ${BTB_ROOT}/tests/full_game_fireworks_source_test.cpp
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
    ${BTB_ROOT}/tests/retail_source_batch_20261011_test.cpp
)
target_include_directories(btb_retail_source_batch_20261011_tests PRIVATE ${BTB_ROOT}/include)
btb_require_cpp26(btb_retail_source_batch_20261011_tests PRIVATE)
add_test(NAME retail_source_batch_20261011
    COMMAND btb_retail_source_batch_20261011_tests)


# 32 additional source-backed original runtime functions (96-bit arithmetic,
# MSVC CRT, string handling, and native callback wrappers).
add_executable(btb_retail_source_batch_32_tests
    ${BTB_ROOT}/tests/retail_source_batch_32_test.cpp
)
target_include_directories(btb_retail_source_batch_32_tests PRIVATE ${BTB_ROOT}/include)
btb_require_cpp26(btb_retail_source_batch_32_tests PRIVATE)
add_test(NAME retail_source_batch_32
    COMMAND btb_retail_source_batch_32_tests)


# Retail Park Designer scene closure: 8 interconnected original functions.
# Use the exact 400 x 76-byte native save layout and verified polygon parity.
# Original Ghidra functions 0040DD50/0040D5B0 and original
# Spud animation/MSVC functions, compiled out of line for direct reuse.
add_library(btb_original_source9_algorithms STATIC
    ${BTB_ROOT}/src/original_source9.cpp)
target_include_directories(btb_original_source9_algorithms PUBLIC ${BTB_ROOT}/include)
btb_require_cpp26(btb_original_source9_algorithms PUBLIC)

add_library(btb_park_designer_scene
    ${BTB_ROOT}/src/park_designer_scene.cpp
    ${BTB_ROOT}/src/park_designer_placement.cpp
)
target_include_directories(btb_park_designer_scene PUBLIC ${BTB_ROOT}/include)
btb_require_cpp26(btb_park_designer_scene PUBLIC)
# Native Park Designer collision checks now execute original 0040DD50
# from a real compiled C++ object, not only an inline source model.
target_link_libraries(btb_park_designer_scene PUBLIC
    btb_original_source9_algorithms)
add_executable(btb_park_designer_scene_tests
    ${BTB_ROOT}/tests/park_designer_scene_test.cpp
)
target_link_libraries(btb_park_designer_scene_tests PRIVATE
    btb_park_designer_scene
)
add_test(NAME park_designer_scene COMMAND btb_park_designer_scene_tests)
add_executable(btb_park_designer_retail_catalog_tests ${BTB_ROOT}/tests/park_designer_retail_catalog_test.cpp)
target_link_libraries(btb_park_designer_retail_catalog_tests PRIVATE btb_park_designer_scene btb_park_designer)
btb_require_cpp26(btb_park_designer_retail_catalog_tests PRIVATE)
if(MSVC)
    target_compile_options(btb_park_designer_retail_catalog_tests PRIVATE /UNDEBUG)
else()
    target_compile_options(btb_park_designer_retail_catalog_tests PRIVATE -UNDEBUG)
endif()
add_test(NAME park_designer_retail_catalog COMMAND btb_park_designer_retail_catalog_tests)


# Original 00410D10 Park Designer outer loop and game dispatcher adapter.
# 0040E440 full editor, DirectDraw bitmap mapping and native music providers
# remain explicit dependencies rather than counterfeit reconstructed hosts.
add_library(btb_park_designer_activity_loop
    ${BTB_ROOT}/src/park_designer_activity_loop.cpp
)
target_include_directories(btb_park_designer_activity_loop PUBLIC ${BTB_ROOT}/include)
btb_require_cpp26(btb_park_designer_activity_loop PUBLIC)
target_link_libraries(btb_park_designer_activity_loop PUBLIC
    btb_park_designer_scene btb_park_designer)

add_executable(btb_park_designer_activity_loop_tests
    ${BTB_ROOT}/tests/park_designer_activity_loop_test.cpp)
target_link_libraries(btb_park_designer_activity_loop_tests PRIVATE
    btb_park_designer_activity_loop)
add_test(NAME park_designer_activity_loop
    COMMAND btb_park_designer_activity_loop_tests)

add_library(btb_full_game_park_designer_source
    ${BTB_ROOT}/src/full_game_park_designer.cpp
    ${BTB_ROOT}/src/full_game_park_scene.cpp
    ${BTB_ROOT}/src/full_game_park_scene.cpp
    ${BTB_ROOT}/src/full_game_park_designer_native.cpp)
target_include_directories(btb_full_game_park_designer_source PUBLIC ${BTB_ROOT}/include)
btb_require_cpp26(btb_full_game_park_designer_source PUBLIC)
target_link_libraries(btb_full_game_park_designer_source PUBLIC
    btb_full_game_root_source btb_retail_game_lifecycle_15 btb_park_designer_activity_loop)
add_executable(btb_full_game_park_designer_source_tests
    ${BTB_ROOT}/tests/full_game_park_designer_source_test.cpp)
target_link_libraries(btb_full_game_park_designer_source_tests PRIVATE
    btb_full_game_park_designer_source)
if(MSVC)
    target_compile_options(btb_full_game_park_designer_source_tests PRIVATE /UNDEBUG)
    target_compile_options(btb_park_designer_scene_tests PRIVATE /UNDEBUG)
else()
    target_compile_options(btb_full_game_park_designer_source_tests PRIVATE -UNDEBUG)
    target_compile_options(btb_park_designer_scene_tests PRIVATE -UNDEBUG)
endif()
add_test(NAME full_game_park_designer_source
    COMMAND btb_full_game_park_designer_source_tests)


# Verified toolbar / View mode slices of central editor 0x0040E440.
# PARTIAL ONLY: does not promote the 10,433-byte whole function to completed.
add_executable(btb_park_designer_editor_control_slice_tests
    ${BTB_ROOT}/tests/park_designer_editor_control_slice_test.cpp)
target_include_directories(btb_park_designer_editor_control_slice_tests PRIVATE ${BTB_ROOT}/include)
btb_require_cpp26(btb_park_designer_editor_control_slice_tests PRIVATE)
add_test(NAME park_designer_editor_control_slice
    COMMAND btb_park_designer_editor_control_slice_tests)


# Throughput batch: 30 authentic MSVC 2003 CRT/function-pointer wrappers,
# stream/character operations, status-table and exception source semantics.
# Source-level models only; native x86 ABI/OS callbacks remain host-owned.
add_executable(btb_retail_crt_dependency_30_tests
    ${BTB_ROOT}/tests/retail_crt_dependency_30_test.cpp)
target_include_directories(btb_retail_crt_dependency_30_tests PRIVATE ${BTB_ROOT}/include)
btb_require_cpp26(btb_retail_crt_dependency_30_tests PRIVATE)
add_test(NAME retail_crt_dependency_30 COMMAND btb_retail_crt_dependency_30_tests)


# Retail 15-function native game lifecycle batch:
# 9 original resource ownership functions; 3 Park Designer save/load/geometry;
# 2 Fireworks editor input entrypoints; 1 Spud Skate state dispatcher.
add_library(btb_retail_game_lifecycle_15
    ${BTB_ROOT}/src/retail_game_lifecycle_15.cpp)
target_include_directories(btb_retail_game_lifecycle_15 PUBLIC ${BTB_ROOT}/include)
btb_require_cpp26(btb_retail_game_lifecycle_15 PUBLIC)
target_link_libraries(btb_retail_game_lifecycle_15 PUBLIC
    btb_park_designer)
add_executable(btb_retail_game_lifecycle_15_tests
    ${BTB_ROOT}/tests/retail_game_lifecycle_15_test.cpp)
target_link_libraries(btb_retail_game_lifecycle_15_tests PRIVATE
    btb_retail_game_lifecycle_15)
add_test(NAME retail_game_lifecycle_15
    COMMAND btb_retail_game_lifecycle_15_tests)


# Batch source-model reconstruction of 0041D4D0/00423F40 activity dispatch,
# 004211A0 Spud Maze directional input and 00423470 collision/drop. Shared
# Maze input, motion, transition, shortest-path and Spud Maze path models are
# explicitly linked and exercised in the combined gameplay regression.
add_executable(btb_maze_spud_frame_batch_tests
    ${BTB_ROOT}/tests/maze_spud_frame_batch_test.cpp)
target_include_directories(btb_maze_spud_frame_batch_tests PRIVATE ${BTB_ROOT}/include)
btb_require_cpp26(btb_maze_spud_frame_batch_tests PRIVATE)
target_link_libraries(btb_maze_spud_frame_batch_tests PRIVATE
    btb_maze btb_spud_maze)
add_test(NAME maze_spud_frame_batch COMMAND btb_maze_spud_frame_batch_tests)


# 20 original-game-function throughput batch. Eleven newly recovered native
# gameplay/data procedures + nine earlier, explicitly validated parsers.
# The original PE32 resource/DirectDraw/Bink/DirectSound effects remain host
# callbacks; passing this test does not imply retail binary/ABI parity.
add_executable(btb_gameplay_data_batch_20_tests
    ${BTB_ROOT}/tests/gameplay_data_batch_20_test.cpp
    ${BTB_ROOT}/src/bobs_band.cpp)
target_include_directories(btb_gameplay_data_batch_20_tests PRIVATE ${BTB_ROOT}/include)
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
    ${BTB_ROOT}/tests/gameplay_control_batch_10_test.cpp
    ${BTB_ROOT}/src/bobs_band.cpp)
target_include_directories(btb_gameplay_control_batch_10_tests PRIVATE ${BTB_ROOT}/include)
btb_require_cpp26(btb_gameplay_control_batch_10_tests PRIVATE)
add_test(NAME gameplay_control_batch_10 COMMAND btb_gameplay_control_batch_10_tests)


# High-throughput original 32-function MSVC 2003 / Win32 dependency batch.
# Original DOS/PE32 Ghidra machine-code input, typed host-facing C++26
# behaviors and their branch/effect-level unit tests. Native SEH/WinAPI and
# byte-identical retail equivalence remain separate validation work.
add_executable(btb_runtime_dependency_32_tests
    ${BTB_ROOT}/tests/runtime_dependency_32_test.cpp)
target_include_directories(btb_runtime_dependency_32_tests PRIVATE ${BTB_ROOT}/include)
btb_require_cpp26(btb_runtime_dependency_32_tests PRIVATE)
add_test(NAME runtime_dependency_32 COMMAND btb_runtime_dependency_32_tests)


# Game-facing shared UI operations: eight complete Ghidra-source models
# + one explicitly partial SaveAndUnloadPlayerProfiles mapping (not
# promoted to source_written until source stream-mode evidence is resolved).
add_executable(btb_shared_ui_native_9_tests
    ${BTB_ROOT}/tests/shared_ui_native_9_test.cpp)
target_include_directories(btb_shared_ui_native_9_tests PRIVATE ${BTB_ROOT}/include)
btb_require_cpp26(btb_shared_ui_native_9_tests PRIVATE)
add_test(NAME shared_ui_native_9 COMMAND btb_shared_ui_native_9_tests)


# Twelve original function bodies across game activities and their shared
# native platform services. Source-level state/operation validation is NOT
# retail x86 ABI, true DirectDraw/DirectSound/Bink end-to-end equivalence.
add_executable(btb_retail_service_batch_8_tests
    ${BTB_ROOT}/tests/retail_service_batch_8_test.cpp)
target_include_directories(btb_retail_service_batch_8_tests PRIVATE ${BTB_ROOT}/include)
btb_require_cpp26(btb_retail_service_batch_8_tests PRIVATE)
add_test(NAME retail_service_batch_8 COMMAND btb_retail_service_batch_8_tests)

add_executable(btb_native_gameplay_batch_6_tests
    ${BTB_ROOT}/tests/native_gameplay_batch_6_test.cpp
    ${BTB_ROOT}/src/bobs_band.cpp)
target_include_directories(btb_native_gameplay_batch_6_tests PRIVATE ${BTB_ROOT}/include)
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
    ${BTB_ROOT}/src/original_editor3.cpp
    ${BTB_ROOT}/src/original_heap9.cpp
    ${BTB_ROOT}/src/original_game_device2.cpp
    ${BTB_ROOT}/src/original_imports3.cpp
    ${BTB_ROOT}/src/original_display3.cpp
    ${BTB_ROOT}/src/original_cxx12.cpp
    ${BTB_ROOT}/src/original_tail4.cpp
    ${BTB_ROOT}/src/original_closure8.cpp
    ${BTB_ROOT}/src/original_input_crt8.cpp
    ${BTB_ROOT}/src/original_gameplay3.cpp
    ${BTB_ROOT}/src/original_fireworks_start2.cpp
    ${BTB_ROOT}/src/original_maze_init2.cpp
    ${BTB_ROOT}/src/original_profiles_fireworks2.cpp
    ${BTB_ROOT}/src/original_draw4.cpp
    ${BTB_ROOT}/src/original_activity4.cpp
    ${BTB_ROOT}/src/original_motion4.cpp
    ${BTB_ROOT}/src/original_coordinators2.cpp
    ${BTB_ROOT}/src/original_gameplay3b.cpp
    ${BTB_ROOT}/src/original_init2.cpp
    ${BTB_ROOT}/src/original_herding_init.cpp
    ${BTB_ROOT}/src/original_park_init.cpp
    ${BTB_ROOT}/src/original_crt_float3.cpp
    ${BTB_ROOT}/src/original_closure4.cpp
    ${BTB_ROOT}/src/original_stdio2.cpp
    ${BTB_ROOT}/src/original_last2.cpp
    ${BTB_ROOT}/src/original_unwind13.cpp
    ${BTB_ROOT}/src/original_ftol1.cpp
    ${BTB_ROOT}/src/original_float2.cpp
    ${BTB_ROOT}/src/original_support3.cpp
    ${BTB_ROOT}/src/original_file7.cpp
    ${BTB_ROOT}/src/original_system2.cpp
    ${BTB_ROOT}/src/original_heap4.cpp
    ${BTB_ROOT}/src/original_memory4.cpp
    ${BTB_ROOT}/src/original_environment4.cpp
    ${BTB_ROOT}/src/original_message1.cpp
    ${BTB_ROOT}/src/original_sound2.cpp
    ${BTB_ROOT}/src/original_printer2.cpp
    ${BTB_ROOT}/src/original_wave7.cpp
    ${BTB_ROOT}/src/original_printer7.cpp
    ${BTB_ROOT}/src/original_runtime6.cpp
    ${BTB_ROOT}/src/original_batch20_validation.cpp
    ${BTB_ROOT}/src/original_locale7.cpp
    ${BTB_ROOT}/src/original_locale7_validation.cpp
    ${BTB_ROOT}/src/original_runtime15.cpp
    ${BTB_ROOT}/src/original_runtime15_validation.cpp
    ${BTB_ROOT}/src/original_exception9.cpp
    ${BTB_ROOT}/src/original_startup11.cpp
    ${BTB_ROOT}/src/original_startup11_validation.cpp
    ${BTB_ROOT}/src/original_float6.cpp
    ${BTB_ROOT}/src/original_string8.cpp
    ${BTB_ROOT}/src/original_string8_validation.cpp
    ${BTB_ROOT}/src/original_runtime16.cpp
    ${BTB_ROOT}/src/original_runtime16_validation.cpp
    ${BTB_ROOT}/src/original_crt9.cpp
    ${BTB_ROOT}/src/original_crt9_probe.cpp
    ${BTB_ROOT}/src/original_shared_ui8.cpp
    ${BTB_ROOT}/src/original_shared_ui8_probe.cpp
    ${BTB_ROOT}/src/original_services8.cpp
    ${BTB_ROOT}/src/original_services8_probe.cpp
    ${BTB_ROOT}/src/original_gameplay6.cpp
    ${BTB_ROOT}/src/original_gameplay6_probe.cpp
    ${BTB_ROOT}/src/original_source9_probe.cpp
    ${BTB_ROOT}/src/original_gameplay9.cpp
    ${BTB_ROOT}/src/original_gameplay9_probe.cpp
    ${BTB_ROOT}/src/original_gameplay11.cpp
    ${BTB_ROOT}/src/original_gameplay11_probe.cpp
    ${BTB_ROOT}/src/original_shared11.cpp
    ${BTB_ROOT}/src/original_shared11_probe.cpp
    ${BTB_ROOT}/src/original_lifecycle12.cpp
    ${BTB_ROOT}/src/original_lifecycle12_probe.cpp
    ${BTB_ROOT}/src/original_activity9_probe.cpp
    ${BTB_ROOT}/src/original_system14.cpp
    ${BTB_ROOT}/src/original_system14_probe.cpp
    ${BTB_ROOT}/src/original_media15.cpp
    ${BTB_ROOT}/src/original_media15_probe.cpp
    ${BTB_ROOT}/src/original_crt10.cpp
    ${BTB_ROOT}/src/original_crt10_probe.cpp
    ${BTB_ROOT}/src/original_runtime8.cpp
    ${BTB_ROOT}/src/original_runtime8_probe.cpp
    ${BTB_ROOT}/src/original_active_frame.cpp
    ${BTB_ROOT}/src/original_active_frame_probe.cpp
    ${BTB_ROOT}/src/original_runtime5.cpp
    ${BTB_ROOT}/src/original_runtime5_probe.cpp
    ${BTB_ROOT}/src/original_crt17.cpp
    ${BTB_ROOT}/src/original_crt17_probe.cpp
    ${BTB_ROOT}/src/native_input_pulse.cpp
    ${BTB_ROOT}/src/original_msvc30.cpp
    ${BTB_ROOT}/src/original_msvc30_probe.cpp
    ${BTB_ROOT}/src/original_games_batch_14.cpp
    ${BTB_ROOT}/src/original_crt_batch_32.cpp
    ${BTB_ROOT}/src/original_game_batch_13.cpp
    ${BTB_ROOT}/src/original_leaf_batch_25.cpp
    ${BTB_ROOT}/src/full_game_native_session.cpp
    ${BTB_ROOT}/src/full_game_effect_pipeline.cpp
    ${BTB_ROOT}/src/full_game_resources.cpp
    ${BTB_ROOT}/src/full_game_bobs_band.cpp
    ${BTB_ROOT}/src/full_game_golf.cpp
    ${BTB_ROOT}/src/full_game_maze.cpp
    ${BTB_ROOT}/src/full_game_spud_maze.cpp
    ${BTB_ROOT}/src/spud_maze_pilchard_native.cpp
    ${BTB_ROOT}/src/full_game_squirrel.cpp
    ${BTB_ROOT}/src/full_game_spud_skate.cpp
    ${BTB_ROOT}/src/full_game_dino.cpp
    ${BTB_ROOT}/src/full_game_profile_screen_native.cpp
    ${BTB_ROOT}/src/golf_activity.cpp
    ${BTB_ROOT}/src/full_game_herding.cpp
    ${BTB_ROOT}/src/full_game_herding_event_simulation.cpp
    ${BTB_ROOT}/src/herding_native_pickles_motion.cpp
    ${BTB_ROOT}/src/herding_behavior_bridge.cpp
    ${BTB_ROOT}/src/full_game_herding_presentation.cpp
    ${BTB_ROOT}/src/full_game_front_end.cpp
    ${BTB_ROOT}/src/full_game_generic_ui.cpp
    ${BTB_ROOT}/src/full_game_startup_movies.cpp
    ${BTB_ROOT}/src/full_game_walkthrough_catalog.cpp
    ${BTB_ROOT}/src/full_game_ui_bitmaps.cpp
    ${BTB_ROOT}/src/full_game_pregame_resources.cpp
    ${BTB_ROOT}/src/bobs_band.cpp
    ${BTB_ROOT}/src/bobs_band_activity.cpp
    ${BTB_ROOT}/src/bobs_band_data.cpp
    ${BTB_ROOT}/src/bobs_band_animation.cpp
    ${BTB_ROOT}/src/bobs_band_persistence.cpp
    ${BTB_ROOT}/src/recompiled_core_smoke.cpp
    ${BTB_ROOT}/src/recompiled_gameplay_integration.cpp
)
target_include_directories(btb_recompiled_core PUBLIC ${BTB_ROOT}/include)
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
    ${BTB_ROOT}/tests/original_game_batch_13_test.cpp)
target_link_libraries(btb_original_game_batch_13_tests PRIVATE btb_recompiled_core)
btb_require_cpp26(btb_original_game_batch_13_tests PRIVATE)
add_test(NAME original_game_batch_13 COMMAND btb_original_game_batch_13_tests)

add_executable(btb_original_leaf_batch_25_tests
    ${BTB_ROOT}/tests/original_leaf_batch_25_test.cpp)
target_link_libraries(btb_original_leaf_batch_25_tests PRIVATE btb_recompiled_core)
btb_require_cpp26(btb_original_leaf_batch_25_tests PRIVATE)
add_test(NAME original_leaf_batch_25 COMMAND btb_original_leaf_batch_25_tests)

add_executable(btb_original_games_batch_14_tests
    ${BTB_ROOT}/tests/original_games_batch_14_test.cpp)
target_link_libraries(btb_original_games_batch_14_tests PRIVATE btb_recompiled_core)
btb_require_cpp26(btb_original_games_batch_14_tests PRIVATE)
add_test(NAME original_games_batch_14 COMMAND btb_original_games_batch_14_tests)

add_executable(btb_original_crt_batch_32_tests
    ${BTB_ROOT}/tests/original_crt_batch_32_test.cpp)
target_link_libraries(btb_original_crt_batch_32_tests PRIVATE btb_recompiled_core)
btb_require_cpp26(btb_original_crt_batch_32_tests PRIVATE)
add_test(NAME original_crt_batch_32 COMMAND btb_original_crt_batch_32_tests)

add_executable(btb_original_msvc30_tests
    ${BTB_ROOT}/tests/original_msvc30_test.cpp)
target_link_libraries(btb_original_msvc30_tests PRIVATE btb_recompiled_core)
btb_require_cpp26(btb_original_msvc30_tests PRIVATE)
add_test(NAME original_msvc30 COMMAND btb_original_msvc30_tests)


# Event-driven keyboard/mouse/text native host admission, shared by x86 and x64.
add_executable(btb_native_host_input_tests ${BTB_ROOT}/tests/native_host_input_test.cpp)
target_link_libraries(btb_native_host_input_tests PRIVATE btb_recompiled_core)
btb_require_cpp26(btb_native_host_input_tests PRIVATE)
add_test(NAME native_host_input COMMAND btb_native_host_input_tests)
add_executable(btb_native_input_pulse_tests
    ${BTB_ROOT}/tests/native_input_pulse_test.cpp)
target_link_libraries(btb_native_input_pulse_tests PRIVATE btb_recompiled_core)
btb_require_cpp26(btb_native_input_pulse_tests PRIVATE)
add_test(NAME native_input_pulse COMMAND btb_native_input_pulse_tests)

add_executable(btb_original_crt17_tests
    ${BTB_ROOT}/tests/original_crt17_test.cpp)
target_link_libraries(btb_original_crt17_tests PRIVATE btb_recompiled_core)
btb_require_cpp26(btb_original_crt17_tests PRIVATE)
add_test(NAME original_crt17 COMMAND btb_original_crt17_tests)

add_executable(btb_original_runtime5_tests
    ${BTB_ROOT}/tests/original_runtime5_test.cpp)
target_link_libraries(btb_original_runtime5_tests PRIVATE btb_recompiled_core)
btb_require_cpp26(btb_original_runtime5_tests PRIVATE)
add_test(NAME original_runtime5 COMMAND btb_original_runtime5_tests)

add_executable(btb_original_active_frame_tests
    ${BTB_ROOT}/tests/original_active_frame_test.cpp)
target_link_libraries(btb_original_active_frame_tests PRIVATE btb_recompiled_core)
btb_require_cpp26(btb_original_active_frame_tests PRIVATE)
add_test(NAME original_active_frame COMMAND btb_original_active_frame_tests)

add_executable(btb_original_runtime8_tests
    ${BTB_ROOT}/tests/original_runtime8_test.cpp)
target_link_libraries(btb_original_runtime8_tests PRIVATE btb_recompiled_core)
btb_require_cpp26(btb_original_runtime8_tests PRIVATE)
add_test(NAME original_runtime8 COMMAND btb_original_runtime8_tests)

add_executable(btb_original_crt10_tests
    ${BTB_ROOT}/tests/original_crt10_test.cpp)
target_link_libraries(btb_original_crt10_tests PRIVATE btb_recompiled_core)
btb_require_cpp26(btb_original_crt10_tests PRIVATE)
add_test(NAME original_crt10 COMMAND btb_original_crt10_tests)

# Previously orphaned source-native-session tests are now compiled
# and run on every supported architecture, with checks active in Release.
# Native source Dinosaur retail asset/drag-drop/progress regression.
# Test only; original proprietary bitmaps are not committed to the repo.
# Exercise real retail five-sign screen: original font data, saved-player
# selection, the state-1 name-entry caller and fail-closed missing assets.
add_executable(btb_full_game_profile_selector_native_tests
    ${BTB_ROOT}/tests/full_game_profile_selector_native_test.cpp)
target_link_libraries(btb_full_game_profile_selector_native_tests PRIVATE
    btb_recompiled_core)
btb_require_cpp26(btb_full_game_profile_selector_native_tests PRIVATE)
add_test(NAME full_game_profile_selector_native
    COMMAND btb_full_game_profile_selector_native_tests)

# Actual native Golf completion tests. These existing C++26 files were
# previously outside the CTest registry and thus never executed by CI.
add_executable(btb_full_game_golf_source_tests
    ${BTB_ROOT}/tests/full_game_golf_source_test.cpp)
target_link_libraries(btb_full_game_golf_source_tests PRIVATE
    btb_recompiled_core)
btb_require_cpp26(btb_full_game_golf_source_tests PRIVATE)
add_test(NAME full_game_golf_source
    COMMAND btb_full_game_golf_source_tests)

add_executable(btb_full_game_adventure_to_golf_source_tests
    ${BTB_ROOT}/tests/full_game_adventure_to_golf_source_test.cpp)
target_link_libraries(btb_full_game_adventure_to_golf_source_tests PRIVATE
    btb_recompiled_core)
btb_require_cpp26(btb_full_game_adventure_to_golf_source_tests PRIVATE)
add_test(NAME full_game_adventure_to_golf_source
    COMMAND btb_full_game_adventure_to_golf_source_tests)

add_executable(btb_full_game_dino_native_tests
    ${BTB_ROOT}/tests/full_game_dino_native_test.cpp)
target_link_libraries(btb_full_game_dino_native_tests PRIVATE
    btb_recompiled_core)
btb_require_cpp26(btb_full_game_dino_native_tests PRIVATE)
add_test(NAME full_game_dino_native
    COMMAND btb_full_game_dino_native_tests)

add_executable(btb_full_game_native_session_source_tests
    ${BTB_ROOT}/tests/full_game_native_session_source_test.cpp)
target_link_libraries(btb_full_game_native_session_source_tests PRIVATE
    btb_recompiled_core)
btb_require_cpp26(btb_full_game_native_session_source_tests PRIVATE)
add_test(NAME full_game_native_session_source
    COMMAND btb_full_game_native_session_source_tests)

# Native source Maze first pass: actual original map parser, original
# 0041B140 player movement, bitmap/timer effects and truthful timeout flow.
# Original retail PE-backed 46px Maze package collision, stolen and dropped
# status and final package win-counter semantics.
add_executable(btb_maze_package_pickup_source_tests
    ${BTB_ROOT}/tests/maze_package_pickup_source_test.cpp)
target_link_libraries(btb_maze_package_pickup_source_tests PRIVATE
    btb_recompiled_core)
btb_require_cpp26(btb_maze_package_pickup_source_tests PRIVATE)
add_test(NAME maze_package_pickup_source
    COMMAND btb_maze_package_pickup_source_tests)

# Real native source Spud Maze initialization, five-screen graph, repair
# population, bitmap presentation, countdown and no-fake-win teardown.
# Source-backed Pilchard collision/hammer bridge linked to native game.
add_executable(btb_spud_maze_pilchard_native_tests
    ${BTB_ROOT}/tests/spud_maze_pilchard_native_test.cpp)
target_link_libraries(btb_spud_maze_pilchard_native_tests PRIVATE
    btb_recompiled_core)
btb_require_cpp26(btb_spud_maze_pilchard_native_tests PRIVATE)
add_test(NAME spud_maze_pilchard_native
    COMMAND btb_spud_maze_pilchard_native_tests)

# Rebased Squirrel gameplay and progression CI after Pilchard.
# Native retail Pets Corner initializer, Farmer Pickles navigation and
# source-owned (not fabricated) animal population and completion state.
add_executable(btb_full_game_herding_firstpass_native_tests
    ${BTB_ROOT}/tests/full_game_herding_firstpass_native_test.cpp)
target_link_libraries(btb_full_game_herding_firstpass_native_tests PRIVATE
    btb_recompiled_core)
btb_require_cpp26(btb_full_game_herding_firstpass_native_tests PRIVATE)
add_test(NAME full_game_herding_firstpass_native
    COMMAND btb_full_game_herding_firstpass_native_tests)
# Optional read-only installed-data playthrough, independent of hosted CI assets.
add_executable(btb_retail_herding_playthrough EXCLUDE_FROM_ALL
    ${BTB_ROOT}/tools/native/retail_herding_playthrough.cpp)
target_link_libraries(btb_retail_herding_playthrough PRIVATE btb_recompiled_core)
btb_require_cpp26(btb_retail_herding_playthrough PRIVATE)
set(BTB_HERDING_INTEGRATED_TARGETS btb_full_game_herding_firstpass_native_tests)
foreach(test_name IN ITEMS
    full_game_herding_event_simulation_source
    full_game_herding_retail_map_source
    full_game_herding_presentation_source
    full_game_herding_source
    herding_behavior_bridge_source
)
    set(target_name btb_${test_name}_tests)
    add_executable(${target_name} ${BTB_ROOT}/tests/${test_name}_test.cpp)
    target_link_libraries(${target_name} PRIVATE btb_recompiled_core)
    btb_require_cpp26(${target_name} PRIVATE)
    if(MSVC)
        target_compile_options(${target_name} PRIVATE /UNDEBUG)
    else()
        target_compile_options(${target_name} PRIVATE -UNDEBUG)
    endif()
    add_test(NAME ${test_name} COMMAND ${target_name})
    list(APPEND BTB_HERDING_INTEGRATED_TARGETS ${target_name})
endforeach()
# Compile installed-data diagnostics in CI; execute only with the real assets.
add_executable(btb_retail_golf_squirrel_playthrough
    ${BTB_ROOT}/tools/native/retail_golf_squirrel_playthrough.cpp)
target_link_libraries(btb_retail_golf_squirrel_playthrough PRIVATE btb_recompiled_core)
btb_require_cpp26(btb_retail_golf_squirrel_playthrough PRIVATE)
add_executable(btb_retail_maze_playthrough
    ${BTB_ROOT}/tools/native/retail_maze_playthrough.cpp)
target_link_libraries(btb_retail_maze_playthrough PRIVATE btb_recompiled_core)
btb_require_cpp26(btb_retail_maze_playthrough PRIVATE)
add_executable(btb_retail_spud_maze_playthrough
    ${BTB_ROOT}/tools/native/retail_spud_maze_playthrough.cpp)
target_link_libraries(btb_retail_spud_maze_playthrough PRIVATE btb_recompiled_core)
btb_require_cpp26(btb_retail_spud_maze_playthrough PRIVATE)
find_package(Python3 QUIET COMPONENTS Interpreter)
if(Python3_Interpreter_FOUND)
    add_test(NAME retail_golf_squirrel_metadata
        COMMAND "${Python3_EXECUTABLE}" "${BTB_ROOT}/tools/test_retail_golf_squirrel_playthrough.py"
                "$<TARGET_FILE:btb_retail_golf_squirrel_playthrough>")
endif()
add_custom_target(btb_herding_source_regressions DEPENDS
    ${BTB_HERDING_PURE_TARGETS} ${BTB_HERDING_INTEGRATED_TARGETS})

# Compile and execute all previously standalone whole-game source contracts.
set(BTB_FULL_GAME_SOURCE_TARGETS "")
foreach(test_name IN ITEMS
    bobs_band_persistence_source
    full_game_blt_clip_source
    full_game_bobs_band_source
    full_game_effect_pipeline_source
    full_game_front_end_source
    full_game_generic_ui_source
    full_game_menu_actions_source
    full_game_park_scene_source
    full_game_park_native_source
    park_designer_placement_source
    full_game_pregame_resources_source
    full_game_pregame_root_source
    full_game_pregame_source
    full_game_resources_source
    full_game_startup_movies_source
    full_game_ui_bitmaps_source
    full_game_walkthrough_catalog_source
    golf_activity_source
    original_asset_resolver_source
    retail_crt_random_source
    retail_point_in_polygon_source
)
    set(target_name btb_${test_name}_tests)
    add_executable(${target_name} ${BTB_ROOT}/tests/${test_name}_test.cpp)
    target_link_libraries(${target_name} PRIVATE btb_recompiled_core)
    btb_require_cpp26(${target_name} PRIVATE)
    if(MSVC)
        target_compile_options(${target_name} PRIVATE /UNDEBUG)
    else()
        target_compile_options(${target_name} PRIVATE -UNDEBUG)
    endif()
    add_test(NAME ${test_name} COMMAND ${target_name})
    list(APPEND BTB_FULL_GAME_SOURCE_TARGETS ${target_name})
endforeach()
add_custom_target(btb_full_game_source_regressions DEPENDS ${BTB_FULL_GAME_SOURCE_TARGETS} btb_park_designer_retail_catalog_tests btb_full_game_park_designer_source_tests btb_park_designer_scene_tests)

add_executable(btb_full_game_squirrel_native_tests
    ${BTB_ROOT}/tests/full_game_squirrel_native_test.cpp)
target_link_libraries(btb_full_game_squirrel_native_tests PRIVATE
    btb_recompiled_core)
btb_require_cpp26(btb_full_game_squirrel_native_tests PRIVATE)
add_test(NAME full_game_squirrel_native
    COMMAND btb_full_game_squirrel_native_tests)
# Windows CI previously omitted both integrated Golf tests and Squirrel.
add_dependencies(btb_full_game_source_regressions
    btb_full_game_squirrel_native_tests btb_full_game_golf_source_tests
    btb_full_game_adventure_to_golf_source_tests btb_squirrel_runtime_tests
    btb_squirrel_data_tests btb_squirrel_presentation_tests
    btb_retail_golf_squirrel_playthrough btb_retail_maze_playthrough btb_retail_spud_maze_playthrough)

add_executable(btb_full_game_spud_maze_native_tests
    ${BTB_ROOT}/tests/full_game_spud_maze_native_test.cpp)
target_link_libraries(btb_full_game_spud_maze_native_tests PRIVATE
    btb_recompiled_core)
btb_require_cpp26(btb_full_game_spud_maze_native_tests PRIVATE)
add_test(NAME full_game_spud_maze_native
    COMMAND btb_full_game_spud_maze_native_tests)

add_executable(btb_full_game_maze_native_tests
    ${BTB_ROOT}/tests/full_game_maze_native_test.cpp)
target_link_libraries(btb_full_game_maze_native_tests PRIVATE
    btb_recompiled_core)
btb_require_cpp26(btb_full_game_maze_native_tests PRIVATE)
add_test(NAME full_game_maze_native COMMAND btb_full_game_maze_native_tests)

add_executable(btb_original_media15_tests
    ${BTB_ROOT}/tests/original_media15_test.cpp)
target_link_libraries(btb_original_media15_tests PRIVATE btb_recompiled_core)
btb_require_cpp26(btb_original_media15_tests PRIVATE)
add_test(NAME original_media15 COMMAND btb_original_media15_tests)

add_executable(btb_original_system14_tests
    ${BTB_ROOT}/tests/original_system14_test.cpp)
target_link_libraries(btb_original_system14_tests PRIVATE btb_recompiled_core)
btb_require_cpp26(btb_original_system14_tests PRIVATE)
add_test(NAME original_system14 COMMAND btb_original_system14_tests)

add_executable(btb_original_activity9_tests
    ${BTB_ROOT}/tests/original_activity9_test.cpp)
target_link_libraries(btb_original_activity9_tests PRIVATE btb_recompiled_core)
btb_require_cpp26(btb_original_activity9_tests PRIVATE)
add_test(NAME original_activity9 COMMAND btb_original_activity9_tests)

add_executable(btb_original_lifecycle12_tests
    ${BTB_ROOT}/tests/original_lifecycle12_test.cpp)
target_link_libraries(btb_original_lifecycle12_tests PRIVATE btb_recompiled_core)
btb_require_cpp26(btb_original_lifecycle12_tests PRIVATE)
add_test(NAME original_lifecycle12 COMMAND btb_original_lifecycle12_tests)

add_executable(btb_original_shared11_tests ${BTB_ROOT}/tests/original_shared11_test.cpp)
target_link_libraries(btb_original_shared11_tests PRIVATE btb_recompiled_core)
btb_require_cpp26(btb_original_shared11_tests PRIVATE)
add_test(NAME original_shared11 COMMAND btb_original_shared11_tests)

add_executable(btb_original_gameplay11_tests
    ${BTB_ROOT}/tests/original_gameplay11_test.cpp)
target_link_libraries(btb_original_gameplay11_tests PRIVATE btb_recompiled_core)
btb_require_cpp26(btb_original_gameplay11_tests PRIVATE)
add_test(NAME original_gameplay11 COMMAND btb_original_gameplay11_tests)

add_executable(btb_original_gameplay9_tests
    ${BTB_ROOT}/tests/original_gameplay9_test.cpp)
target_link_libraries(btb_original_gameplay9_tests PRIVATE btb_recompiled_core)
btb_require_cpp26(btb_original_gameplay9_tests PRIVATE)
add_test(NAME original_gameplay9 COMMAND btb_original_gameplay9_tests)

add_executable(btb_original_source9_tests
    ${BTB_ROOT}/tests/original_source9_test.cpp)
target_link_libraries(btb_original_source9_tests PRIVATE btb_recompiled_core)
btb_require_cpp26(btb_original_source9_tests PRIVATE)
add_test(NAME original_source9 COMMAND btb_original_source9_tests)

add_executable(btb_original_gameplay6_tests
    ${BTB_ROOT}/tests/original_gameplay6_test.cpp)
target_link_libraries(btb_original_gameplay6_tests PRIVATE btb_recompiled_core)
btb_require_cpp26(btb_original_gameplay6_tests PRIVATE)
add_test(NAME original_gameplay6 COMMAND btb_original_gameplay6_tests)

add_executable(btb_original_services8_tests
    ${BTB_ROOT}/tests/original_services8_test.cpp)
target_link_libraries(btb_original_services8_tests PRIVATE btb_recompiled_core)
btb_require_cpp26(btb_original_services8_tests PRIVATE)
add_test(NAME original_services8 COMMAND btb_original_services8_tests)

add_executable(btb_original_shared_ui8_tests
    ${BTB_ROOT}/tests/original_shared_ui8_test.cpp)
target_link_libraries(btb_original_shared_ui8_tests PRIVATE btb_recompiled_core)
btb_require_cpp26(btb_original_shared_ui8_tests PRIVATE)
add_test(NAME original_shared_ui8 COMMAND btb_original_shared_ui8_tests)

add_executable(btb_original_crt9_tests
    ${BTB_ROOT}/tests/original_crt9_test.cpp)
target_link_libraries(btb_original_crt9_tests PRIVATE btb_recompiled_core)
btb_require_cpp26(btb_original_crt9_tests PRIVATE)
add_test(NAME original_crt9 COMMAND btb_original_crt9_tests)

add_executable(btb_recompiled_core_link_tests
    ${BTB_ROOT}/tests/recompiled_core_link_test.cpp)
target_link_libraries(btb_recompiled_core_link_tests PRIVATE btb_recompiled_core)
add_test(NAME recompiled_core_link COMMAND btb_recompiled_core_link_tests)

# This source regression was previously checked in but never registered.
add_executable(btb_bobs_band_source_tests ${BTB_ROOT}/tests/bobs_band_source_test.cpp)
target_link_libraries(btb_bobs_band_source_tests PRIVATE btb_recompiled_core)
add_test(NAME bobs_band_source COMMAND btb_bobs_band_source_tests)

add_executable(btb_original_profile_screen_tests ${BTB_ROOT}/tests/original_profile_screen_test.cpp)
target_link_libraries(btb_original_profile_screen_tests PRIVATE btb_recompiled_core)
add_test(NAME original_profile_screen COMMAND btb_original_profile_screen_tests)

add_executable(btb_original_runtime16_tests ${BTB_ROOT}/tests/original_runtime16_test.cpp)
target_link_libraries(btb_original_runtime16_tests PRIVATE btb_recompiled_core)
add_test(NAME original_runtime16 COMMAND btb_original_runtime16_tests)

add_executable(btb_original_string8_tests ${BTB_ROOT}/tests/original_string8_test.cpp)
target_link_libraries(btb_original_string8_tests PRIVATE btb_recompiled_core)
add_test(NAME original_string8 COMMAND btb_original_string8_tests)

add_executable(btb_retail_pure_runtime_vectors_tests ${BTB_ROOT}/tests/retail_pure_runtime_vectors_test.cpp)
target_link_libraries(btb_retail_pure_runtime_vectors_tests PRIVATE btb_recompiled_core)
add_test(NAME retail_pure_runtime_vectors COMMAND btb_retail_pure_runtime_vectors_tests
    "${BTB_ROOT}/analysis/reconstruction/retail-pure-runtime-vectors.txt")

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

add_executable(btb_full_game_fireworks_transition_tests
    ${BTB_ROOT}/tests/full_game_fireworks_transition_test.cpp)
target_link_libraries(btb_full_game_fireworks_transition_tests PRIVATE
    btb_recompiled_core)
btb_require_cpp26(btb_full_game_fireworks_transition_tests PRIVATE)
# These fixtures assert real frame and audio transitions; ensure they run
# even for Windows Release builds where NDEBUG would normally remove them.
if(MSVC)
    target_compile_options(btb_full_game_fireworks_transition_tests PRIVATE /UNDEBUG)
else()
    target_compile_options(btb_full_game_fireworks_transition_tests PRIVATE -UNDEBUG)
endif()
add_test(NAME full_game_fireworks_transition
    COMMAND btb_full_game_fireworks_transition_tests)

add_executable(btb_fireworks_native_show_tests
    ${BTB_ROOT}/tests/fireworks_native_show_test.cpp)
target_link_libraries(btb_fireworks_native_show_tests PRIVATE
    btb_recompiled_core)
btb_require_cpp26(btb_fireworks_native_show_tests PRIVATE)
if(MSVC)
    target_compile_options(btb_fireworks_native_show_tests PRIVATE /UNDEBUG)
else()
    target_compile_options(btb_fireworks_native_show_tests PRIVATE -UNDEBUG)
endif()
add_test(NAME fireworks_native_show COMMAND btb_fireworks_native_show_tests)

add_executable(btb_fireworks_movie_bank_catalog_tests
    ${BTB_ROOT}/tests/fireworks_movie_bank_catalog_test.cpp)
target_include_directories(btb_fireworks_movie_bank_catalog_tests PRIVATE ${BTB_ROOT}/include)
btb_require_cpp26(btb_fireworks_movie_bank_catalog_tests PRIVATE)
if(MSVC)
    target_compile_options(btb_fireworks_movie_bank_catalog_tests PRIVATE /UNDEBUG)
else()
    target_compile_options(btb_fireworks_movie_bank_catalog_tests PRIVATE -UNDEBUG)
endif()
add_test(NAME fireworks_movie_bank_catalog
    COMMAND btb_fireworks_movie_bank_catalog_tests)

add_executable(btb_full_game_spud_skate_native_tests
    ${BTB_ROOT}/tests/full_game_spud_skate_native_test.cpp)
target_link_libraries(btb_full_game_spud_skate_native_tests PRIVATE
    btb_recompiled_core)
btb_require_cpp26(btb_full_game_spud_skate_native_tests PRIVATE)
add_test(NAME full_game_spud_skate_native
    COMMAND btb_full_game_spud_skate_native_tests)

# Test actual linked game executable on every supported platform,
# including original x86 Windows (not only a library-linked test program).
add_test(NAME recompiled_game_executable_link
    COMMAND btb_game --recompiled-smoke)

# Compiles previously orphaned original native Windows implementations as part
# of the REAL Windows game executable, not just behavior-plan tests.
if(WIN32)
    add_library(btb_original_win32_platform STATIC
        ${BTB_ROOT}/runtime/win32_original_imports3.cpp
        ${BTB_ROOT}/runtime/win32_original_directdraw.cpp
        ${BTB_ROOT}/runtime/original_recompiled_bitmaps.cpp
        ${BTB_ROOT}/runtime/win32_recompiled_graphics.cpp
        ${BTB_ROOT}/runtime/win32_native_bitmap_registry.cpp
        ${BTB_ROOT}/runtime/win32_original_directsound.cpp
        ${BTB_ROOT}/runtime/win32_original_bink_exports.cpp
        ${BTB_ROOT}/runtime/win32_retail_bink_provider.cpp
        ${BTB_ROOT}/runtime/win32_native_bink_surface.cpp
        ${BTB_ROOT}/runtime/win32_native_bink_pcm_voice.cpp
        ${BTB_ROOT}/runtime/win32_native_silent_bink_provider.cpp
        ${BTB_ROOT}/runtime/win32_hybrid_bink_provider.cpp
        ${BTB_ROOT}/runtime/win32_retail_spud_skate_movies.cpp
        ${BTB_ROOT}/runtime/win32_native_spud_skate_movies.cpp
        ${BTB_ROOT}/runtime/win32_retail_fireworks_movie_bank.cpp
        ${BTB_ROOT}/runtime/win32_native_fireworks_movie_bank.cpp
        ${BTB_ROOT}/runtime/win32_retail_print_provider.cpp
        ${BTB_ROOT}/runtime/win32_original_game_device.cpp
        ${BTB_ROOT}/runtime/win32_original_platform_link.cpp
        ${BTB_ROOT}/runtime/win32_recompiled_host.cpp
        ${BTB_ROOT}/runtime/win32_original_display_lifecycle.cpp
        ${BTB_ROOT}/runtime/win32_original_main_window_proc.cpp
    )
    target_include_directories(btb_original_win32_platform PUBLIC ${BTB_ROOT}/include)
    btb_require_cpp26(btb_original_win32_platform PUBLIC)
    target_link_libraries(btb_original_win32_platform PUBLIC
        btb_recompiled_core btb_bink32_recovered ddraw dxguid dsound winmm ole32 gdi32 user32 comdlg32)
    target_link_libraries(btb_game PRIVATE btb_original_win32_platform)

    target_link_libraries(btb_game PRIVATE shell32)

    add_executable(btb_original_win32_platform_link_tests
        ${BTB_ROOT}/tests/original_win32_platform_link_test.cpp)
    target_link_libraries(btb_original_win32_platform_link_tests
        PRIVATE btb_original_win32_platform)
    btb_require_cpp26(btb_original_win32_platform_link_tests PRIVATE)
    add_test(NAME original_win32_platform_link
        COMMAND btb_original_win32_platform_link_tests)
endif()

# Fail configuration if any production .cpp is silently left out of the
# build, as happened previously to the Golf and Herding source files.
if(WIN32)
    target_sources(btb_recompiled_core PRIVATE ${BTB_ROOT}/runtime/win32_original_locale7.cpp)
endif()

file(GLOB BTB_PRODUCTION_CPP CONFIGURE_DEPENDS
    "${BTB_ROOT}/src/*.cpp"
    "${BTB_ROOT}/runtime/*.cpp")
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
            BASE_DIR "${BTB_ROOT}")
        if(btb_absolute MATCHES "/(src|runtime)/[^/]+[.]cpp$")
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

add_executable(btb_original_float6_tests ${BTB_ROOT}/tests/original_float6_test.cpp)
target_link_libraries(btb_original_float6_tests PRIVATE btb_recompiled_core)
add_test(NAME original_float6 COMMAND btb_original_float6_tests ${BTB_ROOT}/analysis/reconstruction/retail-float-vectors.txt)

add_executable(btb_original_exception9_tests ${BTB_ROOT}/tests/original_exception9_test.cpp)
target_link_libraries(btb_original_exception9_tests PRIVATE btb_recompiled_core)
add_test(NAME original_exception9 COMMAND btb_original_exception9_tests)
if(MSVC)
    target_compile_options(btb_original_exception9_tests PRIVATE /UNDEBUG)
else()
    target_compile_options(btb_original_exception9_tests PRIVATE -UNDEBUG)
endif()
add_executable(btb_original_startup11_tests ${BTB_ROOT}/tests/original_startup11_test.cpp)
target_link_libraries(btb_original_startup11_tests PRIVATE btb_recompiled_core)
add_test(NAME original_startup11 COMMAND btb_original_startup11_tests ${BTB_ROOT}/analysis/reconstruction/retail-startup-vectors.txt)

add_executable(btb_original_help2_tests ${BTB_ROOT}/tests/original_help2_test.cpp)
target_link_libraries(btb_original_help2_tests PRIVATE btb_recompiled_core)
add_test(NAME original_help2 COMMAND btb_original_help2_tests ${BTB_ROOT}/analysis/reconstruction/retail-help-vectors.txt)
if(MSVC)
    target_compile_options(btb_original_help2_tests PRIVATE /UNDEBUG)
else()
    target_compile_options(btb_original_help2_tests PRIVATE -UNDEBUG)
endif()

add_executable(btb_original_editor3_tests ${BTB_ROOT}/tests/original_editor3_test.cpp)
target_link_libraries(btb_original_editor3_tests PRIVATE btb_recompiled_core)
add_test(NAME original_editor3 COMMAND btb_original_editor3_tests ${BTB_ROOT}/analysis/reconstruction/retail-editor-vectors.txt)

add_executable(btb_original_runtime15_tests ${BTB_ROOT}/tests/original_runtime15_test.cpp)
target_link_libraries(btb_original_runtime15_tests PRIVATE btb_recompiled_core)
add_test(NAME original_runtime15 COMMAND btb_original_runtime15_tests ${BTB_ROOT}/analysis/reconstruction/retail-runtime15-vectors.txt)
add_executable(btb_original_locale7_tests ${BTB_ROOT}/tests/original_locale7_test.cpp)
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
    add_executable(btb_original_${group}_tests ${BTB_ROOT}/tests/original_${group}_test.cpp)
    target_link_libraries(btb_original_${group}_tests PRIVATE btb_recompiled_core)
    add_test(NAME original_${group} COMMAND btb_original_${group}_tests ${BTB_ROOT}/analysis/reconstruction/retail-batch20-vectors.txt)
    if(MSVC)
        target_compile_options(btb_original_${group}_tests PRIVATE /UNDEBUG)
    else()
        target_compile_options(btb_original_${group}_tests PRIVATE -UNDEBUG)
    endif()
endforeach()

add_executable(btb_original_batch13_tests ${BTB_ROOT}/tests/original_batch13_test.cpp)
target_link_libraries(btb_original_batch13_tests PRIVATE btb_recompiled_core)
add_test(NAME original_batch13 COMMAND btb_original_batch13_tests ${BTB_ROOT}/analysis/reconstruction/retail-batch13-vectors.txt)
if(MSVC)
 target_compile_options(btb_original_batch13_tests PRIVATE /UNDEBUG)
else()
 target_compile_options(btb_original_batch13_tests PRIVATE -UNDEBUG)
endif()

add_executable(btb_original_io13_tests ${BTB_ROOT}/tests/original_io13_test.cpp)
target_link_libraries(btb_original_io13_tests PRIVATE btb_recompiled_core)
add_test(NAME original_io13 COMMAND btb_original_io13_tests ${BTB_ROOT}/analysis/reconstruction/retail-io13-vectors.txt)
if(MSVC)
 target_compile_options(btb_original_io13_tests PRIVATE /UNDEBUG)
else()
 target_compile_options(btb_original_io13_tests PRIVATE -UNDEBUG)
endif()

add_executable(btb_original_heap14_tests ${BTB_ROOT}/tests/original_heap14_test.cpp)
target_link_libraries(btb_original_heap14_tests PRIVATE btb_recompiled_core)
add_test(NAME original_heap14 COMMAND btb_original_heap14_tests ${BTB_ROOT}/analysis/reconstruction/retail-heap14-vectors.txt)
if(MSVC)
 target_compile_options(btb_original_heap14_tests PRIVATE /UNDEBUG)
else()
 target_compile_options(btb_original_heap14_tests PRIVATE -UNDEBUG)
endif()

add_executable(btb_original_unwind15_tests ${BTB_ROOT}/tests/original_unwind15_test.cpp)
target_link_libraries(btb_original_unwind15_tests PRIVATE btb_recompiled_core)
add_test(NAME original_unwind15 COMMAND btb_original_unwind15_tests ${BTB_ROOT}/analysis/reconstruction/retail-unwind15-vectors.txt)
if(MSVC)
 target_compile_options(btb_original_unwind15_tests PRIVATE /UNDEBUG)
else()
 target_compile_options(btb_original_unwind15_tests PRIVATE -UNDEBUG)
endif()

add_executable(btb_original_cxx12_tests ${BTB_ROOT}/tests/original_cxx12_test.cpp)
target_link_libraries(btb_original_cxx12_tests PRIVATE btb_recompiled_core)
add_test(NAME original_cxx12 COMMAND btb_original_cxx12_tests ${BTB_ROOT}/analysis/reconstruction/retail-cxx12-vectors.txt)
if(MSVC)
 target_compile_options(btb_original_cxx12_tests PRIVATE /UNDEBUG)
else()
 target_compile_options(btb_original_cxx12_tests PRIVATE -UNDEBUG)
endif()

add_executable(btb_original_device8_tests ${BTB_ROOT}/tests/original_device8_test.cpp)
target_link_libraries(btb_original_device8_tests PRIVATE btb_recompiled_core)
add_test(NAME original_device8 COMMAND btb_original_device8_tests ${BTB_ROOT}/analysis/reconstruction/retail-device8-vectors.txt)
if(MSVC)
 target_compile_options(btb_original_device8_tests PRIVATE /UNDEBUG)
else()
 target_compile_options(btb_original_device8_tests PRIVATE -UNDEBUG)
endif()

# Four original CRT/time/stack-unwind source-level boundary functions.
add_executable(btb_original_tail4_tests ${BTB_ROOT}/tests/original_tail4_test.cpp)
target_link_libraries(btb_original_tail4_tests PRIVATE btb_recompiled_core)
add_test(NAME original_tail4 COMMAND btb_original_tail4_tests)
if(MSVC)
 target_compile_options(btb_original_tail4_tests PRIVATE /UNDEBUG)
else()
 target_compile_options(btb_original_tail4_tests PRIVATE -UNDEBUG)
endif()

# Eight large original-address activity/UI/resource/print source reconstructions.
add_executable(btb_original_closure8_tests ${BTB_ROOT}/tests/original_closure8_test.cpp)
target_link_libraries(btb_original_closure8_tests PRIVATE btb_recompiled_core)
add_test(NAME original_closure8 COMMAND btb_original_closure8_tests)
if(MSVC)
 target_compile_options(btb_original_closure8_tests PRIVATE /UNDEBUG)
else()
 target_compile_options(btb_original_closure8_tests PRIVATE -UNDEBUG)
endif()

# Eight additional original-address DirectInput and MSVC runtime source bodies.
add_executable(btb_original_input_crt8_tests ${BTB_ROOT}/tests/original_input_crt8_test.cpp)
target_link_libraries(btb_original_input_crt8_tests PRIVATE btb_recompiled_core)
add_test(NAME original_input_crt8 COMMAND btb_original_input_crt8_tests)
if(MSVC)
 target_compile_options(btb_original_input_crt8_tests PRIVATE /UNDEBUG)
else()
 target_compile_options(btb_original_input_crt8_tests PRIVATE -UNDEBUG)
endif()

# Three interconnected source-level retail gameplay state machines.
add_executable(btb_original_gameplay3_tests ${BTB_ROOT}/tests/original_gameplay3_test.cpp)
target_link_libraries(btb_original_gameplay3_tests PRIVATE btb_recompiled_core)
add_test(NAME original_gameplay3 COMMAND btb_original_gameplay3_tests)
if(MSVC)
 target_compile_options(btb_original_gameplay3_tests PRIVATE /UNDEBUG)
else()
 target_compile_options(btb_original_gameplay3_tests PRIVATE -UNDEBUG)
endif()

# Original Fireworks movie bank and editor setup entry points.
add_executable(btb_original_fireworks_start2_tests ${BTB_ROOT}/tests/original_fireworks_start2_test.cpp)
target_link_libraries(btb_original_fireworks_start2_tests PRIVATE btb_recompiled_core)
add_test(NAME original_fireworks_start2 COMMAND btb_original_fireworks_start2_tests)
if(MSVC)
 target_compile_options(btb_original_fireworks_start2_tests PRIVATE /UNDEBUG)
else()
 target_compile_options(btb_original_fireworks_start2_tests PRIVATE -UNDEBUG)
endif()

# Two original Maze initialization bodies; source/asset bindings tested on
# Linux C++26 and native Windows x86/x64.
add_executable(btb_original_maze_init2_tests ${BTB_ROOT}/tests/original_maze_init2_test.cpp)
target_link_libraries(btb_original_maze_init2_tests PRIVATE btb_recompiled_core)
add_test(NAME original_maze_init2 COMMAND btb_original_maze_init2_tests)
if(MSVC)
 target_compile_options(btb_original_maze_init2_tests PRIVATE /UNDEBUG)
else()
 target_compile_options(btb_original_maze_init2_tests PRIVATE -UNDEBUG)
endif()

# Original profile initialization / Fireworks editor drawing recovery.
add_executable(btb_original_profiles_fireworks2_tests ${BTB_ROOT}/tests/original_profiles_fireworks2_test.cpp)
target_link_libraries(btb_original_profiles_fireworks2_tests PRIVATE btb_recompiled_core)
add_test(NAME original_profiles_fireworks2 COMMAND btb_original_profiles_fireworks2_tests)
if(MSVC)
 target_compile_options(btb_original_profiles_fireworks2_tests PRIVATE /UNDEBUG)
else()
 target_compile_options(btb_original_profiles_fireworks2_tests PRIVATE -UNDEBUG)
endif()

# Four original drawing functions with explicit native video/physics boundary.
add_executable(btb_original_draw4_tests ${BTB_ROOT}/tests/original_draw4_test.cpp)
target_link_libraries(btb_original_draw4_tests PRIVATE btb_recompiled_core)
add_test(NAME original_draw4 COMMAND btb_original_draw4_tests)
if(MSVC)
 target_compile_options(btb_original_draw4_tests PRIVATE /UNDEBUG)
else()
 target_compile_options(btb_original_draw4_tests PRIVATE -UNDEBUG)
endif()

# Four source-level original game activity coordination routines.
add_executable(btb_original_activity4_tests ${BTB_ROOT}/tests/original_activity4_test.cpp)
target_link_libraries(btb_original_activity4_tests PRIVATE btb_recompiled_core)
add_test(NAME original_activity4 COMMAND btb_original_activity4_tests)
if(MSVC)
  target_compile_options(btb_original_activity4_tests PRIVATE /UNDEBUG)
else()
  target_compile_options(btb_original_activity4_tests PRIVATE -UNDEBUG)
endif()

# Original Maze, Spud Maze, Spud NPC and Herding movement coordination.
add_executable(btb_original_motion4_tests ${BTB_ROOT}/tests/original_motion4_test.cpp)
target_link_libraries(btb_original_motion4_tests PRIVATE btb_recompiled_core)
add_test(NAME original_motion4 COMMAND btb_original_motion4_tests)
if(MSVC)
 target_compile_options(btb_original_motion4_tests PRIVATE /UNDEBUG)
else()
 target_compile_options(btb_original_motion4_tests PRIVATE -UNDEBUG)
endif()

# Original 00409E00 Dino and 00428600 generic UI event coordinators.
add_executable(btb_original_coordinators2_tests ${BTB_ROOT}/tests/original_coordinators2_test.cpp)
target_link_libraries(btb_original_coordinators2_tests PRIVATE btb_recompiled_core)
add_test(NAME original_coordinators2 COMMAND btb_original_coordinators2_tests)
if(MSVC)
 target_compile_options(btb_original_coordinators2_tests PRIVATE /UNDEBUG)
else()
 target_compile_options(btb_original_coordinators2_tests PRIVATE -UNDEBUG)
endif()

# Retail source-model controllers 00415080 / 00425F40 / 004182B0.
add_executable(btb_original_gameplay3b_tests ${BTB_ROOT}/tests/original_gameplay3b_test.cpp)
target_link_libraries(btb_original_gameplay3b_tests PRIVATE btb_recompiled_core)
add_test(NAME original_gameplay3b COMMAND btb_original_gameplay3b_tests)
if(MSVC)
 target_compile_options(btb_original_gameplay3b_tests PRIVATE /UNDEBUG)
else()
 target_compile_options(btb_original_gameplay3b_tests PRIVATE -UNDEBUG)
endif()

# Original 0040A670 Dino and 00425160 Squirrel native initializer recovery.
add_executable(btb_original_init2_tests ${BTB_ROOT}/tests/original_init2_test.cpp)
target_link_libraries(btb_original_init2_tests PRIVATE btb_recompiled_core)
add_test(NAME original_init2 COMMAND btb_original_init2_tests)
if(MSVC)
 target_compile_options(btb_original_init2_tests PRIVATE /UNDEBUG)
else()
 target_compile_options(btb_original_init2_tests PRIVATE -UNDEBUG)
endif()

# Original 00419600 Herding retail-data and bitmap initialization.
add_executable(btb_original_herding_init_tests ${BTB_ROOT}/tests/original_herding_init_test.cpp)
target_link_libraries(btb_original_herding_init_tests PRIVATE btb_recompiled_core)
add_test(NAME original_herding_init COMMAND btb_original_herding_init_tests)
if(MSVC)
 target_compile_options(btb_original_herding_init_tests PRIVATE /UNDEBUG)
else()
 target_compile_options(btb_original_herding_init_tests PRIVATE -UNDEBUG)
endif()

# Source-level original 0040B7E0 Park Designer initializer.
add_executable(btb_original_park_init_tests ${BTB_ROOT}/tests/original_park_init_test.cpp)
target_link_libraries(btb_original_park_init_tests PRIVATE btb_recompiled_core)
add_test(NAME original_park_init COMMAND btb_original_park_init_tests)
if(MSVC)
 target_compile_options(btb_original_park_init_tests PRIVATE /UNDEBUG)
else()
 target_compile_options(btb_original_park_init_tests PRIVATE -UNDEBUG)
endif()

# Original CRT 004332F2 / 004333F6 / 004334D4.
add_executable(btb_original_crt_float3_tests ${BTB_ROOT}/tests/original_crt_float3_test.cpp)
target_link_libraries(btb_original_crt_float3_tests PRIVATE btb_recompiled_core)
add_test(NAME original_crt_float3 COMMAND btb_original_crt_float3_tests)
if(MSVC)
 target_compile_options(btb_original_crt_float3_tests PRIVATE /UNDEBUG)
else()
 target_compile_options(btb_original_crt_float3_tests PRIVATE -UNDEBUG)
endif()

# 00402700 / 0042A2C0 / 00438968 / 00438E39 original source model.
add_executable(btb_original_closure4_tests ${BTB_ROOT}/tests/original_closure4_test.cpp)
target_link_libraries(btb_original_closure4_tests PRIVATE btb_recompiled_core)
add_test(NAME original_closure4 COMMAND btb_original_closure4_tests)
if(MSVC)
 target_compile_options(btb_original_closure4_tests PRIVATE /UNDEBUG)
else()
 target_compile_options(btb_original_closure4_tests PRIVATE -UNDEBUG)
endif()

# Original CRT stdio 00432971 printf / 00431314 scanf.
add_executable(btb_original_stdio2_tests ${BTB_ROOT}/tests/original_stdio2_test.cpp)
target_link_libraries(btb_original_stdio2_tests PRIVATE btb_recompiled_core)
add_test(NAME original_stdio2 COMMAND btb_original_stdio2_tests)
if(MSVC)
 target_compile_options(btb_original_stdio2_tests PRIVATE /UNDEBUG)
else()
 target_compile_options(btb_original_stdio2_tests PRIVATE -UNDEBUG)
endif()

# Source-model final two original functions 0040E440 / 0041DB20.
add_executable(btb_original_last2_tests ${BTB_ROOT}/tests/original_last2_test.cpp)
target_link_libraries(btb_original_last2_tests PRIVATE btb_recompiled_core)
add_test(NAME original_last2 COMMAND btb_original_last2_tests)
if(MSVC)
 target_compile_options(btb_original_last2_tests PRIVATE /UNDEBUG)
else()
 target_compile_options(btb_original_last2_tests PRIVATE -UNDEBUG)
endif()

add_executable(btb_bink32_recovered_tests ${BTB_ROOT}/tests/bink32_recovered_test.cpp)
target_link_libraries(btb_bink32_recovered_tests PRIVATE btb_bink32_recovered)
btb_require_cpp26(btb_bink32_recovered_tests PRIVATE)
add_test(NAME bink32_recovered_export_source COMMAND btb_bink32_recovered_tests)
add_executable(btb_bink32_container_tests ${BTB_ROOT}/tests/bink32_container_test.cpp)
target_link_libraries(btb_bink32_container_tests PRIVATE btb_bink32_recovered)
btb_require_cpp26(btb_bink32_container_tests PRIVATE)
add_test(NAME bink32_native_container_index COMMAND btb_bink32_container_tests)
add_executable(btb_bink32_playback_tests ${BTB_ROOT}/tests/bink32_playback_test.cpp)
target_link_libraries(btb_bink32_playback_tests PRIVATE btb_bink32_recovered)
btb_require_cpp26(btb_bink32_playback_tests PRIVATE)
add_test(NAME bink32_native_pause_and_seek COMMAND btb_bink32_playback_tests)
# Source-derived real BinkDoFrame frame demultiplexer + entropy bitstream.
add_executable(btb_bink32_packet_decode_tests ${BTB_ROOT}/tests/bink32_packet_decode_test.cpp)
target_link_libraries(btb_bink32_packet_decode_tests PRIVATE btb_bink32_recovered)
btb_require_cpp26(btb_bink32_packet_decode_tests PRIVATE)
add_test(NAME bink32_source_frame_packets COMMAND btb_bink32_packet_decode_tests)
add_executable(btb_bink32_fixed_huffman_tests ${BTB_ROOT}/tests/bink32_fixed_huffman_test.cpp)
target_link_libraries(btb_bink32_fixed_huffman_tests PRIVATE btb_bink32_recovered)
btb_require_cpp26(btb_bink32_fixed_huffman_tests PRIVATE)
add_test(NAME bink32_original_fixed_huffman COMMAND btb_bink32_fixed_huffman_tests)
# Original 3000FCB0 tree construction and 300100E0 merge helper.
add_executable(btb_bink32_dynamic_huffman_tests ${BTB_ROOT}/tests/bink32_dynamic_huffman_test.cpp)
target_link_libraries(btb_bink32_dynamic_huffman_tests PRIVATE btb_bink32_recovered)
btb_require_cpp26(btb_bink32_dynamic_huffman_tests PRIVATE)
add_test(NAME bink32_original_dynamic_huffman COMMAND btb_bink32_dynamic_huffman_tests)
add_executable(btb_bink32_entropy_commands_tests ${BTB_ROOT}/tests/bink32_entropy_commands_test.cpp)
target_link_libraries(btb_bink32_entropy_commands_tests PRIVATE btb_bink32_recovered)
btb_require_cpp26(btb_bink32_entropy_commands_tests PRIVATE)
add_test(NAME bink32_original_value_run_commands COMMAND btb_bink32_entropy_commands_tests)
# Original Bink internal 0x30010980 signed-magnitude coefficient reader.
add_executable(btb_bink32_signed_coefficients_tests ${BTB_ROOT}/tests/bink32_signed_coefficients_test.cpp)
target_link_libraries(btb_bink32_signed_coefficients_tests PRIVATE btb_bink32_recovered)
btb_require_cpp26(btb_bink32_signed_coefficients_tests PRIVATE)
add_test(NAME bink32_original_signed_coefficients COMMAND btb_bink32_signed_coefficients_tests)
# Source-derived output pixels: Bink 2002 0x30012940 residual + reference
# frame, with 8x8 strides and true mod-256 arithmetic.
add_executable(btb_bink32_pixel_reconstruction_tests ${BTB_ROOT}/tests/bink32_pixel_reconstruction_test.cpp)
target_link_libraries(btb_bink32_pixel_reconstruction_tests PRIVATE btb_bink32_recovered)
btb_require_cpp26(btb_bink32_pixel_reconstruction_tests PRIVATE)
add_test(NAME bink32_original_pixel_residual COMMAND btb_bink32_pixel_reconstruction_tests)
# Bink32 original 0x300113CE partial 8x8 IDCT (DC-only column fast path).
add_executable(btb_bink32_inverse_transform_tests ${BTB_ROOT}/tests/bink32_inverse_transform_test.cpp)
target_link_libraries(btb_bink32_inverse_transform_tests PRIVATE btb_bink32_recovered)
btb_require_cpp26(btb_bink32_inverse_transform_tests PRIVATE)
add_test(NAME bink32_original_idct_dc_only COMMAND btb_bink32_inverse_transform_tests)
# Complete source-derived Bink x86 two-pass inverse transform at
# 0x30011350..0x30011715, 8x8 coeffs -> doubled 16x16 pixels.
add_executable(btb_bink32_full_idct_tests ${BTB_ROOT}/tests/bink32_full_idct_test.cpp)
target_link_libraries(btb_bink32_full_idct_tests PRIVATE btb_bink32_recovered)
btb_require_cpp26(btb_bink32_full_idct_tests PRIVATE)
add_test(NAME bink32_original_general_idct COMMAND btb_bink32_full_idct_tests)
# Full 16 original DLL quant tables -> native integer inverse transform.
add_executable(btb_bink32_original_quant_tests ${BTB_ROOT}/tests/bink32_original_quant_test.cpp)
target_link_libraries(btb_bink32_original_quant_tests PRIVATE btb_bink32_recovered)
btb_require_cpp26(btb_bink32_original_quant_tests PRIVATE)
add_test(NAME bink32_original_quant_to_idct COMMAND btb_bink32_original_quant_tests)
# Original 0x30011C4B jump table cases 1 and 3 (signed AC coefficients).
add_executable(btb_bink32_ac_nonzero_tests ${BTB_ROOT}/tests/bink32_ac_nonzero_test.cpp)
target_link_libraries(btb_bink32_ac_nonzero_tests PRIVATE btb_bink32_recovered)
btb_require_cpp26(btb_bink32_ac_nonzero_tests PRIVATE)
add_test(NAME bink32_original_nonzero_AC_branches COMMAND btb_bink32_ac_nonzero_tests)
add_executable(btb_bink32_ac_group4_tests ${BTB_ROOT}/tests/bink32_ac_group4_test.cpp)
target_link_libraries(btb_bink32_ac_group4_tests PRIVATE btb_bink32_recovered)
btb_require_cpp26(btb_bink32_ac_group4_tests PRIVATE)
add_test(NAME bink32_original_AC_group4 COMMAND btb_bink32_ac_group4_tests)
# Deterministic 50k-input malformed bitstream rollback and cursor invariants.
add_executable(btb_bink32_ac_group4_fuzz_tests ${BTB_ROOT}/tests/bink32_ac_group4_fuzz_test.cpp)
target_link_libraries(btb_bink32_ac_group4_fuzz_tests PRIVATE btb_bink32_recovered)
btb_require_cpp26(btb_bink32_ac_group4_fuzz_tests PRIVATE)
add_test(NAME bink32_original_AC_50k_invariants COMMAND btb_bink32_ac_group4_fuzz_tests)
# Actual 0x30011BF0..0x3001202A mutable pending command stack
# within ONE 2002 Bink coefficient magnitude-class pass.
add_executable(btb_bink32_ac_command_pass_tests ${BTB_ROOT}/tests/bink32_ac_command_pass_test.cpp)
target_link_libraries(btb_bink32_ac_command_pass_tests PRIVATE btb_bink32_recovered)
btb_require_cpp26(btb_bink32_ac_command_pass_tests PRIVATE)
add_test(NAME bink32_original_AC_mutable_pass COMMAND btb_bink32_ac_command_pass_tests)
# Original Bink second AC phase 0x30012030..0x300122E7, +/-1
# coefficient output and shared mutable pending command pointers.
add_executable(btb_bink32_ac_unit_pass_tests ${BTB_ROOT}/tests/bink32_ac_unit_pass_test.cpp)
target_link_libraries(btb_bink32_ac_unit_pass_tests PRIVATE btb_bink32_recovered)
btb_require_cpp26(btb_bink32_ac_unit_pass_tests PRIVATE)
add_test(NAME bink32_original_AC_unit_pass COMMAND btb_bink32_ac_unit_pass_tests)
# Real Bink 0x30011B10 outer mode scheduler for source-supported
# all 16 four-bit modes (0..15) with verified x86 read widths and final +/-1 pass.
add_executable(btb_bink32_ac_outer_levels_tests ${BTB_ROOT}/tests/bink32_ac_outer_levels_test.cpp)
target_link_libraries(btb_bink32_ac_outer_levels_tests PRIVATE btb_bink32_recovered)
btb_require_cpp26(btb_bink32_ac_outer_levels_tests PRIVATE)
add_test(NAME bink32_original_AC_outer_levels COMMAND btb_bink32_ac_outer_levels_tests)
# End-to-end subset: original AC mode 0..15 and pending-command passes
# -> all 16 original quant matrices -> integer iDCT -> 16x16 pixels.
add_executable(btb_bink32_ac_block_pixels_tests ${BTB_ROOT}/tests/bink32_ac_block_pixels_test.cpp)
target_link_libraries(btb_bink32_ac_block_pixels_tests PRIVATE btb_bink32_recovered)
btb_require_cpp26(btb_bink32_ac_block_pixels_tests PRIVATE)
add_test(NAME bink32_original_all_modes_to_pixels COMMAND btb_bink32_ac_block_pixels_tests)
# DC words produced by original 0x30010BF0 are handed off at
# original 0x3000F087 before the AC decoder. This test exercises
# real signed DC cursor semantics through all-mode AC and iDCT.
add_executable(btb_bink32_dc_stream_tests ${BTB_ROOT}/tests/bink32_dc_stream_test.cpp)
target_link_libraries(btb_bink32_dc_stream_tests PRIVATE btb_bink32_recovered)
btb_require_cpp26(btb_bink32_dc_stream_tests PRIVATE)
add_test(NAME bink32_original_DC_AC_quant_pixels COMMAND btb_bink32_dc_stream_tests)

add_executable(btb_bink32_block_modes_tests ${BTB_ROOT}/tests/bink32_block_modes_test.cpp)
target_link_libraries(btb_bink32_block_modes_tests PRIVATE btb_bink32_recovered)
btb_require_cpp26(btb_bink32_block_modes_tests PRIVATE)
add_test(NAME bink32_block_modes_pixels COMMAND btb_bink32_block_modes_tests)

# Stateful original Bink per-plane compositor: Y/U/V decode is atomic,
# reference-frame history resets on keyframes and rejects malformed motion.
add_executable(btb_bink32_video_planes_tests ${BTB_ROOT}/tests/bink32_video_planes_test.cpp)
target_link_libraries(btb_bink32_video_planes_tests PRIVATE btb_bink32_recovered)
btb_require_cpp26(btb_bink32_video_planes_tests PRIVATE)
add_test(NAME bink32_video_planes_history COMMAND btb_bink32_video_planes_tests)

# Native Bink source-ordered nine-bundle row scheduler, supporting
# original compressed Skip/Motion/Fill/Pattern/Raw luma subset.
add_executable(btb_bink32_simple_picture_tests ${BTB_ROOT}/tests/bink32_simple_picture_test.cpp)
target_link_libraries(btb_bink32_simple_picture_tests PRIVATE btb_bink32_recovered)
btb_require_cpp26(btb_bink32_simple_picture_tests PRIVATE)
add_test(NAME bink32_compressed_simple_picture_luma COMMAND btb_bink32_simple_picture_tests)

# Native SJPi original compressed frame assembly in A/Y/V/U order,
# decoded canonical Y/U/V/A, preserving complete reference history.
add_executable(btb_bink32_yuva_frame_tests ${BTB_ROOT}/tests/bink32_yuva_frame_test.cpp)
target_link_libraries(btb_bink32_yuva_frame_tests PRIVATE btb_bink32_recovered)
btb_require_cpp26(btb_bink32_yuva_frame_tests PRIVATE)
add_test(NAME bink32_original_sjpi_yuva_frame COMMAND btb_bink32_yuva_frame_tests)

# Native retail SJPi movie parity audit. Reads supplied original BIK
# and optionally independent raw yuva420p reference frames; no RAD
# DLL, FFmpeg binary or proprietary assets required to build.
add_executable(btb_bink32_retail_video_audit ${BTB_ROOT}/tools/native/bink32_retail_video_audit.cpp)
target_link_libraries(btb_bink32_retail_video_audit PRIVATE btb_bink32_recovered)
btb_require_cpp26(btb_bink32_retail_video_audit PRIVATE)

# DLL-free native movie frame -> packed RGB/DirectDraw presentation bridge.
# Exercise odd chroma, alpha, row pitch and invalid-surface atomic rejection.
add_executable(btb_bink32_surface_convert_tests ${BTB_ROOT}/tests/bink32_surface_convert_test.cpp)
target_link_libraries(btb_bink32_surface_convert_tests PRIVATE btb_bink32_recovered)
btb_require_cpp26(btb_bink32_surface_convert_tests PRIVATE)
add_test(NAME bink32_native_surface_convert COMMAND btb_bink32_surface_convert_tests)

# Native audio stream metadata + first compressed block envelope, no PCM
# claims. Explicitly protects buffer sizing prior to future DCT/RDFT decoding.
add_executable(btb_bink32_audio_packets_tests ${BTB_ROOT}/tests/bink32_audio_packets_test.cpp)
target_link_libraries(btb_bink32_audio_packets_tests PRIVATE btb_bink32_recovered)
btb_require_cpp26(btb_bink32_audio_packets_tests PRIVATE)
add_test(NAME bink32_audio_packet_preflight COMMAND btb_bink32_audio_packets_tests)

# Real Bink1 compressed audio -> spectral coefficients (not PCM yet).
add_executable(btb_bink32_audio_spectrum_tests ${BTB_ROOT}/tests/bink32_audio_spectrum_test.cpp)
target_link_libraries(btb_bink32_audio_spectrum_tests PRIVATE btb_bink32_recovered)
btb_require_cpp26(btb_bink32_audio_spectrum_tests PRIVATE)
add_test(NAME bink32_audio_spectrum_dct_rdft COMMAND btb_bink32_audio_spectrum_tests)

# Post-inverse-transform Bink1 1/16 overlap history, not yet PCM output.
add_executable(btb_bink32_audio_overlap_tests ${BTB_ROOT}/tests/bink32_audio_overlap_test.cpp)
target_link_libraries(btb_bink32_audio_overlap_tests PRIVATE btb_bink32_recovered)
btb_require_cpp26(btb_bink32_audio_overlap_tests PRIVATE)
add_test(NAME bink32_audio_overlap_history COMMAND btb_bink32_audio_overlap_tests)

# Fast dependency-free candidate native inverse transforms, verified
# analytically before any comparison to retail decoded PCM.
add_executable(btb_bink32_audio_inverse_tests ${BTB_ROOT}/tests/bink32_audio_inverse_test.cpp)
target_link_libraries(btb_bink32_audio_inverse_tests PRIVATE btb_bink32_recovered)
btb_require_cpp26(btb_bink32_audio_inverse_tests PRIVATE)
add_test(NAME bink32_audio_inverse_dct_rdft COMMAND btb_bink32_audio_inverse_tests)

# Experimental complete packet -> spectral -> inverse FFT -> overlap,
# including >1 transform per audio frame; never wired to live DirectSound.
add_executable(btb_bink32_audio_stream_tests ${BTB_ROOT}/tests/bink32_audio_stream_test.cpp)
target_link_libraries(btb_bink32_audio_stream_tests PRIVATE btb_bink32_recovered)
btb_require_cpp26(btb_bink32_audio_stream_tests PRIVATE)
add_test(NAME bink32_audio_stream_multiblock COMMAND btb_bink32_audio_stream_tests)

# Bounded signed PCM16 output contract, not yet selected by runtime.
add_executable(btb_bink32_audio_pcm_tests ${BTB_ROOT}/tests/bink32_audio_pcm_test.cpp)
target_link_libraries(btb_bink32_audio_pcm_tests PRIVATE btb_bink32_recovered)
btb_require_cpp26(btb_bink32_audio_pcm_tests PRIVATE)
add_test(NAME bink32_audio_pcm16_format COMMAND btb_bink32_audio_pcm_tests)

# Original BIK -> in-memory PCM and sample-accurate per-video-frame
# timestamps, deliberately not connected to the game sound runtime yet.
add_executable(btb_bink32_audio_movie_tests ${BTB_ROOT}/tests/bink32_audio_movie_test.cpp)
target_link_libraries(btb_bink32_audio_movie_tests PRIVATE btb_bink32_recovered)
btb_require_cpp26(btb_bink32_audio_movie_tests PRIVATE)
add_test(NAME bink32_audio_movie_timing COMMAND btb_bink32_audio_movie_tests)

# Audio-master timebase resilience to DirectSound cursor jitter and EOS.
add_executable(btb_bink32_audio_master_clock_tests ${BTB_ROOT}/tests/bink32_audio_master_clock_test.cpp)
target_link_libraries(btb_bink32_audio_master_clock_tests PRIVATE btb_bink32_recovered)
btb_require_cpp26(btb_bink32_audio_master_clock_tests PRIVATE)
add_test(NAME bink32_audio_master_clock COMMAND btb_bink32_audio_master_clock_tests)

# Read-only retail BIK audio packet audit. Optionally exports experimental
# interleaved F32 for external differential testing, never native playback.
add_executable(btb_bink32_native_audio_audit ${BTB_ROOT}/tools/native/bink32_native_audio_audit.cpp)
target_link_libraries(btb_bink32_native_audio_audit PRIVATE btb_bink32_recovered)
btb_require_cpp26(btb_bink32_native_audio_audit PRIVATE)

# Dump a decoded retail BIK frame as a P6 image for visual review without
# either binkw32.dll or FFmpeg at runtime. Original assets stay user-owned.
add_executable(btb_bink32_native_frame_dump ${BTB_ROOT}/tools/native/bink32_native_frame_dump.cpp)
target_link_libraries(btb_bink32_native_frame_dump PRIVATE btb_bink32_recovered)
btb_require_cpp26(btb_bink32_native_frame_dump PRIVATE)

# Original Bink1 sparse signed residue bitplane reader.
add_executable(btb_bink32_residue_tests ${BTB_ROOT}/tests/bink32_residue_test.cpp)
target_link_libraries(btb_bink32_residue_tests PRIVATE btb_bink32_recovered)
btb_require_cpp26(btb_bink32_residue_tests PRIVATE)
add_test(NAME bink32_original_residue_coefficients COMMAND btb_bink32_residue_tests)

# Original Bink1 8x8 coefficient tree and quantization-selector parser.
add_executable(btb_bink32_dct_coefficients_tests ${BTB_ROOT}/tests/bink32_dct_coefficients_test.cpp)
target_link_libraries(btb_bink32_dct_coefficients_tests PRIVATE btb_bink32_recovered)
btb_require_cpp26(btb_bink32_dct_coefficients_tests PRIVATE)
add_test(NAME bink32_original_dct_coefficients COMMAND btb_bink32_dct_coefficients_tests)

# Original Bink1 8x8 inverse transform and distinct intra/inter
# quantization matrices (full AC support).
add_executable(btb_bink32_dct_pixels_tests ${BTB_ROOT}/tests/bink32_dct_pixels_test.cpp)
target_link_libraries(btb_bink32_dct_pixels_tests PRIVATE btb_bink32_recovered)
btb_require_cpp26(btb_bink32_dct_pixels_tests PRIVATE)
add_test(NAME bink32_original_dct_pixels COMMAND btb_bink32_dct_pixels_tests)

option(BTB_BUILD_ENHANCED "Build the separate opt-in enhanced executable" OFF)
if(BTB_BUILD_ENHANCED)
    include("${BTB_ROOT}/enhanced/CMakeLists.txt")
endif()
