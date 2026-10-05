cmake_minimum_required(VERSION 3.20)
project(btb_reconstruction LANGUAGES CXX)

add_library(btb_dino
    src/dino_level.cpp
)

target_include_directories(btb_dino PUBLIC include)
target_compile_features(btb_dino PUBLIC cxx_std_20)

add_executable(btb_dino_tests
    tests/dino_level_test.cpp
)
target_link_libraries(btb_dino_tests PRIVATE btb_dino)

enable_testing()
add_test(NAME dino_level COMMAND btb_dino_tests)
