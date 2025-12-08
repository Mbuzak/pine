#ifndef PINE_MATERIAL
#define PINE_MATERIAL

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include "uniform.hpp"

struct Material {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
};

void uniform_material_send(GLuint, std::string, Material*);

#endif
