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
#include "mesh.hpp"
#include "uniform.hpp"
#include "material.hpp"
#include "light.hpp"

#ifdef __cplusplus
extern "C" {
#endif
#include "texture.h"
#ifdef __cplusplus
}
#endif

typedef struct {
	glm::vec3 pos;
	glm::vec3 rot;
	float scale;
} Transform;

glm::mat4 transform_model_compute(const Transform*);

// Visible object on scene
class Shape {
public:
	Shape() = default;
	Shape(Mesh*, glm::vec3);
	Shape(Mesh*, glm::vec3, GLuint);

	Transform transform;
	Material material;
	GLuint texture_;
	Mesh *mesh = nullptr;
};

class Piece {
public:
	Piece(Mesh*, GLuint);

	Shape shape;
};

struct Lamp {
	LightPoint light;
	Transform transform;
	Mesh* mesh;
};

void lamp_init(Lamp*, glm::vec3, Mesh*);

#endif
