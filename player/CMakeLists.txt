
add_library(
    player
    player.cpp
)

target_include_directories(player
    PUBLIC
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/graphics
    ${CMAKE_SOURCE_DIR}/physics
    ${CMAKE_SOURCE_DIR}/weapon
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/glm
)

target_link_libraries(
    player
    PUBLIC
    SDL3::SDL3
    EnTT::EnTT
    physics)
