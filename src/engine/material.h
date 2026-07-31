#ifndef PINE_MATERIAL_H
#define PINE_MATERIAL_H

#include <GL/glew.h>
#include <string.h>
#include "cglm/struct.h"

typedef struct {
	vec3s ambient;
	vec3s diffuse;
	vec3s specular;
	float shininess;
} Material;

void uniform_material_send(GLuint, const char*, Material*);

#endif
