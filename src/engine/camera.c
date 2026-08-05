#include "camera.h"

vec3s direction_compute(vec2s rot) {
	vec3s dir = {{cos(glm_rad(rot.y)) * cos(glm_rad(rot.x)), sin(glm_rad(rot.x)), sin(glm_rad(rot.y)) * cos(glm_rad(rot.x))}};
	glms_vec3_normalize(dir);
	return dir;
}

vec3s right_vector_compute(vec3s direction) {
	const vec3s UP = {{0.0, 1.0, 0.0}};
	return glms_vec3_normalize(glms_cross(UP, direction));
}

mat4s camera_view_compute(Camera* camera) {
	const vec3s dir = direction_compute(camera->rot);
	const vec3s up = {{0.0, 1.0, 0.0}};
	vec3s center = glms_vec3_add(camera->pos, dir);
	mat4s view = glms_lookat(camera->pos, center, up);
	return view;
}

void camera_move(Camera* camera, int keys_pressed[128], Shader* shaders, int shader_count) {
	vec3s dir = direction_compute(camera->rot);
	vec3s right = right_vector_compute(dir);
	const float speed = 0.5;
	if (keys_pressed[SDLK_w] == 1) {
		vec3s mydir = glms_vec3_scale(dir, speed);
		camera->pos = glms_vec3_add(camera->pos, mydir);
	}
	if (keys_pressed[SDLK_s] == 1) {
		vec3s mydir = glms_vec3_scale(dir, speed);
		camera->pos = glms_vec3_sub(camera->pos, mydir);
	}
	if (keys_pressed[SDLK_a] == 1) {
		vec3s mydir = glms_vec3_scale(right, speed);
		camera->pos = glms_vec3_add(camera->pos, mydir);
	}
	if (keys_pressed[SDLK_d] == 1) {
		vec3s mydir = glms_vec3_scale(right, speed);
		camera->pos = glms_vec3_sub(camera->pos, mydir);
	}

	mat4s view = camera_view_compute(camera);
	uniform_vec3_send(shaders, shader_count, UNIFORM_CAMERA_COORDS, camera->pos);
	uniform_mat4_send(shaders, shader_count, UNIFORM_VIEW, view);
}
