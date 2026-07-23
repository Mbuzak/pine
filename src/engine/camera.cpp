#include "camera.hpp"

void camera_light_init(CameraOrthographic* camera, glm::vec3 pos,
	glm::vec2 rot)
{
	camera->pos = pos;
	camera->rot = rot;
	camera->size = 18.0;
	camera->plane_near = 2.0;
	camera->plane_far = 35.5;
	camera->projection = glm::ortho(-camera->size, camera->size,
		-camera->size, camera->size, camera->plane_near, camera->plane_far);

	const glm::vec3 dir = camera_dir_compute(camera->rot);
	const glm::vec3 up(0.0, 1.0, 0.0);
	camera->view =  glm::lookAt(camera->pos, camera->pos + dir, up);
}

glm::vec3 camera_dir_compute(glm::vec2 rot) {
	glm::vec3 dir = { cos(glm::radians(rot.y)) * cos(glm::radians(rot.x)),
		sin(glm::radians(rot.x)),
		sin(glm::radians(rot.y)) * cos(glm::radians(rot.x)) };
	return glm::normalize(dir);
}

glm::vec3 camera_right_compute(glm::vec3 dir) {
	glm::vec3 up(0.0, 1.0, 0.0);
	return glm::normalize(glm::cross(up, dir));
}

glm::mat4 camera_view_compute(Camera* camera) {
	const glm::vec3 dir = camera_dir_compute(camera->rot);
	const glm::vec3 up(0.0, 1.0, 0.0);
	return glm::lookAt(camera->pos, camera->pos + dir, up);
}

void camera_send_uniform(GLuint program_id, glm::vec3 pos, glm::vec2 rot) {
	glUniform3fv(glGetUniformLocation(program_id, "cameraPos"), 1, &pos[0]);
}
