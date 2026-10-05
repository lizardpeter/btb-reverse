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
    src/fireworks_sequence.cpp
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

add_executable(btb_fireworks_sequence_tests
    tests/fireworks_sequence_test.cpp
)
target_link_libraries(btb_fireworks_sequence_tests PRIVATE btb_fireworks)
add_test(NAME fireworks_sequence COMMAND btb_fireworks_sequence_tests)

add_library(btb_golf
    src/golf_data.cpp
    src/golf_runtime.cpp
)
target_include_directories(btb_golf PUBLIC include)
target_compile_features(btb_golf PUBLIC cxx_std_20)

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
)
target_include_directories(btb_maze PUBLIC include)
target_compile_features(btb_maze PUBLIC cxx_std_20)

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


add_library(btb_herding
    src/herding_data.cpp
    src/herding_runtime.cpp
)
target_include_directories(btb_herding PUBLIC include)
target_compile_features(btb_herding PUBLIC cxx_std_20)

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


add_library(btb_park_designer
    src/park_designer_data.cpp
    src/park_designer_runtime.cpp
)
target_include_directories(btb_park_designer PUBLIC include)
target_compile_features(btb_park_designer PUBLIC cxx_std_20)

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
)
target_include_directories(btb_spud_skate PUBLIC include)
target_compile_features(btb_spud_skate PUBLIC cxx_std_20)

add_executable(btb_spud_skate_data_tests
    tests/spud_skate_data_test.cpp
)
target_link_libraries(btb_spud_skate_data_tests PRIVATE btb_spud_skate)
add_test(NAME spud_skate_data COMMAND btb_spud_skate_data_tests)


add_library(btb_spud_maze
    src/spud_maze_data.cpp
    src/spud_maze_runtime.cpp
)
target_include_directories(btb_spud_maze PUBLIC include)
target_compile_features(btb_spud_maze PUBLIC cxx_std_20)

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
)
target_include_directories(btb_squirrel PUBLIC include)
target_compile_features(btb_squirrel PUBLIC cxx_std_20)

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
