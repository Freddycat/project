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

    glm::vec3 position = glm::vec3(500.0f, 500.0f, 500.0f);
    glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 offset = glm::vec3(500.0f, 500.0f, 500.0f);
    glm::vec3 up_axis = glm::vec3(0, 0, -1);

    glm::vec3 forward;
    glm::vec3 right;
    glm::vec3 up;
    glm::vec3 map_up;
    glm::vec3 map_forward;

    glm::mat4 projection{1.0f};
    glm::mat4 view{1.0f};
    glm::mat4 inverse_view{1.0f};

    float center_x;
    float center_y;
    float origin_x;
    float origin_y;
    float zoom_amount = 0.3f;

    void SetCam();
    void CenterCam(Input &input, PlayerCtx &player);
};

#endif