#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "entity.hpp"

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
	Lamp(Model *model, glm::vec3 position, glm::vec3 diffuse);
	Lamp(Model* model, glm::vec3 position, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 attenuation);

	glm::mat4 CalculateMatModel();
	void Display(GLuint program_id);

public:
	glm::vec3 position;
	glm::vec3 attenuation;

	Model* model = nullptr;
};


// Sun class represents directional lighting
class Sun: public Light {
public:
	Sun(glm::vec3 direction);

	void SendUniform(GLuint program_id);

public:
	glm::vec3 direction;
};

#endif
