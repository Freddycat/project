cmake_minimum_required(VERSION 3.10)

project(project)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE BOOL "" FORCE)
add_subdirectory(include/assimp)

add_subdirectory(include/SDL)
add_subdirectory(include/entt)

add_subdirectory(Graphics)
add_subdirectory(GUI)
add_subdirectory(physics)
add_subdirectory(helper)
add_subdirectory(player)
add_subdirectory(World)
add_subdirectory(Weapons)

find_package(OpenGL REQUIRED)

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
    assimp
    Graphics
    GUI
    physics
    helper
    World
    player
    Weapons
)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(SHADER_OUT_DIR ${CMAKE_BINARY_DIR}/Debug/shaders)
    set(MAP_OUT_DIR ${CMAKE_BINARY_DIR}/Debug/maps)

else()
    set(SHADER_OUT_DIR ${CMAKE_BINARY_DIR}/shaders)
    set(MAP_OUT_DIR ${CMAKE_BINARY_DIR}/maps)
endif()

add_custom_target(copy_resources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/Graphics/shaders
    ${SHADER_OUT_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/World/maps
    ${MAP_OUT_DIR}
)
