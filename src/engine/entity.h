#ifndef PINE_ENTITY_H
#define PINE_ENTITY_H

#include "cglm/struct.h"
#include <GL/glew.h>
#include "mesh.h"
#include "material.h"
#include "light.h"

typedef struct {
	vec3s pos;
	vec3s rot;
	float scale;
} Transform;

mat4s transform_model_compute(const Transform* transform);

typedef struct {
	Transform transform;
	Material material;
	Mesh* mesh;
} Shape;

typedef struct {
	LightPoint light;
	Transform transform;
} Lamp;

void shape_init(Shape* shape, vec3s pos, Mesh* mesh);
void lamp_init(Lamp* lamp, vec3s pos);

#endif
