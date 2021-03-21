cmake_minimum_required(VERSION 3.8)
project(mafia-core)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
message(STATUS "Using toolchain file: ${CMAKE_TOOLCHAIN_FILE}.")

########## Targets
add_executable(main
        src/main.cpp
        src/memory_utility.cpp
        src/sqf_function.cpp
        src/allocator_info.h
        src/types.cpp
        src/containers.cpp
        src/vector.cpp
        src/shared.h
        src/game_types/game_state.cpp
        src/game_types/ref_count.cpp
        src/game_types/rv_pool_allocator.cpp
        src/game_types/rv_allocator.cpp
        src/game_types/ref.cpp
        src/game_types/game_type.h
        src/game_types/rv_allocator_local.cpp
        src/game_types/game_data.cpp
        src/game_types/debug_value.cpp
        src/game_types/serialization.cpp
        src/game_types/rv_string.cpp
        src/game_types/param_archive.cpp src/game_types/param_archive.h src/game_types/game_value.cpp src/game_types/game_value.h src/game_types/game_data/number.cpp src/game_types/game_data/number.h src/game_types/game_data/bool.cpp src/game_types/game_data/bool.h src/game_types/game_data/array.cpp src/game_types/game_data/array.h src/game_types/game_data/string.cpp src/game_types/game_data/string.h)

########## Dependencies
find_package(fmt CONFIG REQUIRED)
target_link_libraries(main PRIVATE fmt::fmt)
