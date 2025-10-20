cmake_minimum_required(VERSION 3.20.3)

project(MystAnimator)

add_executable(app main.cpp)

add_subdirectory(libs/assimp)

target_include_directories(app PRIVATE libs/SDL3/include)
target_link_libraries(app PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL3/lib/x64/SDL3.lib")

target_include_directories(app PRIVATE libs/SDL3_ttf/include)
target_link_libraries(app PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL3_ttf/lib/x64/SDL3_ttf.lib")

target_include_directories(app PRIVATE libs/SDL3_mixer/include)
target_link_libraries(app PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2_mixer/lib/x64/SDL2_mixer.lib")

target_link_libraries(app PRIVATE assimp::assimp)

target_include_directories(app PRIVATE
    libs
    libs/Glad
    libs/glm
)