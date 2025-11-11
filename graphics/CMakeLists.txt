
add_library(
  graphics
  graphics.cpp
  gizmos.cpp)

target_include_directories(graphics PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/include
)
