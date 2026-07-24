#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

typedef struct {
	glm::vec3 pos;
	glm::vec2 rot;
} Camera;

glm::vec3 direction_compute(glm::vec2);
glm::vec3 right_vector_compute(glm::vec3);

glm::mat4 camera_view_compute(Camera*);

#endif
