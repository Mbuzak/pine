#ifndef PINE_LIGHT
#define PINE_LIGHT

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "entity.hpp"
#include "uniform.hpp"

class Light {
public:
	Light() = default;
	Light(glm::vec3 diffuse, glm::vec3 specular);

public:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};


// Lamp class represents point light
class Lamp: public Light {
public:
	Lamp(Mesh*, glm::vec3, glm::vec3);
	Lamp(Mesh*, glm::vec3, glm::vec3, glm::vec3, glm::vec3);

	glm::mat4 CalculateMatModel();
	void Display(GLuint);

public:
	glm::vec3 position;
	glm::vec3 attenuation;

	Mesh* mesh = nullptr;
};

// Sun class represents directional lighting
class Sun: public Light {
public:
	Sun(glm::vec3 direction);

public:
	glm::vec3 direction;
};

void uniform_light_point_send(GLuint, std::string, Lamp*);
void uniform_light_directional_send(GLuint, std::string, Sun*);

#endif
