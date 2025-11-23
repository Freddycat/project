
add_library(
    helper
    jsonHelper.cpp
)

target_include_directories(helper
    PUBLIC
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/graphics
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/world
    ${CMAKE_SOURCE_DIR}/player
    ${CMAKE_SOURCE_DIR}/include/glm
    ${CMAKE_SOURCE_DIR}/include/json
)

target_link_libraries(
    helper
    PUBLIC
    SDL3::SDL3
    EnTT::EnTT)
