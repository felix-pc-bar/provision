cmake_minimum_required(VERSION 3.15)
project(provision)

set(CMAKE_CXX_STANDARD 17)

# =========================
# Fetch SDL2 from GitHub
# =========================

include(FetchContent)

FetchContent_Declare(
    SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-2.32.8
)

FetchContent_MakeAvailable(SDL2)

# =========================
# Main Executable
# =========================

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

# Link against SDL2 from FetchContent
target_link_libraries(provision PRIVATE SDL2::SDL2 SDL2::SDL2main)

# =========================
# Copy Content + SDL2.dll
# =========================

# Master content folder in source tree
set(MASTER_CONTENT_DIR "${CMAKE_SOURCE_DIR}/content")

# Copy content folder before build
add_custom_command(
    TARGET provision PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${MASTER_CONTENT_DIR}" "$<TARGET_FILE_DIR:provision>/content"
)

# Copy SDL2.dll if present (only on Windows)
if (WIN32)
    # The dll is generated when building SDL2 from source in the bin folder
    add_custom_command(
        TARGET provision PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "$<TARGET_FILE_DIR:SDL2::SDL2>/SDL2.dll"
                "$<TARGET_FILE_DIR:provision>/SDL2.dll"
    )
endif()
