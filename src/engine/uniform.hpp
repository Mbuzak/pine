#ifndef PINE_UNIFORM
#define PINE_UNIFORM

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifdef __cplusplus
extern "C" {
#endif
#include "shader.h"
#ifdef __cplusplus
}
#endif

void uniform_int_send(GLuint, const char*, GLint);
void uniform_vec3f_send(GLuint, const char*, glm::vec3);
void uniform_mat4f_send(GLuint, const char*, glm::mat4&);

void shader_static_projection_send(ShaderStatic*, glm::mat4);
void shader_static_view_send(ShaderStatic*, glm::mat4);

#endif
