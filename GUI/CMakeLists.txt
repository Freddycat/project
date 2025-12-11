
add_library(
    GUI
    GUI.cpp
)

target_include_directories(GUI PUBLIC
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/Graphics
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/player
    ${CMAKE_SOURCE_DIR}/Weapons
    ${CMAKE_SOURCE_DIR}/World
    ${CMAKE_SOURCE_DIR}/include/glm
    ${CMAKE_SOURCE_DIR}/include/imgui
    ${CMAKE_SOURCE_DIR}/include/imgui/backends
)

target_link_libraries(
    GUI
    PRIVATE
    Graphics
    Weapons
    EnTT::EnTT)
