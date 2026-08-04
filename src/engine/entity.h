#ifndef PINE_ENTITY_H
#define PINE_ENTITY_H

#include "cglm/struct.h"
#include <GL/glew.h>
#include "mesh.h"
#include "material.h"
#include "texture.h"
#include "light.h"

typedef struct {
	vec3s pos;
	vec3s rot;
	float scale;
} Transform;

mat4s transform_model_compute(const Transform*);

typedef struct {
	Transform transform;
	Material material;
	GLuint texture_;
	Mesh* mesh;
} Shape;

typedef struct {
	LightPoint light;
	Transform transform;
} Lamp;

void shape_init(Shape*, vec3s, Mesh*, GLuint);
void lamp_init(Lamp*, vec3s);

#endif
