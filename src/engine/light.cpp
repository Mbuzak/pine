#include "light.hpp"

Lamp::Lamp(Model *model, glm::vec3 position, glm::vec3 diffuse) {
	this->model = model;
	this->position = position;
	this->ambient = glm::vec3(0.1);
	this->diffuse = diffuse;
	this->specular = glm::vec3(0.5);
	this->attenuation = glm::vec3(0.1);
}

Lamp::Lamp(Model* model, glm::vec3 position, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 attenuation) {
	this->model = model;
	this->position = position;
	this->ambient = glm::vec3(0.1);
	this->diffuse = diffuse;
	this->specular = specular;
	this->attenuation = attenuation;
}

glm::mat4 Lamp::CalculateMatModel() {
	glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
	return model;
}

void Lamp::Display(GLuint program_id) {
	glm::mat4 mat_model = CalculateMatModel();
	glUniformMatrix4fv(glGetUniformLocation(program_id, "matModel"), 1, GL_FALSE, glm::value_ptr(mat_model));

	glm::mat3 matNormal = glm::transpose(glm::inverse(mat_model));
	glUniformMatrix3fv(glGetUniformLocation(program_id, "matNormal"), 1, GL_FALSE, glm::value_ptr(matNormal));

	model->Draw();
}

Sun::Sun(glm::vec3 direction) {
	this->direction = direction;
	this->ambient = glm::vec3(0.4);
	this->diffuse = glm::vec3(0.2, 0.2, 0.5);
	this->specular = glm::vec3(0.1);
}

void Sun::SendUniform(GLuint program_id) {
	glUniform3fv(glGetUniformLocation(program_id, "sun.ambient"), 1, glm::value_ptr(ambient));
	glUniform3fv(glGetUniformLocation(program_id, "sun.diffuse"), 1, glm::value_ptr(diffuse));
	glUniform3fv(glGetUniformLocation(program_id, "sun.specular"), 1, glm::value_ptr(specular));
	glUniform3fv(glGetUniformLocation(program_id, "sun.direction"), 1, glm::value_ptr(direction));
}
