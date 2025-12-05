#ifndef MODEL_HPP
#define MODEL_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <vector>
#include "material.hpp"
#include "texture.hpp"
#include "obj_loader.hpp"

// Skybox model on scene
class Skybox {
public:
	void init();
	void draw();

private:
	enum Buffer {
		POSITIONS,
		INDICES,
		COUNT
	};

	GLuint vao;
	GLuint vbos[COUNT];
};

// Visible model on scene
class Model {
public:
	Model(std::string);

	void Draw();

private:
	GLuint vao;
	GLuint vbos[3];

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
};

#endif
