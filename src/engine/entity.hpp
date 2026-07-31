#ifndef PINE_ENTITY
#define PINE_ENTITY

#include "cglm/struct.h"
#include <GL/glew.h>
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
	vec3s pos;
	vec3s rot;
	float scale;
} Transform;

mat4s transform_model_compute(const Transform*);

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

void shape_init(Shape*, vec3s, Mesh*, GLuint);
void lamp_init(Lamp*, vec3s);

#endif
