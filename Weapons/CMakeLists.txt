
add_library(
    Weapons
    Weapons.cpp
)

target_include_directories(Weapons PUBLIC
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/Graphics
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/World
    ${CMAKE_SOURCE_DIR}/player
    ${CMAKE_SOURCE_DIR}/include/glm
)

target_link_libraries(
    Weapons
    PUBLIC
    SDL3::SDL3
    EnTT::EnTT)
