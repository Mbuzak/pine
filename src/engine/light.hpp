#ifndef PINE_LIGHT
#define PINE_LIGHT

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include "uniform.hpp"

typedef struct {
	glm::vec3 position;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 attenuation;
} LightPoint;

// Sun class represents directional lighting
struct Sun {
	glm::vec3 direction;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

Sun sun_init(glm::vec3);

void uniform_light_point_send(GLuint, std::string, LightPoint*);
void uniform_light_directional_send(GLuint, std::string, Sun*);

#endif
