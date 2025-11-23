cmake_minimum_required(VERSION 3.10)

project(project)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_subdirectory(include/SDL)
add_subdirectory(include/entt)

add_subdirectory(graphics)
add_subdirectory(physics)
add_subdirectory(helper)
add_subdirectory(player)
add_subdirectory(world)
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
    camera.cpp
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
    include/json
    include/glad
)

target_link_libraries(project PUBLIC
    SDL3::SDL3
    EnTT::EnTT
    OpenGL::GL
    graphics
    physics
    helper
    world
    player
    weapon
    gui
)

# Copy shaders folder into the build directory
file(COPY ${CMAKE_SOURCE_DIR}/graphics/shaders DESTINATION ${CMAKE_BINARY_DIR})

# Copy maps folder
file(COPY ${CMAKE_SOURCE_DIR}/world/maps DESTINATION ${CMAKE_BINARY_DIR})

add_custom_target(copy_resources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/graphics/shaders
            ${CMAKE_BINARY_DIR}/shaders

    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/world/maps
            ${CMAKE_BINARY_DIR}/maps
)
