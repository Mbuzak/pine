#include "light.hpp"

Lamp::Lamp(Mesh* mesh, glm::vec3 position, glm::vec3 diffuse) {
	this->mesh = mesh;
	this->position = position;
	this->ambient = glm::vec3(0.1);
	this->diffuse = diffuse;
	this->specular = glm::vec3(0.5);
	this->attenuation = glm::vec3(0.1);
}

Lamp::Lamp(Mesh* mesh, glm::vec3 position, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 attenuation) {
	this->mesh = mesh;
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

	mesh_raw_draw(mesh);
}

Sun::Sun(glm::vec3 direction) {
	this->direction = direction;
	this->ambient = glm::vec3(0.4);
	this->diffuse = glm::vec3(0.2, 0.2, 0.5);
	this->specular = glm::vec3(0.1);
}

void uniform_light_point_send(GLuint program_id, std::string name, Lamp* light) {
	uniform_vec3f_send(program_id, (name + "ambient").c_str(), light->ambient);
	uniform_vec3f_send(program_id, (name + "specular").c_str(), light->specular);
	uniform_vec3f_send(program_id, (name + "attenuation").c_str(), light->attenuation);
	uniform_vec3f_send(program_id, (name + "position").c_str(), light->position);
}

void uniform_light_directional_send(GLuint program_id, std::string name, Sun* light) {
	uniform_vec3f_send(program_id, (name + "ambient").c_str(), light->ambient);
	uniform_vec3f_send(program_id, (name + "diffuse").c_str(), light->diffuse);
	uniform_vec3f_send(program_id, (name + "specular").c_str(), light->specular);
	uniform_vec3f_send(program_id, (name + "direction").c_str(), light->direction);
}
