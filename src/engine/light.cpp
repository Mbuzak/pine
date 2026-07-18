#include "light.hpp"

Sun sun_init() {
	Sun sun = {.ambient = glm::vec3(0.2), .diffuse = glm::vec3(0.5),
		.specular = glm::vec3(1.0)};
	return sun;
}

void uniform_light_point_send(GLuint program_id, std::string name,
	LightPoint* light)
{
	uniform_vec3f_send(program_id, (name + "ambient").c_str(), light->ambient);
	uniform_vec3f_send(program_id, (name + "diffuse").c_str(), light->diffuse);
	uniform_vec3f_send(program_id, (name + "specular").c_str(), light->specular);
	uniform_vec3f_send(program_id, (name + "attenuation").c_str(), light->attenuation);
}

void uniform_light_directional_send(GLuint program_id, std::string name,
	Sun* light)
{
	uniform_vec3f_send(program_id, (name + "ambient").c_str(), light->ambient);
	uniform_vec3f_send(program_id, (name + "diffuse").c_str(), light->diffuse);
	uniform_vec3f_send(program_id, (name + "specular").c_str(), light->specular);
}
