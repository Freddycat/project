#define GRAPHICS_PRIMITIVES_H
#ifdef GRAPHICS_PRIMITIVES_H

#include <vector>
#include <glm/glm.hpp> 

using std::vector;
using glm::vec3;

extern vector<vec3> base_triangle;

extern vector<vec3> base_cube_wireframe;
extern vector<vec3> base_cube;
extern vector<vec3> base_circle;
extern vector<vec3> base_sphere;
extern vector<vec3> base_cylinder;
extern vector<vec3> base_capsule;
extern vector<vec3> base_ground;

void LoadBasics();

#endif