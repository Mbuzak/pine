#ifndef RENDERED_H
#define RENDERED_H

#include "cglm/struct.h"
#include <GL/glew.h>
#include "entity.h"
#include "shader.h"
#include "texture.h"

void shader_rendered_init(Shader*);

void rendered_shape_render(Shader*, Shape*);

#endif
