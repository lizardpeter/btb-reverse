cmake_minimum_required(VERSION 3.25)
project(btb_reconstruction LANGUAGES CXX)

add_library(btb_dino
    src/dino_level.cpp
    src/dino_runtime.cpp
    src/dino_character_animation.cpp
    src/dino_presentation.cpp
)

target_include_directories(btb_dino PUBLIC include)
target_compile_features(btb_dino PUBLIC cxx_std_26)

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
target_compile_features(btb_fireworks PUBLIC cxx_std_26)

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
target_compile_features(btb_golf PUBLIC cxx_std_26)

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
target_compile_features(btb_maze PUBLIC cxx_std_26)

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
target_compile_features(btb_maze_spud_controller_tests PRIVATE cxx_std_26)
add_test(NAME maze_spud_controller COMMAND btb_maze_spud_controller_tests)

add_executable(btb_maze_spud_neighbor_tests
    tests/maze_spud_neighbor_test.cpp
)
target_include_directories(btb_maze_spud_neighbor_tests PRIVATE include)
target_compile_features(btb_maze_spud_neighbor_tests PRIVATE cxx_std_26)
add_test(NAME maze_spud_neighbor COMMAND btb_maze_spud_neighbor_tests)

add_executable(btb_maze_spud_carry_tests
    tests/maze_spud_carry_test.cpp
)
target_include_directories(btb_maze_spud_carry_tests PRIVATE include)
target_compile_features(btb_maze_spud_carry_tests PRIVATE cxx_std_26)
add_test(NAME maze_spud_carry COMMAND btb_maze_spud_carry_tests)

add_executable(btb_maze_spud_reset_tests
    tests/maze_spud_reset_test.cpp
)
target_include_directories(btb_maze_spud_reset_tests PRIVATE include)
target_compile_features(btb_maze_spud_reset_tests PRIVATE cxx_std_26)
add_test(NAME maze_spud_reset COMMAND btb_maze_spud_reset_tests)


add_library(btb_herding
    src/herding_data.cpp
    src/herding_runtime.cpp
)
target_include_directories(btb_herding PUBLIC include)
target_compile_features(btb_herding PUBLIC cxx_std_26)

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
target_compile_features(btb_park_designer PUBLIC cxx_std_26)

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
target_compile_features(btb_spud_skate PUBLIC cxx_std_26)

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
target_compile_features(btb_spud_maze PUBLIC cxx_std_26)

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
target_compile_features(btb_squirrel PUBLIC cxx_std_26)

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
target_compile_features(btb_grand_opening PUBLIC cxx_std_26)

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
target_compile_features(btb_player_progress PUBLIC cxx_std_26)

add_executable(btb_player_progress_tests
    tests/player_progress_test.cpp
)
target_link_libraries(btb_player_progress_tests PRIVATE btb_player_progress)
add_test(NAME player_progress COMMAND btb_player_progress_tests)


add_library(btb_player_profiles
    src/player_profiles.cpp
)
target_include_directories(btb_player_profiles PUBLIC include)
target_compile_features(btb_player_profiles PUBLIC cxx_std_26)

add_executable(btb_player_profiles_tests
    tests/player_profiles_test.cpp
)
target_link_libraries(btb_player_profiles_tests PRIVATE btb_player_profiles)
add_test(NAME player_profiles COMMAND btb_player_profiles_tests)


add_library(btb_sound_manager
    src/sound_manager.cpp
)
target_include_directories(btb_sound_manager PUBLIC include)
target_compile_features(btb_sound_manager PUBLIC cxx_std_26)

add_executable(btb_sound_manager_tests
    tests/sound_manager_test.cpp
)
target_link_libraries(btb_sound_manager_tests PRIVATE btb_sound_manager)
add_test(NAME sound_manager COMMAND btb_sound_manager_tests)


add_library(btb_bitmap_registry
    src/bitmap_registry.cpp
)
target_include_directories(btb_bitmap_registry PUBLIC include)
target_compile_features(btb_bitmap_registry PUBLIC cxx_std_26)

add_executable(btb_bitmap_registry_tests
    tests/bitmap_registry_test.cpp
)
target_link_libraries(btb_bitmap_registry_tests PRIVATE btb_bitmap_registry)
add_test(NAME bitmap_registry COMMAND btb_bitmap_registry_tests)


