#define SHADERS_H
#ifdef SHADERS_H

#include <vector>
#include <glm/glm.hpp>
#include "primitives.h"

struct Graphics;
struct Gizmos;
struct Camera;

void SetupShaders(Graphics &graphics, Gizmos &gizmos, Camera &camera);

#endif