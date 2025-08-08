cmake_minimum_required(VERSION 3.15)
project(provision)

set(CMAKE_CXX_STANDARD 17)

# SDL2 last 

set(SDL2_ROOT "${CMAKE_SOURCE_DIR}/external/SDL2-2.32.8")


# =========================================
# SDL2 - Fetch from source, build externally
# =========================================

# Choose version
set(SDL2_VERSION "release-2.32.8")
set(SDL2_BUILD_DIR "${CMAKE_SOURCE_DIR}/external/SDL2-build")

# Use FetchContent but force population/build in a fixed dir
FetchContent_Declare(
    SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG ${SDL2_VERSION}
    SOURCE_DIR ${SDL2_BUILD_DIR}/src
    BINARY_DIR ${SDL2_BUILD_DIR}/bin
)

FetchContent_MakeAvailable(SDL2)

# =========================================
# Main Executable
# =========================================
add_executable(provision
    src/main.cpp
    src/render/CPURenderer.cpp
    src/render/CPURenderer.h
    src/engTools.cpp
    src/engTools.h
    src/logic2d.cpp
    src/logic2d.h
    src/engconfig.h
    src/engconfig.cpp
    src/import3d.cpp
    src/import3d.h
    app_icon.rc
)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set_target_properties(provision PROPERTIES
    LINK_FLAGS "/SUBSYSTEM:WINDOWS"
)

# Path to your master content folder in the source tree
set(MASTER_CONTENT_DIR "${CMAKE_SOURCE_DIR}/content")

# Custom command: copy into the same dir as the executable for the current config

add_custom_command(
    TARGET provision PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${MASTER_CONTENT_DIR}" "$<TARGET_FILE_DIR:provision>/content"
)

target_link_libraries(provision SDL2main SDL2)