
add_library(
    Models
    meshes.cpp
    models.cpp
)

target_include_directories(Graphics PUBLIC
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/Graphics
)

#target_link_libraries(Models PUBLIC SDL3::SDL3)
