#ifndef OUTLINE_HPP
#define OUTLINE_HPP

#include "cglm/struct.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
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
void shader_outline_render(Shader*, mat4s, int, Transform*, Mesh*);

#endif
