cmake_minimum_required(VERSION 3.10)

project(project)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_subdirectory(include/SDL)
add_subdirectory(include/entt)

add_subdirectory(graphics)
add_subdirectory(physics)
add_subdirectory(gui)
add_subdirectory(weapon)

find_package(OpenGL REQUIRED)

#include_directories(
#  ${CMAKE_SOURCE_DIR}/include
#  ${CMAKE_SOURCE_DIR}/include/imgui
#  ${CMAKE_SOURCE_DIR}/include/imgui/backends
#  ${CMAKE_SOURCE_DIR}/include/glad
#)

add_executable(
    project
    app.cpp
    app_start.cpp
    render.cpp
    input.cpp
    world.cpp
    camera.cpp
    player.cpp
    glad.c
    include/imgui/imgui.cpp
    include/imgui/imgui_draw.cpp
    include/imgui/imgui_tables.cpp
    include/imgui/imgui_widgets.cpp
    include/imgui/backends/imgui_impl_sdl3.cpp
    include/imgui/backends/imgui_impl_opengl3.cpp
)

target_include_directories(
    project
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    include
    include/entt
    include/glm
    include/imgui
    include/imgui/backends
    include/glad
)

target_link_libraries(project PUBLIC
    SDL3::SDL3
    EnTT::EnTT
    OpenGL::GL
    graphics
    physics
    weapon
    gui
)
