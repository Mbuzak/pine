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

Shape::Shape(Mesh *mesh) {
	transform = {glm::vec3(0), glm::vec3(0), 1};
	this->mesh = mesh;
	this->texture_ = -1;
	material = Material{{0.1f, 0.1f, 0.1f}, {0.70f, 0.27f, 0.08f}, {0.25f, 0.13f, 0.08f}, 1.0f};
}

Shape::Shape(Mesh *mesh, glm::vec3 position):
Shape(mesh) {
	transform.pos = position;
}

Shape::Shape(Mesh *mesh, glm::vec3 position, GLuint texture):
Shape(mesh, position) {
	texture_ = texture;
}

Piece::Piece(int field_id, Mesh *mesh, GLuint texture) {
	shape = Shape(mesh, glm::vec3(0.0, 0.1, 0.0), texture);
	field.push_back((char)'a' + (field_id % 8));
	field.push_back((char)'8' - (field_id / 8));

	update_world_position();
}

void Piece::update_world_position() {
	shape.transform.pos.x = (field[0] - 'a' - 4) * 2.25 + 1.12;
	shape.transform.pos.z = ('8' - field[1] - 4) * 2.25 + 1.12;
}

std::string square_compute(glm::vec3 pos) {
	int rank = 4 + (int)((pos.z + 22.5) / 2.25) - 10;
	int file = 4 + (int)((pos.x + 22.5) / 2.25) - 10;

	std::string field = std::string() + (char)('a' + file) + (char)('8' - rank);
	return field;
}

Shape terrain_init() {
	Shape shape;
	shape.transform = {{0, -0.1, 0}, glm::vec3(0), 80};
	shape.mesh = new Mesh();
	mesh_texture_init(shape.mesh, "square");
	shape.texture_ = texture_2d_init("grass.jpg");
	shape.material = Material{{0.1f, 0.1f, 0.1f}, {0.7f, 0.27f, 0.08f}, {0.25f, 0.13f, 0.08f}, 1.0f};
	return shape;
}


void lamp_init(Lamp* lamp, glm::vec3 pos, Mesh* mesh) {
	LightPoint light = {.ambient = glm::vec3(0.1), .diffuse = {0.9, 0.9, 0.2},
		.specular = glm::vec3(0.5), .attenuation = glm::vec3(0.005)};
	lamp->light = light;
	lamp->transform = {.pos = pos, .rot = glm::vec3(0), .scale = 1.0};
	lamp->mesh = mesh;
}
