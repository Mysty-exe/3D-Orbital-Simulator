cmake_minimum_required(VERSION 3.20)

project(3dOrbitalSimulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

# Put executable in build/ instead of build/Debug or Release
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR})

# Collect source files
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")

# Executable
add_executable(3dOrbitalSimulator ${SOURCES})

# Include directories
target_include_directories(3dOrbitalSimulator PRIVATE
    include
    libs
    libs/glm
    libs/glad/include
    libs/assimp/include
    libs/SDL3/include
    libs/freetype/include
)

# Glad
add_library(glad STATIC libs/glad/src/glad.c)
target_include_directories(glad PUBLIC libs/glad/include)
target_link_libraries(3dOrbitalSimulator PRIVATE glad)

# Assimp (Debug / Release)
target_link_libraries(3dOrbitalSimulator PRIVATE
    debug "${CMAKE_CURRENT_SOURCE_DIR}/libs/assimp/lib/Debug/assimp-vc143-mtd.lib"
    optimized "${CMAKE_CURRENT_SOURCE_DIR}/libs/assimp/lib/Release/assimp-vc143-mt.lib"
)

# SDL3
target_link_libraries(3dOrbitalSimulator PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL3/lib/x64/SDL3.lib"
)

# Freetype
target_link_libraries(3dOrbitalSimulator PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/libs/freetype/release static/vs2015-2022/win64/freetype.lib"
)

# Copy required DLLs after build
add_custom_command(TARGET 3dOrbitalSimulator POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_CURRENT_SOURCE_DIR}/libs/assimp/bin/Release/assimp-vc143-mt.dll"
    $<TARGET_FILE_DIR:3dOrbitalSimulator>
)

add_custom_command(TARGET 3dOrbitalSimulator POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL3/lib/x64/SDL3.dll"
    $<TARGET_FILE_DIR:3dOrbitalSimulator>
)