add_executable(btb_display_manager_tests
    tests/display_manager_test.cpp
)
target_include_directories(btb_display_manager_tests PRIVATE include)
target_compile_features(btb_display_manager_tests PRIVATE cxx_std_26)
add_test(NAME display_manager COMMAND btb_display_manager_tests)


add_executable(btb_dsutil_tests
    tests/dsutil_test.cpp
)
target_include_directories(btb_dsutil_tests PRIVATE include)
target_compile_features(btb_dsutil_tests PRIVATE cxx_std_26)
add_test(NAME dsutil_layout COMMAND btb_dsutil_tests)


add_library(btb_printing_system
    src/printing_system.cpp
)
target_include_directories(btb_printing_system PUBLIC include)
target_compile_features(btb_printing_system PUBLIC cxx_std_26)

add_executable(btb_printing_system_tests
    tests/printing_system_test.cpp
)
target_link_libraries(btb_printing_system_tests PRIVATE btb_printing_system)
add_test(NAME printing_system COMMAND btb_printing_system_tests)


add_library(btb_bink_system
    src/bink_system.cpp
)
target_include_directories(btb_bink_system PUBLIC include)
target_compile_features(btb_bink_system PUBLIC cxx_std_26)

add_executable(btb_bink_system_tests
    tests/bink_system_test.cpp
)
target_link_libraries(btb_bink_system_tests PRIVATE btb_bink_system)
add_test(NAME bink_system COMMAND btb_bink_system_tests)


add_executable(btb_application_state_tests
    tests/application_state_test.cpp
)
target_include_directories(btb_application_state_tests PRIVATE include)
target_compile_features(btb_application_state_tests PRIVATE cxx_std_26)
add_test(NAME application_state COMMAND btb_application_state_tests)


add_executable(btb_application_runtime_tests
    tests/application_runtime_test.cpp
)
target_include_directories(btb_application_runtime_tests PRIVATE include)
target_compile_features(btb_application_runtime_tests PRIVATE cxx_std_26)
add_test(NAME application_runtime COMMAND btb_application_runtime_tests)


add_executable(btb_input_runtime_tests
    tests/input_runtime_test.cpp
)
target_include_directories(btb_input_runtime_tests PRIVATE include)
target_compile_features(btb_input_runtime_tests PRIVATE cxx_std_26)
add_test(NAME input_runtime COMMAND btb_input_runtime_tests)


add_executable(btb_window_runtime_tests
    tests/window_runtime_test.cpp
)
target_include_directories(btb_window_runtime_tests PRIVATE include)
target_compile_features(btb_window_runtime_tests PRIVATE cxx_std_26)
add_test(NAME window_runtime COMMAND btb_window_runtime_tests)


add_executable(btb_game_flow_tests
    tests/game_flow_test.cpp
)
target_include_directories(btb_game_flow_tests PRIVATE include)
target_compile_features(btb_game_flow_tests PRIVATE cxx_std_26)
add_test(NAME game_flow COMMAND btb_game_flow_tests)


add_executable(btb_front_end_ui_tests
    tests/front_end_ui_test.cpp
)
target_include_directories(btb_front_end_ui_tests PRIVATE include)
target_compile_features(btb_front_end_ui_tests PRIVATE cxx_std_26)
add_test(NAME front_end_ui COMMAND btb_front_end_ui_tests)


add_library(btb_front_end_ui_data
    src/front_end_ui_data.cpp
)
target_include_directories(btb_front_end_ui_data PUBLIC include)
target_compile_features(btb_front_end_ui_data PUBLIC cxx_std_26)

add_executable(btb_front_end_ui_data_tests
    tests/front_end_ui_data_test.cpp
)
target_link_libraries(btb_front_end_ui_data_tests PRIVATE btb_front_end_ui_data)
add_test(NAME front_end_ui_data COMMAND btb_front_end_ui_data_tests)


add_executable(btb_front_end_ui_runtime_tests
    tests/front_end_ui_runtime_test.cpp
)
target_include_directories(btb_front_end_ui_runtime_tests PRIVATE include)
target_compile_features(btb_front_end_ui_runtime_tests PRIVATE cxx_std_26)
add_test(NAME front_end_ui_runtime COMMAND btb_front_end_ui_runtime_tests)
