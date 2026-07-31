#ifndef RENDERED_HPP
#define RENDERED_HPP

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

void shader_rendered_init(Shader*);

void rendered_shape_render(Shader*, Shape*);

#endif
