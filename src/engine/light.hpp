#ifndef PINE_LIGHT
#define PINE_LIGHT

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "entity.hpp"
#include "uniform.hpp"

// Lamp class represents point light
struct Lamp {
	Lamp(Mesh*, glm::vec3, glm::vec3);

	glm::mat4 CalculateMatModel();
	void Display(GLuint);

	glm::vec3 position;
	glm::vec3 attenuation;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	Mesh* mesh = nullptr;
};

// Sun class represents directional lighting
struct Sun {
	glm::vec3 direction;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

Sun sun_init(glm::vec3);

void uniform_light_point_send(GLuint, std::string, Lamp*);
void uniform_light_directional_send(GLuint, std::string, Sun*);

#endif
