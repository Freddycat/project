
add_library(
    weapon
    weapon.cpp
)

target_include_directories(weapon PUBLIC
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/graphics
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/glm
)

target_link_libraries(
    weapon
    PUBLIC
    SDL3::SDL3
    EnTT::EnTT)
