#include "material.hpp"

void uniform_material_send(GLuint programID, std::string name, Material* material) {
	uniform_vec3f_send(programID, (name + "ambient").c_str(), material->ambient);
	uniform_vec3f_send(programID, (name + "diffuse").c_str(), material->diffuse);
	uniform_vec3f_send(programID, (name + "specular").c_str(), material->specular);
	glUniform1f(glGetUniformLocation(programID, (name + "shininess").c_str()), material->shininess);
}
