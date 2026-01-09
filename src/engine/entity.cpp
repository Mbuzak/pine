#include "entity.hpp"

Shape::Shape(Mesh *mesh) {
	this->pos = glm::vec3(0.0);
	this->rot = glm::vec3(0.0);
	this->mesh = mesh;
	this->texture_ = -1;
	material_ = Material{glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.70f, 0.27f, 0.08f), glm::vec3(0.25f, 0.13f, 0.08f), 1.0f};
}

Shape::Shape(Mesh *mesh, glm::vec3 position):
Shape(mesh) {
	pos = position;
}

Shape::Shape(Mesh *mesh, glm::vec3 position, GLuint texture):
Shape(mesh, position) {
	texture_ = texture;
}

Shape::Shape(Mesh *mesh, glm::vec3 position, Material &material):
Shape(mesh, position) {
	material_ = material;
}

bool Shape::HasTexture() {
	return texture_ != -1;
}

glm::mat4 Shape::CalculateMatModel(int value) {
	glm::mat4 model(1.0);

	model = glm::translate(model, pos);
	model = glm::rotate(model, rot.x, glm::vec3(1.0, 0.0, 0.0));
	model = glm::rotate(model, rot.y, glm::vec3(0.0, 1.0, 0.0));
	model = glm::rotate(model, rot.z, glm::vec3(0.0, 0.0, 1.0));

	if (value == 0)
		return model;
	
	model = glm::scale(model, glm::vec3(1.35, 1.04, 1.35));

	return model;
}

void Shape::Display(GLuint programID, int value) {
	glm::mat4 model = CalculateMatModel(value);
	uniform_mat4f_send(programID, "matModel", model);

	glm::mat3 matNormal = glm::transpose(glm::inverse(model));
	glUniformMatrix3fv(glGetUniformLocation(programID, "matNormal"), 1, GL_FALSE, glm::value_ptr(matNormal));

	uniform_material_send(programID, "my_material.", &material_);

	glUniform1i(glGetUniformLocation(programID, "hasTex"), HasTexture());

	if (HasTexture())
		texture_2d_send(programID, texture_);

	mesh_texture_draw(mesh);
}

void Shape::DisplayOutline(GLuint program_id, int selected_id) {
	// Wylaczenie zapisu do bufora szablonowego
	glStencilMask(0x00);

	// Ustwianie funkcji testujacej
	// Obiekt bedzie rysowany tylko tam, gdzie stencil buffer nie jest rowny 1
	glStencilFunc(GL_NOTEQUAL, selected_id + 1, 0xFF);

	// Obliczenie nowej macierzy modelu (przeskalowanie obiektu)
	glUniform1i(glGetUniformLocation(program_id, "selected"), 1);
	glDisable(GL_DEPTH_TEST);
	Display(program_id, OUTLINE);
	glEnable(GL_DEPTH_TEST);
	glUniform1i(glGetUniformLocation(program_id, "selected"), 0);

	// Ponowne wlaczenie zapisu do bufora szablonowego
	// W szczegolnosci na potrzeby czyszczenia bufora
	// poleceniem glClear()
	glStencilMask(0xFF);
	glStencilFunc(GL_ALWAYS, 0, 0xFF);
}

Piece::Piece(int field_id, Mesh *mesh, GLuint texture) {
	shape = Shape(mesh, glm::vec3(0.0, 0.1, 0.0), texture);
	field.push_back((char)'a' + (field_id % 8));
	field.push_back((char)'8' - (field_id / 8));

	// std::cout << field_ << "\n";

	update_world_position();
}

void Piece::update_field(std::string f) {
	field = f;
	update_world_position();
}

void Piece::update_world_position() {
	shape.pos.x = (field[0] - 'a' - 4) * 2.25 + 1.12;
	shape.pos.z = ('8' - field[1] - 4) * 2.25 + 1.12;

	// std::cout << field << "\n";
	// std::cout << pos.x << " " << pos.y << " " << pos.z << "\n";
}

void Piece::update_position() {
	int rank = 4 + (int)((shape.pos.z + 22.5) / 2.25) - 10;
	int file = 4 + (int)((shape.pos.x + 22.5) / 2.25) - 10;

	int field_id = rank * 8 + file;
	std::cout << "Update!\n" << "rank: " << rank << ", file: " << file << "\n";
	std::cout << "field_id: " << field_id << "\n";

	std::cout << (char)('a' + file) << (char)('8' - rank) << "\n";

	field = std::string() + (char)('a' + file) + (char)('8' - rank);
}

Shape terrain_init() {
	Shape shape;
	shape.pos = {0.0, -0.1, 0.0};
	shape.rot = {0.0, 0.0, 0.0};
	shape.mesh = new Mesh();
	mesh_texture_init(shape.mesh, "square");
	shape.texture_ = texture_2d_init("grass.jpg");
	shape.material_ = Material{glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.70f, 0.27f, 0.08f), glm::vec3(0.25f, 0.13f, 0.08f), 1.0f};
	return shape;
}

void render(GLuint program_id, Shape* shape) {
	glm::mat4 model(1.0);
	model = glm::translate(model, shape->pos);
	model = glm::rotate(model, shape->rot.x, glm::vec3(1.0, 0.0, 0.0));
	model = glm::rotate(model, shape->rot.y, glm::vec3(0.0, 1.0, 0.0));
	model = glm::rotate(model, shape->rot.z, glm::vec3(0.0, 0.0, 1.0));
	model = glm::scale(model, {80, 80, 80});
	uniform_mat4f_send(program_id, "matModel", model);

	glm::mat3 matNormal = glm::transpose(glm::inverse(model));
	glUniformMatrix3fv(glGetUniformLocation(program_id, "matNormal"), 1, GL_FALSE, glm::value_ptr(matNormal));

	uniform_material_send(program_id, "my_material.", &shape->material_);

	glUniform1i(glGetUniformLocation(program_id, "hasTex"), 1);
	texture_2d_send(program_id, shape->texture_);

	mesh_texture_draw(shape->mesh);
}
