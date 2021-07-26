cmake_minimum_required(VERSION 3.8)
project(mafia-core)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
message(STATUS "Using toolchain file: ${CMAKE_TOOLCHAIN_FILE}.")

########## Defines
# 64 or 32 bits?
if ("${CMAKE_SIZEOF_VOID_P}" STREQUAL "8")
    add_definitions(-DMAFIA_64)
    set(MAFIA_DLL_NAME mafia_x64)
else ()
    add_definitions(-DMAFIA_32)
    set(MAFIA_DLL_NAME mafia)
endif ()

# Debug or Release?
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DMAFIA_DEBUG)
else ()
    add_definitions(-DMAFIA_RELEASE)
endif ()

########## Targets
add_library(mafia-types STATIC
        src/game_types/game_data_type.cpp
        src/game_types/rv_allocator_local.cpp
        src/game_types/game_data.cpp
        src/game_types/debug_value.cpp
        src/game_types/serialization.cpp
        src/game_types/rv_string.cpp
        src/game_types/param_archive.cpp
        src/game_types/v_table.h
        src/game_types/game_variable.cpp
        src/game_types/game_instruction.cpp
        src/game_types/vm_context.cpp
        src/game_types/game_var_space.cpp
        src/game_types/script_type_info.cpp
        src/game_types/sqf_script_type.cpp
        src/game_types/defs.cpp
        src/game_types/internal_object.cpp
        src/game_types/source_doc.cpp
        src/game_types/source_doc.h
        src/game_types/rv_pool_allocator.cpp
        src/game_types/rv_allocator.cpp
        src/game_types/game_state.cpp
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
        src/game_types/game_data/nothing.cpp
        src/game_types/game_data/code.cpp
        src/game_types/game_data/object.cpp
        )

add_library(mafia-containers STATIC
        src/containers/compact_array.cpp
        src/containers/rv_array.cpp
        src/containers/auto_array.cpp
        src/containers/stack_array.cpp
        src/containers/find_key_array.cpp
        src/containers/reference_array.cpp
        src/containers/map_string_to_class.cpp)

add_library(mafia-runtime-javascript MODULE
        src/runtime/javascript/javascript_runtime.cpp
        src/runtime/javascript/duktape/duktape.c)

add_library(${MAFIA_DLL_NAME} MODULE
        src/main.cpp
        src/mafia.cpp
        src/mafia_private.h
        src/arguments.cpp
        src/utils.cpp
        src/memory_utility.cpp
        src/allocator_info.h
        src/vector.cpp
        src/shared.h
        src/ref_count.cpp
        src/ref.cpp
        src/pair_hash.h
        src/unique_ref.cpp
        src/logging.cpp
        src/logging_private.h
        src/dll_handle.cpp
        src/dll_handle.tcc
        src/task.cpp
        src/rv_controller.cpp
        src/invoker.cpp
        src/sqf_functions.cpp
        src/loader.cpp
        src/sqf_register_functions.cpp
        src/mission_events.cpp
        src/runtime/mafia_errors.h
        src/runtime.cpp
        src/ssh_server.cpp
        src/ssh/command.cpp
        src/ssh/module.cpp
        src/task_executor.cpp
        src/synchronous_executor.cpp
        src/asynchronous_executor.cpp
        src/runtime/mafia_runtime.h
        src/runtime/mafia_module.h src/module.cpp src/module.h)

target_compile_definitions(${MAFIA_DLL_NAME} PRIVATE -DMAFIA_CORE)

########## Dependencies
# NOTE: Linked-to library DLLs need to be in Arma's *root directory*, and not in @mafia/.
# If you get a "module could not be found"/"not found"/"module not found", this might be why.
find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(dlfcn-win32 CONFIG REQUIRED)
find_package(libssh CONFIG REQUIRED)
find_package(cxxopts CONFIG REQUIRED)
find_package(Taskflow CONFIG REQUIRED)
find_package(Boost COMPONENTS filesystem REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(mafia-runtime-javascript PUBLIC
        mafia-types
        mafia-containers
        fmt::fmt
        nlohmann_json nlohmann_json::nlohmann_json
        ${Boost_LIBRARIES})

target_link_libraries(mafia-types PUBLIC
        Taskflow::Taskflow)

target_link_libraries(${MAFIA_DLL_NAME} PRIVATE
        mafia-types
        mafia-containers
        fmt::fmt
        spdlog::spdlog
        dlfcn-win32::dl
        ssh
        cxxopts::cxxopts
        Taskflow::Taskflow
        nlohmann_json nlohmann_json::nlohmann_json
        ${Boost_LIBRARIES})

########## Misc
set_target_properties(${MAFIA_DLL_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/arma/@mafia")
set_target_properties(mafia-runtime-javascript PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/arma/@mafia")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
