#ifndef MESH_HPP
#define MESH_HPP

#include "obj_loader.hpp"

#include <iostream>
#include <memory>
#include <vector>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "material.hpp"
#include "texture.hpp"

#include "stb_image.h"


class Mesh {
public:
	Material mMaterial;
	Texture *tx = NULL;
	//std::unique_ptr<Texture> texture;

	bool hasTex();

	//Mesh() = delete;
	//Mesh(const Mesh&) = delete;
	//Mesh& operator=(const Mesh&) = delete;
	Mesh() = default;
	Mesh(Material material, float x = 0.0, float y = 0.0, float z = 0.0);

	//~Mesh() = default;

	glm::mat4 calculateMatModel();
	glm::mat4 scaledMatModel(float x, float y, float z);

//private:
	glm::vec3 mPosition;
	glm::vec3 mAngle;
};

class LightMesh : public Mesh {
public:
	glm::vec3 Ambient;
	glm::vec3 Diffuse;
	glm::vec3 Specular;
	glm::vec3 Attenuation;
	glm::vec3 Position;

	bool mIsPoint;

	LightMesh() = default;
	LightMesh( glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 attenuation, glm::vec3 position, bool isPoint );
	glm::mat4 calculateMatModel();
};

#endif
