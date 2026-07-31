#include "material.h"

void uniform_material_send(GLuint programID, const char* name, Material* material) {
	char name_ambient[32];
	char name_diffuse[32];
	char name_specular[32];
	char name_shininess[32];

	strcpy(name_ambient, name);
	strcat(name_ambient, "ambient");
	strcpy(name_diffuse, name);
	strcat(name_diffuse, "diffuse");
	strcpy(name_specular, name);
	strcat(name_specular, "specular");
	strcpy(name_shininess, name);
	strcat(name_shininess, "shininess");

	glUniform3fv(glGetUniformLocation(programID, name_ambient), 1, material->ambient.raw);
	glUniform3fv(glGetUniformLocation(programID, name_diffuse), 1, material->diffuse.raw);
	glUniform3fv(glGetUniformLocation(programID, name_specular), 1, material->specular.raw);
	glUniform1f(glGetUniformLocation(programID, name_shininess), material->shininess);
}
