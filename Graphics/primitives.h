#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <glm/glm.hpp>
#include <vector>

// #include "gizmos.h"
#include "vertex.h"

using glm::vec3;
using std::vector;

extern vector<vec3> base_triangle;
extern vector<vec3> base_cube_wireframe;
extern vector<Vertex> base_cube;
extern vector<vec3> base_circle;
extern vector<Vertex> base_sphere;
extern vector<vec3> base_cylinder;
extern vector<vec3> base_capsule;
extern vector<Vertex> base_plane;

extern size_t max_players;

void LoadBasics();

#endif