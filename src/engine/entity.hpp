#ifndef PINE_ENTITY
#define PINE_ENTITY

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include "material.hpp"
#include "texture.hpp"
#include "mesh.hpp"
#include "uniform.hpp"

#define STANDARD 0
#define OUTLINE 1

// Visible object on scene
class Shape {
public:
	Shape(Mesh*);
	Shape(Mesh*, glm::vec3);
	Shape(Mesh*, glm::vec3, GLuint);
	Shape(Mesh*, glm::vec3, Material&);

	bool HasTexture();
	glm::mat4 CalculateMatModel(int value = 0);
	glm::mat4 ScaledMatModel(float, float, float);
	void Display(GLuint programID, int value = STANDARD);
	void DisplayOutline(GLuint, int);
	void SendMaterial(GLuint, std::string);

	glm::vec3 pos;
	glm::vec3 rot;
	Material material_;
	GLuint texture_;
	Mesh *mesh = nullptr;
};

class Piece: public Shape {
public:
	Piece(int, Mesh*, GLuint);

	std::string get_field();

	void update_field(std::string);
	void update_world_position();
	void update_position();

	bool is_active = true;
	std::string colour;

private:
	std::string field_;
};

#endif
