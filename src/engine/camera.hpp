#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

typedef struct {
	glm::vec3 pos;
	glm::vec2 rot;
} Camera;

glm::vec3 camera_dir_compute(glm::vec2);
glm::vec3 camera_right_compute(glm::vec3);
glm::mat4 camera_view_compute(Camera*);
void camera_send_uniform(GLuint, glm::vec3, glm::vec2);

#endif
