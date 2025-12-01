#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <glm/glm.hpp>

struct Material {
	glm::vec3 Ambient;
	glm::vec3 Diffuse;
	glm::vec3 Specular;
	float Shininess;

	/*Material() = default;

	Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess) {
		Ambient = ambient;
		Diffuse = diffuse;
		Specular = specular;
		Shininess = shininess;
	}*/
};

#endif
