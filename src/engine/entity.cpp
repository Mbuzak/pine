#include "entity.hpp"

glm::mat4 transform_model_compute(const Transform* transform) {
	glm::mat4 model(1.0);
	model = glm::translate(model, transform->pos);
	model = glm::rotate(model, glm::radians(transform->rot.x), glm::vec3(1.0, 0.0, 0.0));
	model = glm::rotate(model, glm::radians(transform->rot.y), glm::vec3(0.0, 1.0, 0.0));
	model = glm::rotate(model, glm::radians(transform->rot.z), glm::vec3(0.0, 0.0, 1.0));
	model = glm::scale(model, glm::vec3(transform->scale));
	return model;
}

Shape::Shape(Mesh *mesh, glm::vec3 pos) {
	this->transform = { .pos = pos, .rot = glm::vec3(0), .scale = 1.0 };
	this->mesh = mesh;
	this->material = { .ambient = {0.1, 0.1, 0.1}, .diffuse = {0.7, 0.3, 0.1},
		.specular = {0.3, 0.1, 0.1}, .shininess = 1.0 };
	this->texture_ = -1;
}

Shape::Shape(Mesh* mesh, glm::vec3 pos, GLuint texture_id) {
	this->transform = { .pos = pos, .rot = glm::vec3(0), .scale = 1.0 };
	this->mesh = mesh;
	this->material = { .ambient = {0.1, 0.1, 0.1}, .diffuse = {0.7, 0.3, 0.1},
		.specular = {0.3, 0.1, 0.1}, .shininess = 1.0 };
	this->texture_ = texture_id;
}

void lamp_init(Lamp* lamp, glm::vec3 pos) {
	LightPoint light = {.ambient = glm::vec3(0.1), .diffuse = {0.9, 0.9, 0.2},
		.specular = glm::vec3(0.5), .attenuation = glm::vec3(0.005)};
	lamp->light = light;
	lamp->transform = {.pos = pos, .rot = glm::vec3(0), .scale = 1.0};
}
