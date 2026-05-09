#include "light.hpp"

int lamp_init(Lamp* lamp, Mesh* mesh, glm::vec3 position, glm::vec3 diffuse) {
	lamp->mesh = mesh;
	lamp->position = position;
	lamp->ambient = glm::vec3(0.1);
	lamp->diffuse = diffuse;
	lamp->specular = glm::vec3(0.5);
	lamp->attenuation = glm::vec3(0.1);
	return 1;
}

void lamp_render(Lamp* lamp, GLuint program_id) {
	glm::mat4 mat_model = position_model_compute(&lamp->position);
	glUniformMatrix4fv(glGetUniformLocation(program_id, "matModel"), 1, GL_FALSE, glm::value_ptr(mat_model));

	glm::mat3 matNormal = glm::transpose(glm::inverse(mat_model));
	glUniformMatrix3fv(glGetUniformLocation(program_id, "matNormal"), 1, GL_FALSE, glm::value_ptr(matNormal));

	mesh_texture_draw(lamp->mesh);
}

Sun sun_init(glm::vec3 dir) {
	Sun sun;
	sun.direction = dir;
	sun.ambient = glm::vec3(0.4);
	sun.diffuse = glm::vec3(0.2, 0.2, 0.5);
	sun.specular = glm::vec3(0.1);

	return sun;
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
