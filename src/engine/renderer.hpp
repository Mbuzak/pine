#ifndef PINE_RENDERER
#define PINE_RENDERER

#include <GL/glew.h>
#include "mesh.hpp"
#include "camera.hpp"
#include "entity.hpp"

#ifdef __cplusplus
extern "C" {
#endif
#include "shader.h"
#include "texture.h"
#ifdef __cplusplus
}
#endif

// Render colored solid model
void solid_render(GLuint, Transform*, Mesh*);
void shape_render(GLuint, Shape*);
void outline_render(GLuint, int, Transform*, Mesh*);

#endif
