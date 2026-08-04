#ifndef OUTLINE_H
#define OUTLINE_H

#include "cglm/struct.h"
#include <GL/glew.h>
#include "entity.h"
#include "shader.h"
#include "texture.h"

void shader_outline_init(Shader*);
void shader_outline_render(Shader*, int, Transform*, Mesh*);

#endif
