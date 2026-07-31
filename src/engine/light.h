#ifndef PINE_LIGHT_H
#define PINE_LIGHT_H

#include <GL/glew.h>
#include <string.h>
#include "cglm/struct.h"

typedef struct {
	vec3s ambient;
	vec3s diffuse;
	vec3s specular;
	vec3s attenuation;
} LightPoint;

// Sun class represents directional lighting
typedef struct {
	vec3s ambient;
	vec3s diffuse;
	vec3s specular;
} Sun;

Sun sun_init();

void uniform_light_point_send(GLuint, const char*, LightPoint*);
void uniform_light_directional_send(GLuint, const char*, Sun*);

#endif
