cmake_minimum_required(VERSION 3.15)
project(provision)

set(CMAKE_CXX_STANDARD 17)

# SDL2
set(SDL2_ROOT "${CMAKE_SOURCE_DIR}/external/SDL2-2.32.8")

include_directories("${SDL2_ROOT}/include")
link_directories("${SDL2_ROOT}/lib/x64")

add_executable(provision
    src/main.cpp
 "src/render/CPURenderer.cpp" 
 "src/render/CPURenderer.h" 
 "src/engTools.cpp" 
 "src/engTools.h" 
 "src/logic2d.cpp" 
 "src/logic2d.h" "src/engconfig.h" "src/engconfig.cpp" "src/import3d.cpp" "src/import3d.h")
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set_target_properties(provision PROPERTIES
    LINK_FLAGS "/SUBSYSTEM:CONSOLE"
)

target_link_libraries(provision SDL2main SDL2)