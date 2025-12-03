
add_library(
    World
    World.cpp
)

target_include_directories(World
    PUBLIC
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/Graphics
    ${CMAKE_SOURCE_DIR}/Weapons
    ${CMAKE_SOURCE_DIR}/camera
    ${CMAKE_SOURCE_DIR}/player
    ${CMAKE_SOURCE_DIR}/physics
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/glm
)

target_link_libraries(
    World
    PUBLIC
    SDL3::SDL3
    EnTT::EnTT
    Graphics
    helper)
