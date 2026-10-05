cmake_minimum_required(VERSION 3.20)
project(btb_reconstruction LANGUAGES CXX)

add_library(btb_dino
    src/dino_level.cpp
    src/dino_runtime.cpp
    src/dino_character_animation.cpp
)

target_include_directories(btb_dino PUBLIC include)
target_compile_features(btb_dino PUBLIC cxx_std_20)

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

add_library(btb_fireworks
    src/fireworks_layout.cpp
)
target_include_directories(btb_fireworks PUBLIC include)
target_compile_features(btb_fireworks PUBLIC cxx_std_20)

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
