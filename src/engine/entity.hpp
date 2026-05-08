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
#include "material.hpp"

#define STANDARD 0
#define OUTLINE 1

typedef struct {
	glm::vec3 pos;
	glm::vec3 rot;
	float scale;
} Transform;

glm::mat4 position_model_compute(const glm::vec3*);
glm::mat4 transform_model_compute(const Transform*);

// Visible object on scene
class Shape {
public:
	Shape() = default;
	Shape(Mesh*);
	Shape(Mesh*, glm::vec3);
	Shape(Mesh*, glm::vec3, GLuint);

	bool HasTexture();
	void Display(GLuint programID, int value = STANDARD);
	void DisplayOutline(GLuint, int);

	Transform transform;
	Material material;
	GLuint texture_;
	Mesh *mesh = nullptr;
};

class Piece {
public:
	Piece(int, Mesh*, GLuint);

	void update_field(std::string);
	void update_world_position();
	void update_position();

	Shape shape;
	bool is_active = true;
	std::string colour;
	std::string field;
};

Shape terrain_init();

void render(GLuint, Shape*);

#endif
