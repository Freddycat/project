cmake_minimum_required(VERSION 3.10)

project(project)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_subdirectory(
  include/SDL
  include/imgui
)

find_package(OpenGL REQUIRED)

include_directories(
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/include/imgui
  ${CMAKE_SOURCE_DIR}/include/imgui/backends
)



add_executable(
  project
  app.cpp
  input.cpp
  gui.cpp
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
  PRIVATE
  include
  ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(project PRIVATE
  SDL3::SDL3
  OpenGL::GL
)