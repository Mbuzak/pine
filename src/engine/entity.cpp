#include "entity.hpp"

mat4s transform_model_compute(const Transform* transform) {
	mat4s model = glms_translate(glms_mat4_identity(), transform->pos);
	model = glms_rotate_x(model, glm_rad(transform->rot.x));
	model = glms_rotate_y(model, glm_rad(transform->rot.y));
	model = glms_rotate_z(model, glm_rad(transform->rot.z));
	model = glms_scale(model, glms_vec3_fill(transform->scale));
	return model;
}

void shape_init(Shape* shape, vec3s pos, Mesh* mesh, GLuint texture_id) {
	shape->transform = { .pos = pos, .rot = glms_vec3_zero(), .scale = 1.0 };
	shape->mesh = mesh;
	shape->material = { .ambient = {0.1, 0.1, 0.1}, .diffuse = {0.7, 0.3, 0.1},
		.specular = {0.3, 0.1, 0.1}, .shininess = 1.0 };
	shape->texture_ = texture_id;
}

void lamp_init(Lamp* lamp, vec3s pos) {
	LightPoint light = {.ambient = {0.1, 0.1, 0.1}, .diffuse = {0.9, 0.9, 0.2},
		.specular = {0.5, 0.5, 0.5}, .attenuation = {0.005, 0.005, 0.005}};
	lamp->light = light;
	lamp->transform = {.pos = pos, .rot = glms_vec3_zero(), .scale = 1.0};
}
