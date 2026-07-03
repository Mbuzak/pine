#include "camera.hpp"

void Camera::Update() {
	dir.x = cos(glm::radians(rot.y)) * cos(glm::radians(rot.x));
	dir.y = sin(glm::radians(rot.x));
	dir.z = sin(glm::radians(rot.y)) * cos(glm::radians(rot.x));
	dir = glm::normalize(dir);

	view = glm::lookAt(pos, pos + dir, glm::vec3(0, 1, 0));
	right = glm::normalize(glm::cross(glm::vec3(0, 1, 0), dir));
}

void Camera::SendUniform(GLuint programID) {
	glUniformMatrix4fv(glGetUniformLocation(programID, "matView"), 1, GL_FALSE, glm::value_ptr(view));
	glUniform3fv(glGetUniformLocation(programID, "cameraPos"), 1, &pos[0]);
}

void Camera::update_perspective(float aspect_ratio) {
	perspective = glm::perspective(glm::radians(fov), aspect_ratio, near_plane, far_plane);
}
