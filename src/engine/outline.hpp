#ifndef OUTLINE_HPP
#define OUTLINE_HPP

#include "cglm/struct.h"
#include <GL/glew.h>
#include "entity.hpp"

#ifdef __cplusplus
extern "C" {
#endif
#include "shader.h"
#include "texture.h"
#ifdef __cplusplus
}
#endif

void shader_outline_init(Shader*);
void shader_outline_render(Shader*, int, Transform*, Mesh*);

#endif
