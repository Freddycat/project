
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "Graphics.h"
#include "camera.h"
#include "gizmos.h"

namespace Draw {

namespace Shapes {

void DrawCubes(Graphics &graphics, Gizmos &gizmos, Camera &camera);
void DrawSpheres(Graphics &graphics, Gizmos &gizmos, Camera &camera);

} // namespace Shapes

namespace Lights {

void DrawLights(Graphics &graphics, Gizmos &gizmos, Camera &camera);

} // namespace Lights
} // namespace Draw