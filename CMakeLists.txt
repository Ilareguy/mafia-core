cmake_minimum_required(VERSION 3.8)
project(mafia-core)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
message(STATUS "Using toolchain file: ${CMAKE_TOOLCHAIN_FILE}.")

########## Targets
add_executable(main
        src/main.cpp
        src/memory_utility.cpp
        src/allocator_info.h
        src/vector.cpp
        src/shared.h
        src/game_types/game_state.cpp
        src/ref_count.cpp
        src/game_types/rv_pool_allocator.cpp
        src/game_types/rv_allocator.cpp
        src/ref.cpp
        src/game_types/game_data_type.cpp
        src/game_types/rv_allocator_local.cpp
        src/game_types/game_data.cpp
        src/game_types/debug_value.cpp
        src/game_types/serialization.cpp
        src/game_types/rv_string.cpp
        src/game_types/param_archive.cpp
        src/game_types/game_value.cpp
        src/game_types/game_data/number.cpp
        src/game_types/game_data/bool.cpp
        src/game_types/game_data/array.cpp
        src/game_types/game_data/String.cpp
        src/game_types/game_data/group.cpp
        src/game_types/game_data/config.cpp
        src/game_types/game_data/control.cpp
        src/game_types/game_data/display.cpp
        src/game_types/game_data/location.cpp
        src/game_types/game_data/script.cpp
        src/game_types/game_data/side.cpp
        src/game_types/game_data/rv_text.cpp
        src/game_types/game_data/team_member.cpp
        src/game_types/game_data/namespace.cpp
        src/game_types/v_table.h
        src/game_types/game_variable.cpp
        src/game_types/game_data/nothing.cpp
        src/game_types/game_data/code.cpp
        src/game_types/game_instruction.cpp
        src/game_types/vm_context.cpp
        src/game_types/game_var_space.cpp
        src/game_types/game_data/object.cpp
        src/game_types/script_type_info.cpp
        src/game_types/sqf_script_type.cpp
        src/game_types/defs.cpp
        src/game_types/internal_object.cpp
        src/game_types/source_doc.cpp
        src/game_types/source_doc.h
        src/pair_hash.h
        src/unique_ref.cpp
        src/containers/compact_array.cpp
        src/containers/rv_array.cpp
        src/containers/auto_array.cpp
        src/containers/stack_array.cpp
        src/containers/find_key_array.cpp
        src/containers/reference_array.cpp
        src/containers/map_string_to_class.cpp
        src/game_types/game_value_impl.h
        src/mafia.cpp)

########## Dependencies
find_package(fmt CONFIG REQUIRED)
target_link_libraries(main PRIVATE fmt::fmt)
