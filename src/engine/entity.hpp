#ifndef PINE_ENTITY
#define PINE_ENTITY

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "mesh.hpp"

#ifdef __cplusplus
extern "C" {
#endif
#include "material.h"
#include "texture.h"
#include "light.h"
#ifdef __cplusplus
}
#endif

typedef struct {
	glm::vec3 pos;
	glm::vec3 rot;
	float scale;
} Transform;

glm::mat4 transform_model_compute(const Transform*);

struct Shape {
	Transform transform;
	Material material;
	GLuint texture_;
	Mesh* mesh;
};

struct Lamp {
	LightPoint light;
	Transform transform;
};

void shape_init(Shape*, glm::vec3, Mesh*, GLuint);
void lamp_init(Lamp*, glm::vec3);

#endif
