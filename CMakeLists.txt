cmake_minimum_required(VERSION 3.15)
project(provision)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

# =========================================
# SDL2 - Fetch from source, build externally
# =========================================

# Choose version
set(SDL2_VERSION "release-2.30.8")
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

# Link SDL2
target_link_libraries(provision PRIVATE SDL2::SDL2 SDL2::SDL2main)

# Explicitly add SDL includes so IntelliSense sees them
target_include_directories(provision PRIVATE
    $<TARGET_PROPERTY:SDL2::SDL2,INTERFACE_INCLUDE_DIRECTORIES>
)

# =========================================
# Copy content folder
# =========================================
set(MASTER_CONTENT_DIR "${CMAKE_SOURCE_DIR}/content")

add_custom_command(
    TARGET provision PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${MASTER_CONTENT_DIR}" "$<TARGET_FILE_DIR:provision>/content"
)

# =========================================
# Copy SDL2.dll (Windows only)
# =========================================
if (WIN32)
    # Ensure SDL is built before we copy its DLL
    add_dependencies(provision SDL2::SDL2)

    add_custom_command(
        TARGET provision PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "$<TARGET_FILE:SDL2::SDL2>"
                "$<TARGET_FILE_DIR:provision>/SDL2.dll"
    )
endif()

