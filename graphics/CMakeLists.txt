
add_library(
  graphics
  graphics.cpp
  shaders.cpp
  gizmos.cpp
)

target_include_directories(graphics PUBLIC
${CMAKE_SOURCE_DIR}
${CMAKE_CURRENT_SOURCE_DIR}
${CMAKE_SOURCE_DIR}/include
${CMAKE_SOURCE_DIR}/include/glm
${CMAKE_SOURCE_DIR}/include/SDL
)

target_link_libraries(graphics PUBLIC SDL3::SDL3)