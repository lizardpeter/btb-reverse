# Separate opt-in product; no enhanced source is linked into btb_game.
add_executable(btb_game_enhanced WIN32
    ${BTB_ROOT}/runtime/game_main.cpp
    ${BTB_ROOT}/runtime/game_win32.cpp)
btb_require_cpp26(btb_game_enhanced PRIVATE)
target_compile_definitions(btb_game_enhanced PRIVATE BTB_ENHANCED_BUILD=1)
target_link_libraries(btb_game_enhanced PRIVATE btb_game_session btb_recompiled_core)
if(WIN32)
    target_compile_definitions(btb_game_enhanced PRIVATE UNICODE _UNICODE)
    target_link_libraries(btb_game_enhanced PRIVATE
        btb_original_win32_platform user32 gdi32 msimg32 comdlg32 shell32)
endif()
add_test(NAME enhanced_baseline_smoke COMMAND btb_game_enhanced --recompiled-smoke)
