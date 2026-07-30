#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "cglm/struct.h"
#include <GL/glew.h>
#include <math.h>

typedef struct {
	vec3s pos;
	vec2s rot;
} Camera;

vec3s direction_compute(vec2s);
vec3s right_vector_compute(vec3s);

mat4s camera_view_compute(Camera*);

#endif
