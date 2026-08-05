#ifndef RENDERED_H
#define RENDERED_H

#include "cglm/struct.h"
#include <GL/glew.h>
#include "entity.h"
#include "shader.h"
#include "texture.h"

void shader_rendered_init(Shader* shader);

void rendered_shapes_render(Shader* shader, Shape* shapes, int shape_count);

#endif
