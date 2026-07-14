#ifndef PINE_LIGHT
#define PINE_LIGHT

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "entity.hpp"
#include "uniform.hpp"

// Lamp class represents point light
struct Lamp {
	Transform transform;
	Mesh* mesh;
	glm::vec3 attenuation;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

void lamp_init(Lamp*, glm::vec3, Mesh*);

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
