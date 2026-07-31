#include "light.h"

Sun sun_init() {
	Sun sun = {.ambient = {{0.2, 0.2, 0.2}}, .diffuse = {{0.5, 0.5, 0.5}},
		.specular = {{1.0, 1.0, 1.0}}};
	return sun;
}

void uniform_light_point_send(GLuint program_id, const char* name,
	LightPoint* light)
{
	char name_ambient[32];
	char name_diffuse[32];
	char name_specular[32];
	char name_attenuation[32];

	strcpy(name_ambient, name);
	strcat(name_ambient, "ambient");
	strcpy(name_diffuse, name);
	strcat(name_diffuse, "diffuse");
	strcpy(name_specular, name);
	strcat(name_specular, "specular");
	strcpy(name_attenuation, name);
	strcat(name_attenuation, "attenuation");

	glUniform3fv(glGetUniformLocation(program_id, name_ambient), 1, light->ambient.raw);
	glUniform3fv(glGetUniformLocation(program_id, name_diffuse), 1, light->diffuse.raw);
	glUniform3fv(glGetUniformLocation(program_id, name_specular), 1, light->specular.raw);
	glUniform3fv(glGetUniformLocation(program_id, name_attenuation), 1, light->attenuation.raw);;
}

void uniform_light_directional_send(GLuint program_id, const char* name,
	Sun* light)
{
	char name_ambient[32];
	char name_diffuse[32];
	char name_specular[32];

	strcpy(name_ambient, name);
	strcat(name_ambient, "ambient");
	strcpy(name_diffuse, name);
	strcat(name_diffuse, "diffuse");
	strcpy(name_specular, name);
	strcat(name_specular, "specular");

	glUniform3fv(glGetUniformLocation(program_id, name_ambient), 1, light->ambient.raw);
	glUniform3fv(glGetUniformLocation(program_id, name_diffuse), 1, light->diffuse.raw);
	glUniform3fv(glGetUniformLocation(program_id, name_specular), 1, light->specular.raw);
}
