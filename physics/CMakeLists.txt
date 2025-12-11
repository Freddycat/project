
add_library(
    physics
    colliders.cpp
)

target_include_directories(physics PUBLIC
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/glm
)

target_link_libraries(
    physics
    PUBLIC
    EnTT::EnTT)
