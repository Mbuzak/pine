#ifndef MODEL_HPP
#define MODEL_HPP

#include <GL/glew.h>
#include <GL/freeglut.h>
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
	GLuint id_vao_;
	GLuint id_vbo_uv_;

	GLuint id_vbo_coord_;
	GLuint id_vbo_idx_;

	GLfloat positions[8*3] = {
	1.0f, 1.0f, 1.0f,   // 0
	-1.0f, 1.0f, 1.0f,  // 1
	-1.0f, -1.0f, 1.0f, // 2
	1.0f, -1.0f, 1.0f,  // 3
	1.0f, 1.0f, -1.0f,  // 4
	-1.0f, 1.0f, -1.0f, // 5
	-1.0f, -1.0f, -1.0f,// 6
	1.0f, -1.0f, -1.0f  // 7
	};

	GLuint indices[12*3] = {
	5, 0, 1,
	5, 4, 0,
	2, 0, 3,
	2, 1, 0,
	7, 0, 4,
	7, 3, 0,
	3, 6, 2,
	3, 7, 6,
	1, 2, 6,
	1, 6, 5,
	4, 5, 6,
	4, 6, 7
	};

	const char files[6][30] = {
	"skybox/posx.jpg",
	"skybox/negx.jpg",
	"skybox/posy.jpg",
	"skybox/negy.jpg",
	"skybox/posz.jpg",
	"skybox/negz.jpg",
	};

	const GLenum targets[6] = {
	GL_TEXTURE_CUBE_MAP_POSITIVE_X,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};

	void CreateVAO();
	void Draw(GLuint programID, glm::mat4 matProj, glm::mat4 matView);
};


// Visible model on scene
class Model {
public:
	Model(std::string filename);

	void Draw();

private:
	void LoadOBJ(std::string filename);

	void CreateVAO();
	void CreateVBOCoord();
	void CreateVBOUV();
	void CreateVBOColor();

private:
	GLuint id_vao_;
	GLuint id_vbo_coord_;
	GLuint id_vbo_color_;
	GLuint id_vbo_uv_;

	std::vector<glm::vec3> vertices_;
	std::vector<glm::vec2> uvs_;
	std::vector<glm::vec3> normals_;
};

#endif
