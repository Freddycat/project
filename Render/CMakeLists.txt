
add_library(
    Render
    Render.cpp
    drawLights.cpp
    drawShapes.cpp
)

target_include_directories(Render PUBLIC
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/Graphics
    ${CMAKE_SOURCE_DIR}/GUI
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/World
    ${CMAKE_SOURCE_DIR}/player
    ${CMAKE_SOURCE_DIR}/include/glm
    ${CMAKE_SOURCE_DIR}/include/imgui
    ${CMAKE_SOURCE_DIR}/include/imgui/backends
)

target_link_libraries(
    Render
    PUBLIC
    SDL3::SDL3
    EnTT::EnTT)
