
add_library(
    Models
    meshes.cpp
    models.cpp
)

target_include_directories(Models PUBLIC
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/Graphics
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/assimp/include
)

target_link_libraries(
    Models
    PUBLIC
    assimp)
