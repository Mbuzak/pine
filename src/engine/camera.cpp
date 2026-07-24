#include "camera.hpp"

glm::vec3 direction_compute(glm::vec2 rot) {
	glm::vec3 dir = { cos(glm::radians(rot.y)) * cos(glm::radians(rot.x)),
		sin(glm::radians(rot.x)),
		sin(glm::radians(rot.y)) * cos(glm::radians(rot.x)) };
	return glm::normalize(dir);
}

glm::vec3 right_vector_compute(glm::vec3 direction) {
	const glm::vec3 UP = { 0.0, 1.0, 0.0 };
	return glm::normalize(glm::cross(UP, direction));
}

glm::mat4 camera_view_compute(Camera* camera) {
	const glm::vec3 dir = direction_compute(camera->rot);
	const glm::vec3 up(0.0, 1.0, 0.0);
	return glm::lookAt(camera->pos, camera->pos + dir, up);
}
