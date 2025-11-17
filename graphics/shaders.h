#define SHADERS_H
#ifdef SHADERS_H

#include <vector>
#include <glm/glm.hpp>

extern std::vector<glm::vec3> base_cube_wireframe;
extern std::vector<glm::vec3> base_cube_filled;
extern std::vector<glm::vec3> base_circle;
extern std::vector<glm::vec3> base_sphere;
extern std::vector<glm::vec3> base_cylinder;
extern std::vector<glm::vec3> base_capsule;

struct Graphics;
struct Gizmos;
struct Camera;


void SetupShaders(Graphics &graphics, Gizmos &gizmos, Camera &camera);

#endif