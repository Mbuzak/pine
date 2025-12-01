#include "mesh.hpp"


Shape::Shape(Model *model):
model_(model), position_(glm::vec3{0.0, 0.0, 0.0}),
angle_(glm::vec3{0.0, 0.0, 0.0}), texture_(nullptr) {
	material_ = Material{glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.70f, 0.27f, 0.08f), glm::vec3(0.25f, 0.13f, 0.08f), 1.0f};
}

Shape::Shape(Model *model, glm::vec3 position):
Shape(model) {
	position_ = position;
}

Shape::Shape(Model *model, glm::vec3 position, Texture *texture):
Shape(model, position) {
	texture_ = texture;
}

Shape::Shape(Model *model, glm::vec3 position, Material &material):
Shape(model, position) {
	material_ = material;
}

bool Shape::HasTexture() {
	return texture_ != nullptr;
}

glm::mat4 Shape::CalculateMatModel(int value) {
	glm::mat4 model(1.0);

	model = glm::translate(model, position_);
	model = glm::rotate(model, angle_.x, glm::vec3(1.0, 0.0, 0.0));
	model = glm::rotate(model, angle_.y, glm::vec3(0.0, 1.0, 0.0));
	model = glm::rotate(model, angle_.z, glm::vec3(0.0, 0.0, 1.0));

	if (value == 0)
		return model;
	
	model = glm::scale(model, glm::vec3(1.35, 1.04, 1.35));

	return model;
}

void Shape::Display(GLuint programID, int value) {
	glm::mat4 model = CalculateMatModel(value);

	glUniformMatrix4fv(glGetUniformLocation(programID, "matModel"), 1, GL_FALSE, glm::value_ptr(model));

	glm::mat3 matNormal = glm::transpose(glm::inverse(model));
	glUniformMatrix3fv(glGetUniformLocation(programID, "matNormal"), 1, GL_FALSE, glm::value_ptr(matNormal));

	SendMaterial(programID);

	glUniform1i(glGetUniformLocation(programID, "hasTex"), HasTexture());

	if (HasTexture())
		texture_->Send(programID);

	model_->Draw();
}

void Shape::DisplayOutline(GLuint program_id) {
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

void Shape::SendMaterial(GLuint programID) {
	glUniform3fv(glGetUniformLocation(programID, "my_material.ambient"), 1, glm::value_ptr(material_.ambient));
	glUniform3fv(glGetUniformLocation(programID, "my_material.diffuse"), 1, glm::value_ptr(material_.diffuse));
	glUniform3fv(glGetUniformLocation(programID, "my_material.specular"), 1, glm::value_ptr(material_.specular));
	glUniform1f(glGetUniformLocation(programID, "my_material.shininess"), material_.shininess);
}


Piece::Piece(int field_id, Model *model, Texture* texture):
Shape(model, glm::vec3(0.0, 0.1, 0.0), texture) {
	field_.push_back((char)'a' + (field_id % 8));
	field_.push_back((char)'8' - (field_id / 8));

	// std::cout << field_ << "\n";

	update_world_position();
}

std::string Piece::get_field() {
	return field_;
}

void Piece::update_field(std::string field) {
	field_ = field;
	update_world_position();
}

void Piece::update_world_position() {
	position_.x = (field_[0] - 'a' - 4) * 2.25 + 1.12;
	position_.z = ('8' - field_[1] - 4) * 2.25 + 1.12;

	// std::cout << field_ << "\n";
	// std::cout << position_.x << " " << position_.y << " " << position_.z << "\n";
}

void Piece::update_position() {
	int rank, file;

	rank = 4 + (int)((position_.z + 22.5) / 2.25) - 10;
	file = 4 + (int)((position_.x + 22.5) / 2.25) - 10;

	int field_id = rank * 8 + file;
	std::cout << "Update!\n" << "rank: " << rank << ", file: " << file << "\n";
	std::cout << "field_id: " << field_id << "\n";

	std::cout << (char)('a' + file) << (char)('8' - rank) << "\n";

	field_ = std::string() + (char)('a' + file) + (char)('8' - rank);
}
