cmake_minimum_required(VERSION 3.20.3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(3dOrbitalSimulator)

file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(3dOrbitalSimulator ${SOURCES})

target_include_directories(3dOrbitalSimulator PRIVATE include)
target_include_directories(3dOrbitalSimulator PRIVATE libs/assimp/include)
target_link_libraries(3dOrbitalSimulator PRIVATE
    debug "${CMAKE_CURRENT_SOURCE_DIR}/libs/assimp/lib/Debug/assimp-vc143-mtd.lib"
    optimized "${CMAKE_CURRENT_SOURCE_DIR}/libs/assimp/lib/Release/assimp-vc143-mt.lib"
)

target_include_directories(3dOrbitalSimulator PRIVATE libs/SDL3/include)
target_link_libraries(3dOrbitalSimulator PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL3/lib/x64/SDL3.lib")

target_include_directories(3dOrbitalSimulator PRIVATE libs/SDL3_ttf/include)
target_link_libraries(3dOrbitalSimulator PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL3_ttf/lib/x64/SDL3_ttf.lib")

target_include_directories(3dOrbitalSimulator PRIVATE libs/SDL3_mixer/include)
target_link_libraries(3dOrbitalSimulator PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2_mixer/lib/x64/SDL2_mixer.lib")

target_include_directories(3dOrbitalSimulator PRIVATE libs/freetype/include)
target_link_libraries(3dOrbitalSimulator PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/libs/freetype/release static/vs2015-2022/win64/freetype.lib")

add_library(glad STATIC libs/glad/src/glad.c)
target_include_directories(glad PUBLIC libs/glad/include)
target_link_libraries(3dOrbitalSimulator PRIVATE glad)

target_include_directories(3dOrbitalSimulator PRIVATE libs/glm)
include_directories("${PROJECT_SOURCE_DIR}/libs")
