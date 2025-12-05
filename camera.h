#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>

struct App;
struct Player;
struct PlayerCtx;
struct Input;
struct State;

struct Camera
{
    glm::vec3 handle; // if we want to manually move the camera later

    glm::vec2 window_center = glm::vec2(0.0f, 0.0f);

    glm::vec3 position = glm::vec3(2000.0f, 2000.0f, 2000.0f);
    glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f);
    float cam_zoom_offset = 2000.0;
    glm::vec3 offset = glm::vec3(cam_zoom_offset);
    glm::vec3 up_axis = glm::vec3(0, 0, 1); // was using -1 for ortho

    glm::vec3 center_world_pos;

    glm::vec3 forward;
    glm::vec3 right;
    glm::vec3 up;
    glm::vec3 map_forward;

    glm::dmat4 projection{1.0f};
    glm::dmat4 view{1.0f};
    glm::dmat4 inverse_view{1.0f};

    float center_x;
    float center_y;
    float origin_x;
    float origin_y;
    float zoom_amount = 0.3f;

    void SetCam();
    void CenterCam(Input &input, PlayerCtx &player);
};

#endif