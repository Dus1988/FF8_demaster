cmake_minimum_required(VERSION 3.20)
project(ff8_demaster_android_port LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(ff8android_port STATIC
    src/archive_parity.cpp
    src/filesystem_android.cpp
    src/address_map.cpp
    src/runtime_target.cpp
)

target_include_directories(ff8android_port PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(ff8android_probe src/runtime_probe_cli.cpp)
target_link_libraries(ff8android_probe PRIVATE ff8android_port)
