#ifndef PINE_UNIFORM
#define PINE_UNIFORM

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

void uniform_int_send(GLuint, const char*, GLint);
void uniform_vec3f_send(GLuint, const char*, glm::vec3&);
void uniform_mat4f_send(GLuint, const char*, glm::mat4&);

#endif
