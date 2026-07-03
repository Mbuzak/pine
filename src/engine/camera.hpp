#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

typedef struct {
	glm::mat4 projection;
	glm::mat4 view;

	glm::vec3 pos;
	glm::vec2 rot;

	float fov;
	float near_plane;
	float far_plane;
} Camera;

glm::vec3 camera_dir_compute(glm::vec2);
glm::vec3 camera_right_compute(glm::vec3);
glm::mat4 view_matrix_compute(glm::vec3, glm::vec2);
glm::mat4 projection_matrix_compute(Camera*, float);
void camera_send_uniform(GLuint, glm::vec3, glm::vec2);

#endif
