
add_library(
    Graphics
    primitives.cpp
    Graphics.cpp
    shaders.cpp
    gizmos.cpp
)

target_include_directories(Graphics PUBLIC
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Models
    ${CMAKE_SOURCE_DIR}/Graphics/Models
    ${CMAKE_SOURCE_DIR}/World
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/glm
    ${CMAKE_SOURCE_DIR}/include/SDL
    ${CMAKE_SOURCE_DIR}/include/stb
)

target_link_libraries(Graphics PUBLIC SDL3::SDL3)
