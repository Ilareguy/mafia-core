cmake_minimum_required(VERSION 3.8)
project(mafia-core)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
message(STATUS "Using toolchain file: ${CMAKE_TOOLCHAIN_FILE}.")

########## Targets
add_executable(main src/main.cpp)

########## Dependencies
find_package(fmt CONFIG REQUIRED)
target_link_libraries(main PRIVATE fmt::fmt)
