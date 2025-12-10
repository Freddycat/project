#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <vector>
#include <glm/glm.hpp> 

#include "gizmos.h"
#include "vertex.h"

using std::vector;
using glm::vec3;

extern vector<vec3> base_triangle;
extern vector<vec3> base_cube_wireframe;
extern vector<Vertex> base_cube;
extern vector<vec3> base_circle;
extern vector<Vertex> base_sphere;
extern vector<vec3> base_cylinder;
extern vector<vec3> base_capsule;
extern vector<Vertex> base_plane;

void LoadBasics();

#endif