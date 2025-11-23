
add_library(
    world
    world.cpp
)

target_include_directories(world
    PUBLIC
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/graphics
    ${CMAKE_SOURCE_DIR}/weapon
    ${CMAKE_SOURCE_DIR}/camera
    ${CMAKE_SOURCE_DIR}/player
    ${CMAKE_SOURCE_DIR}/physics
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/glm
)

target_link_libraries(
    world
    PUBLIC
    SDL3::SDL3
    EnTT::EnTT
    graphics
    helper)
