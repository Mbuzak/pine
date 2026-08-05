#ifndef PINE_CAMERA_H
#define PINE_CAMERA_H

#include <GL/glew.h>
#include <math.h>
#include "SDL2/SDL.h"
#include "cglm/struct.h"
#include "shader.h"

typedef struct {
	vec3s pos;
	vec2s rot;
} Camera;

vec3s direction_compute(vec2s);
vec3s right_vector_compute(vec3s);

mat4s camera_view_compute(Camera*);

void camera_move(Camera* camera, int keys_pressed[128], Shader* shaders, int shader_count);

#endif
