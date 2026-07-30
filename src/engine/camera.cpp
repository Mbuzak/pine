#include "camera.hpp"

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
